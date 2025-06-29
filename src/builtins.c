#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include "builtins.h"
#include "path_execs.h"
#include "globals.h"

char *builtins[] = {
    "exit",
    "echo",
    "type",
    "pwd",
    "cd"
};

int run_builtin(char *args[], size_t args_count) {
    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    } else if (strcmp(args[0], "echo") == 0) {
        builtin_echo(args, args_count);
    } else if (strcmp(args[0], "type") == 0) {
        builtin_type(args, args_count);
    } else if (strcmp(args[0], "pwd") == 0) {
        builtin_pwd(args_count);
    } else if (strcmp(args[0], "cd") == 0) {
        builtin_cd(args, args_count);
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

void builtin_pwd(size_t args_count) {
    if (args_count > 1) {
        printf("pwd: too many arguments\n");
        return;
    }

    printf("%s\n", cwd);
}

void builtin_cd(char *args[], size_t args_count) {
    if (args_count > 2) {
        printf("cd: too many arguments\n");
        return;
    }

    char *dir = args[1];
    size_t dir_len = strlen(dir);
    if (dir[dir_len-1] == '/') {
        dir[dir_len-1] = 0;
    }

    char final_path[2048] = "";

    if (dir[0] == '/') {
        strncpy(final_path, dir, sizeof(final_path) - 1);
        goto change_dir;
    }

    char *final_path_tokens[1024];
    int tc = 0;

    char current_dir[2048];
    strncpy(current_dir, cwd, sizeof(current_dir) - 1);

    char *token = strtok(current_dir, "/");
    while (token != NULL && tc < 1024) {
        final_path_tokens[tc++] = token;
        token = strtok(NULL, "/");
    }

    token = strtok(dir, "/");
    while (token != NULL) {
        if (strcmp(token, ".") == 0) {
            goto next_token;
        } else if (strcmp(token, "..") == 0) {
            if (tc == 0) goto next_token;

            final_path_tokens[tc - 1] = NULL;
            tc--; 
        } else {
            final_path_tokens[tc++] = token;
        }

        next_token:
        token = strtok(NULL, "/");
    }

    for (int i=0; i<tc; i++) {
        strncat(final_path, "/", sizeof(final_path) - strlen(final_path) - 1);
        strncat(final_path, final_path_tokens[i], sizeof(final_path) - strlen(final_path) - 1);
    }

    change_dir:
    
    struct stat st;

    if (stat(final_path, &st) != 0) {
        printf("cd: no such file or directory\n");
    } else if (access(final_path, X_OK) != 0) {
        printf("cd: permission denied\n");
    } else if (!S_ISDIR(st.st_mode)) {
        printf("cd: %s is not a directory\n", dir);
    } else {
        chdir(final_path);
        strncpy(cwd, final_path, sizeof(cwd) - 1); 
    }
}
