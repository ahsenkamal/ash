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

job* add_job(pid_t pid, job_state state, char **args) {
    int idx = find_free_slot();
    if (idx < 0)
        return NULL;

    job* j = malloc(sizeof(job));
    j->job_id = next_job_id;
    j->pgid = pid;
    j->state = state;
    char *command = get_command(args);
    strncpy(j->command, command, sizeof(j->command)-1);

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

char *get_command(char **args) {
    size_t len = 0;

    for (int i = 0; args[i]; i++) {
        len += strlen(args[i]);
        if (args[i + 1])
            len += 1;
    }

    char *cmd = malloc(len + 1);
    if (!cmd)
        return NULL;

    char *p = cmd;

    for (int i = 0; args[i]; i++) {
        size_t l = strlen(args[i]);
        memcpy(p, args[i], l);
        p += l;

        if (args[i + 1])
            *p++ = ' ';
    }

    *p = '\0';
    return cmd;
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

int count_jobs() {
    int count = 0;
    for (int i=0; i<MAX_JOBS; i++) {
        if (jobs[i]==NULL) {
            continue;
        }
        count++;
    }
    return count;
}

job *find_job_by_id(int id) {
    for (int i=0; i<MAX_JOBS; i++) {
        if (jobs[i]==NULL) {
            continue;
        } else if (jobs[i]->job_id == id) {
            return jobs[i];
        }
    }
    return NULL;
}