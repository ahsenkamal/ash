#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "path_execs.h"
#include "jobs.h"
#include <stdbool.h>
#include "globals.h"

// todo: implement cache
void sigchld_handler(int sig) {
    (void)sig;

    pid_t pid;
    int status;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("\nBackground job %d exited...\n", pid);
        delete_job(pid);
    }
}

int run_executable(char *args[], size_t args_count, int fd, bool background) {
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &sa, NULL);

    pid_t pid = fork();
    if (pid == 0) {
        setpgid(0, 0);
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
        dup2(fd, STDOUT_FILENO);
        execvp(args[0], args);
        exit(127);
    } else if (pid > 0) {
        setpgid(pid, pid);

        if (!background) {
            tcsetpgrp(STDIN_FILENO, pid);

            int status;
            waitpid(-pid, &status, WUNTRACED);
            
            if (WIFEXITED(status) && WEXITSTATUS(status) == 127) {
                return 1;
            } else if (WIFSTOPPED(status)) {
                add_job(pid, STOPPED, args);
            }

            tcsetpgrp(STDIN_FILENO, getpgrp());
        } else {
            job *j = add_job(pid, RUNNING, args);
            printf("[%d] %d\n", j->job_id, pid);
        }

        return 0;
    } else {
        return 1;
    }
}

char* find_in_path(char *command) {
    char *path_env = getenv("PATH");
    if (!path_env) return NULL;
    
    char *path = strdup(path_env);
    char *dir_path= strtok(path, ":");
    char fullpath[512];
    
    while (dir_path != NULL) {
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dir_path, command);

        if (access(fullpath, X_OK) == 0) {
            free(path);
            return strdup(fullpath);
        }    

        dir_path = strtok(NULL, ":");
    }

    free(path);
    return NULL;
}

