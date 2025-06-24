#include <stdio.h>
#include <string.h>
#include "builtins.h"

#define INPUT_BUFFER_SIZE 1024

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

        if (strncmp(buffer, "echo ", 5) == 0) {
            builtin_echo(buffer, input_size);
        } else if (strncmp(buffer, "type ", 5) == 0) {
            builtin_type(buffer, input_size);
        } else if (strcmp(buffer, "exit") == 0) {
            exit_status = 1;
        } else {
            printf("%s: %s\n", buffer, "command not found");
        }
    }

    return 0;
}
