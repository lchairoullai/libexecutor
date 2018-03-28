//
// Created by l.hajrullai on 14/03/2018.
//

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

        worker_proc function;
        void *arg;
        ex_task_ *task = queue_dequeue(executor->executor_queue);


        if (task != NULL) {
            function = task->function;
            arg = task->arg;
            function(arg, &executor->global_lock, worker->id);
            free(task);
        }

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