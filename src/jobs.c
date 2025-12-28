#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include "builtins.h"
#include "path_execs.h"
#include "globals.h"
#include "jobs.h"

#define MAX_JOBS 64

static job *jobs[MAX_JOBS];
static int next_job_id = 1;

static int find_free_slot(void) {
    for (int i = 0; i < MAX_JOBS; i++) {
        if (jobs[i] == NULL)
            return i;
    }
    return -1;
}

job* add_job(pid_t pid, job_state state, char *command) {
    int idx = find_free_slot();
    if (idx < 0)
        return NULL;

    job* j = malloc(sizeof(job));
    j->job_id = next_job_id;
    j->pgid = pid;
    j->state = state;

    strncpy(j->command, command, sizeof(j->command) - 1);
    j->command[sizeof(j->command) - 1] = '\0';

    jobs[idx] = j;
    next_job_id++;
    return j;
}

void delete_job(pid_t pgid) {
    for (int i = 0; i < MAX_JOBS; i++) {
        if (jobs[i] && jobs[i]->pgid == pgid) {
            free(jobs[i]);
            jobs[i] = NULL;
            return;
        }
    }
}

void list_jobs(void) {
    for (int i = 0; i < MAX_JOBS; i++) {
        if (jobs[i] == NULL)
            continue;

        const char *state =
            (jobs[i]->state == RUNNING) ? "Running" : "Stopped";

        printf("[%d] %-8s %s\n",
               jobs[i]->job_id,
               state,
               jobs[i]->command);
    }
}
