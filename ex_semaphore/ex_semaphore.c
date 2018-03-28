//
// Created by l.hajrullai on 14/03/2018.
//

/**
MIT License

Copyright (c) 2018

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

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

    while (p->v != 'G') // Wait until green light
        pthread_cond_wait(&p->cvar, &p->mutex);
    p->v = 'R'; // Turn to red light

    pthread_mutex_unlock(&p->mutex);
}


