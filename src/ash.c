#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "builtins.h"
#include "path_execs.h"
#include "globals.h"

#define INPUT_BUFFER_SIZE 1024
#define ARG_MAX 100

char cwd[2048];

void read_input(char *buffer, size_t buffer_size) {
    fgets(buffer, buffer_size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

void run_command(char *args[], size_t args_count, int fd) {
    if (run_builtin(args, args_count, fd) != 0 && run_executable(args, args_count, fd) != 0) {
        printf("%s: %s\n", args[0], "invalid command or arguments");
    }
}

void handle_pipe(char *arg) {

}

int main() {
    if (getcwd(cwd, sizeof(cwd)) == NULL) return 1;

    while (1) {
        char buffer[INPUT_BUFFER_SIZE];
        memset(buffer, 0, INPUT_BUFFER_SIZE);
        int saved_stdout = dup(STDOUT_FILENO);
        int fd = STDOUT_FILENO;
        int pipefd[2];
        pipe(pipefd);
        
        printf("(%s) $ ", cwd);
        read_input(buffer, INPUT_BUFFER_SIZE);

        char *args[ARG_MAX] = {NULL};
        
        int args_count = 0;
        char *arg = strtok(buffer, " ");
        
        if (arg == NULL) continue;

        while (arg != NULL) {
            if (args_count == ARG_MAX-1) {
                printf("Error: Too many arguments!\n");
                return 1;
            }
            
            if (strcmp(arg, ">") == 0) {
                arg = strtok(NULL, " ");
                fd = open(arg, O_WRONLY | O_CREAT | O_TRUNC, 0644);

                if (fd < 0) {
                    printf("Error opening file %s", arg);
                }
                break;
            } else if (strcmp(arg, "|") == 0) {
                arg = strtok(NULL, " ");

                pid_t p1 = fork();
                if (p1 == 0) {
                    dup2(pipefd[0], STDIN_FILENO);
                    args_count = 0;
                    memset(args, 0, sizeof(args));
                } else {
                    dup2(pipefd[1], STDOUT_FILENO);
                    fd = pipefd[1];
                    break;
                }
            }

            args[args_count++] = arg;
            arg = strtok(NULL, " ");
        }

        run_command(args, args_count, fd);
        dup2(saved_stdout, STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
    }

    return 0;
}
