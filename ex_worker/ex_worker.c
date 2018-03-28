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

#include "ex_worker.h"

int worker_start(executor_ *executor, struct worker **worker, int id) {
    *worker = Malloc(sizeof(struct worker));
    (*worker)->executor = executor;
    (*worker)->id = id;
    pthread_create(&(*worker)->pthread, NULL, (void *) worker_loop, (*worker));
    pthread_detach((*worker)->pthread);
    return 0;
}

void *worker_loop(struct worker *worker) {

    executor_ *executor = worker->executor;

    pthread_mutex_lock(&executor->lock);
    executor->workers_started++;
    pthread_mutex_unlock(&executor->lock);


    while (1) {

        if (executor->on_off == 0)
            break;

        executor_sem_wait(&executor->executor_queue->green_light);

        pthread_mutex_lock(&executor->lock);
        executor->workers_running++;
        pthread_mutex_unlock(&executor->lock);

        execute_task_if_exists(worker);

        pthread_mutex_lock(&executor->lock);
        executor->workers_running--;
        if (!executor->workers_running) {
            pthread_cond_signal(&executor->executor_is_idle);
        }
        pthread_mutex_unlock(&executor->lock);

    }

    pthread_mutex_lock(&executor->lock);
    executor->workers_started--;
    pthread_mutex_unlock(&executor->lock);

    return NULL;
}

void *execute_task_if_exists(struct worker *worker) {

    executor_ *executor = worker->executor;

    worker_proc function;
    void *arg;
    ex_task_ *task = queue_dequeue(executor->executor_queue);

    if (task != NULL) {
        function = task->function;
        arg = task->arg;
        function(arg, &executor->global_lock, worker->id);
        free(task);
    }

}