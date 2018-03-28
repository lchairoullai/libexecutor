//
// Created by l.hajrullai on 14/03/2018.
//
#include <stdlib.h>
#include <pthread.h>

#include "ex_semaphore.h"

void executor_sem_init(executor_semaphore_ *p, char value) {
    pthread_mutex_init(&(p->mutex), NULL);
    pthread_cond_init(&(p->cvar), NULL);

    if (!value)
        p->v = 'R'; //RED-LIGHT
    else
        p->v = value;
}

void executor_sem_post(executor_semaphore_ *p) {
    pthread_mutex_lock(&p->mutex);
    p->v = 'G'; // GREEN-LIGHT
    pthread_cond_signal(&p->cvar);
    pthread_mutex_unlock(&p->mutex);
}

void executor_sem_wait(executor_semaphore_ *p) {
    pthread_mutex_lock(&p->mutex);

    /**
     * Because the condition can change before an awakened thread returns from pthread_cond_wait(),
     * the condition that caused the wait must be retested before the mutex lock is acquired.
    **/

    while (p->v != 'G')
        pthread_cond_wait(&p->cvar, &p->mutex);
    p->v = 0;

    pthread_mutex_unlock(&p->mutex);
}


