#ifndef RUNNER_H
#define RUNNER_H

typedef struct runner
{
    int id;
    int preparation_time;
    int running_time;
} runner_t;

runner_t* runner_create(int id, int preparation_time, int running_time);
void runner_destroy(runner_t* runner);

void* run(void* args);

#endif
