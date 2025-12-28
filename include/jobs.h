#ifndef JOBS_H
#define JOBS_H
#include <sys/types.h>

typedef enum {
    RUNNING,
    STOPPED
} job_state;

typedef struct {
    int job_id;
    pid_t pgid;
    job_state state;
    char command[256];
} job;

job* add_job(pid_t pid, job_state state, char **args);
void delete_job(pid_t pgid);
void list_jobs();
int count_jobs();
job *find_job_by_id(int id);
char *get_command(char **args);

#endif