#include <stdio.h>
#include <string.h>
#include <unistd.h>
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

void run_command(char *args[], size_t args_count) {
    if (run_builtin(args, args_count) != 0 && run_executable(args, args_count) != 0) {
        printf("%s: %s\n", args[0], "invalid command or arguments");
    }
}

int main() {
    if (getcwd(cwd, sizeof(cwd)) == NULL) return 1;

    while (1) {
        char buffer[INPUT_BUFFER_SIZE];
        memset(buffer, 0, INPUT_BUFFER_SIZE);
        
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
            args[args_count++] = arg;
            arg = strtok(NULL, " ");
        }

        run_command(args, args_count);
    }

    return 0;
}
