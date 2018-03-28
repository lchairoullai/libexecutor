//
// Created by l.hajrullai on 14/03/2018.
//

#include <pthread.h>

typedef struct executor_semaphore_ {
    pthread_mutex_t mutex;
    pthread_cond_t cvar;
    char v;
} executor_semaphore_;

void executor_sem_init(executor_semaphore_ *p, char value);

void executor_sem_post(executor_semaphore_ *p);

void executor_sem_wait(executor_semaphore_ *p);
