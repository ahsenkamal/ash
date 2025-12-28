#ifndef PATH_EXECS_H
#define PATH_EXECS_H
#include <stdbool.h>

int run_executable(char *args[], size_t args_count, int fd, bool background);
char *find_in_path(char *command);

#endif
