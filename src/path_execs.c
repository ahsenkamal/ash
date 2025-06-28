#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include "path_execs.h"

// todo: implement hashmap

typedef struct PathExecutable {
    char *executable;
    char *fullpath;
} PathExecutable;

PathExecutable *path_executables = NULL;
int path_executables_count = 0;

int read_path() {
    char *path_env = getenv("PATH");
    if (!path_env) return 1;
    
    int count = 0;
    int capacity = 2048;
    path_executables = malloc(capacity * sizeof(PathExecutable));

    char *path = strdup(path_env);
    char *dir_path= strtok(path, ":");
    
    while (dir_path != NULL) {
        DIR *dir = opendir(dir_path);
        if (dir == NULL) goto skip_dir_path;

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_REG || entry->d_type == DT_LNK) {
                char fullpath[512];
                snprintf(fullpath, sizeof(fullpath), "%s/%s", dir_path, entry->d_name);

                if (access(fullpath, X_OK) == 0) {
                    if (count >= capacity) {
                        capacity *= 2;
                        path_executables = realloc(path_executables, capacity * sizeof(PathExecutable));
                    }
                    path_executables[count].executable = strdup(entry->d_name);
                    path_executables[count].fullpath = strdup(fullpath);
                    count++;
                }
            }    
        }

        closedir(dir);

        skip_dir_path:
        dir_path = strtok(NULL, ":");
    }

    free(path);
    path_executables_count = count;
    return 0;
}

char *find_in_path(char *command) {
    for (int i=0; i<path_executables_count; i++) {
        if (strcmp(path_executables[i].executable, command) == 0) {
            return path_executables[i].fullpath;
        }
    } 
    return NULL;
}
