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
#include <stdio.h>
#include <unistd.h>
#include "ex_worker/ex_worker.h"
#include "libexecutor.h"

#define MAX_POOL_SIZE 128
#define MIN_POOL_SIZE 1

executor executor_create(int workers) {

    if (workers < MIN_POOL_SIZE) {
        FATAL_ERROR("POOL SIZE CANNOT BE LESS THAT 1 THREAD");
    }

    if (workers > MAX_POOL_SIZE) {
        FATAL_ERROR("POOL SIZE CANNOT BE MORE THAT 128 THREADS");
    }

    executor_ *executor;
    executor = Malloc(sizeof(struct executor_));
    executor->workers = Malloc(workers * sizeof(struct worker *));

    executor->on_off = 1; // this will be used to shutdown the executor

    executor->workers_started = 0;
    executor->workers_running = 0;

    executor->executor_queue = queue_init();

    int n = 0; // also represents workers id.
    while (n < workers) {
        worker_start(executor, &executor->workers[n], n);
        n++;
    }

    pthread_mutex_init(&(executor->lock), NULL);
    pthread_mutex_init(&(executor->global_lock), NULL);
    pthread_cond_init(&executor->executor_is_idle, NULL);

    usleep(200000); // wait 200ms until all threads have started

    return executor;
}

void executor_add_task(executor_ *executor, task_proc function, task_arg arg) {
    ex_task_ *task;
    task = Malloc(sizeof(struct ex_task_));

    task->function = (worker_proc) function;
    task->arg = arg;

    queue_enqueue(executor->executor_queue, task);
}

void executor_wait(executor_ *executor) {
    pthread_mutex_lock(&executor->lock);
    while (executor->executor_queue->length || executor->workers_running) {
        pthread_cond_wait(&executor->executor_is_idle, &executor->lock);
    }
    pthread_mutex_unlock(&executor->lock);
}

void executor_shutdown(executor_ *executor) {

    if (executor == NULL) // cannot shutdown an executor that doesnt exist
        return;

    executor->on_off = 0; // inform workers that executor is going to shutdown

    while (executor->workers_started) {
        executor_sem_post(&executor->executor_queue->green_light);
    }

    queue_free(executor->executor_queue);

    pthread_mutex_destroy(&executor->lock);
    pthread_cond_destroy(&executor->executor_is_idle);

    free(executor->workers);
    free(executor);
}


