#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "builtins.h"

char *builtins[] = {
    "echo",
    "type",
    "exit"
};

void builtin_echo(char *args[], size_t args_count) {
    for (int i=1; i<args_count; i++) {
        printf("%s ", args[i]);
    }
    printf("\n");
}

void builtin_type(char *args[], size_t args_count) {
    size_t builtins_size = sizeof(builtins)/sizeof(char *);

    for (int i=1; i<args_count; i++) {
        bool found = false;

        for (int j=0; j<builtins_size; j++) {
            if (strcmp(args[i], builtins[j]) == 0) {
                printf("%s is a builtin command\n", args[i]);
                found = true;
            }
        }
        
        if (found) continue;

        char *binary_path = NULL; //find_in_path(args[i]);
        if (binary_path != NULL) {
            printf("%s is %s\n", args[i], binary_path); 
            continue;
        }

        printf("%s %s\n", args[i], "is uknown");
    }
}
