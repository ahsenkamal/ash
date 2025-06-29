#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>
#include "path_execs.h"

// todo: implement cache

int run_executable(char *args[], size_t args_count) {
    pid_t pid = fork();
    if (pid == 0) {
        execvp(args[0], args);
        exit(127);
    } else if (pid > 0) {
        int status;
        wait(&status);
        
        if (WIFEXITED(status) && WEXITSTATUS(status) == 127) {
            return 1;
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

