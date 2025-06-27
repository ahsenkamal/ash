#include <stdio.h>
#include <string.h>
#include "builtins.h"

#define INPUT_BUFFER_SIZE 1024
#define ARG_MAX 100

size_t read_input(char *buffer, size_t buffer_size) {
    fgets(buffer, buffer_size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    return strlen(buffer);
}

int main() {
    int exit_status = 0;

    while (exit_status == 0) {
        char buffer[INPUT_BUFFER_SIZE];
        memset(buffer, 0, INPUT_BUFFER_SIZE);
        
        printf("$ ");
        size_t input_size = read_input(buffer, INPUT_BUFFER_SIZE);

        char *args[ARG_MAX] = {NULL};
        
        int args_count = 0;
        char *arg = strtok(buffer, " ");;

        while (arg != NULL) {
            if (args_count == ARG_MAX) {
                printf("Error: Too many arguments!\n");
                return 1;
            }
            args[args_count++] = arg;
            arg = strtok(NULL, " ");
        }

        if (strcmp(args[0], "echo") == 0) {
            builtin_echo(args, args_count);
        } else if (strcmp(args[0], "type") == 0) {
            builtin_type(args, args_count);
        } else if (strcmp(buffer, "exit") == 0) {
            exit_status = 1;
        } else {
            printf("%s: %s\n", buffer, "invalid command or arguments");
        }
    }

    return 0;
}
