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

#include <pthread.h>
#include "../ex_queue/ex_queue.h"

typedef struct worker {
    int id; // auto-incrementing id e.g. 0,1,3, -- n. To be used in cases where whe want to reduce operations
    pthread_t pthread;
    struct executor_ *executor;
} worker;

/**
 *  I have used volatile because it is crucial that the compiler does not try
 *  to optimize these variables, because I don't want to lose any cycle while
 *  checking if the executor has been turned off
 *
 */

typedef struct executor_ {
    volatile int on_off;
    worker **workers;
    volatile int workers_started;
    volatile int workers_running;
    pthread_mutex_t lock;
    pthread_mutex_t global_lock; // global_lock is aimed to be acquired by the task if needed - optional in the API
    pthread_cond_t executor_is_idle;
    ex_queue *executor_queue;
} executor_;

int worker_start(struct executor_ *executor, struct worker **worker, int id);

void *worker_loop(struct worker *worker);

void *execute_task_if_exists(struct worker *worker);