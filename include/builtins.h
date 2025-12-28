#ifndef BUILTINS_H
#define BUILTINS_H

int run_builtin(char *args[], size_t args_count, int fd);
void builtin_echo(char *args[], size_t args_count, int fd);
void builtin_type(char *args[], size_t args_count, int fd);
void builtin_pwd(size_t args_count, int fd);
void builtin_cd(char *args[], size_t args_count);
void builtin_jobs(char *args[], size_t args_count);

#endif
