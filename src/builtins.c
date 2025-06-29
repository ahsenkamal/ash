#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "builtins.h"
#include "path_execs.h"
#include "globals.h"

char *builtins[] = {
    "exit",
    "echo",
    "type",
    "pwd"
};

int run_builtin(char *args[], size_t args_count) {
    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    } else if (strcmp(args[0], "echo") == 0) {
        builtin_echo(args, args_count);
    } else if (strcmp(args[0], "type") == 0) {
        builtin_type(args, args_count);
    } else if (strcmp(args[0], "pwd") == 0) {
        builtin_pwd();
    } else {
        return 1;
    }

    return 0;
}

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

        char *binary_path = find_in_path(args[i]);
        if (binary_path != NULL) {
            printf("%s is %s\n", args[i], binary_path); 
            free(binary_path);
            continue;
        }

        printf("%s %s\n", args[i], "is uknown");
    }
}

void builtin_pwd() {
    printf("%s\n", cwd);
}
