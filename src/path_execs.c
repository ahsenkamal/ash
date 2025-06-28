#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include "path_execs.h"

// todo: implement cache

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

