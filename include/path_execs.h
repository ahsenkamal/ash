#ifndef PATH_EXECS_H
#define PATH_EXECS_H

int run_executable(char *args[], size_t args_count);
char *find_in_path(char *command);

#endif
