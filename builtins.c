#include <stdio.h>
#include <string.h>
#include "builtins.h"

char *builtins[] = {
    "echo",
    "type",
    "exit"
};

void builtin_echo(char *buffer, size_t buffer_size) {
    for (int i=5; i<buffer_size; i++) {
        printf("%c", buffer[i]);
    }
    printf("\n");
}

void builtin_type(char *buffer, size_t buffer_size) {
    size_t builtins_size = sizeof(builtins)/sizeof(char *);
    char *arg = buffer+5;
    size_t arg_size = buffer_size-5;

    for (int i=0; i<builtins_size; i++) {
        if (strncmp(builtins[i], arg, arg_size) == 0) {
            printf("%s %s\n", arg, "is a builtin command");
            return;
        }
    }

    printf("%s %s\n", arg, "is uknown");
}
