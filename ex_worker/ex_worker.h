//
// Created by l.hajrullai on 14/03/2018.
//

#include <pthread.h>
#include "../ex_queue/ex_queue.h"

typedef struct worker {
    int id;
    pthread_t pthread;
    struct executor_ *executor;
} worker;

typedef struct executor_ {
    volatile int on_off; //must be volatile
    worker **workers;
    volatile int workers_started;
    volatile int workers_running;
    pthread_mutex_t lock;
    pthread_mutex_t global_lock;
    pthread_cond_t executor_is_idle;
    ex_queue *executor_queue;
} executor_;




int worker_start(struct executor_ *executor, struct worker **worker, int id);

void *worker_loop(struct worker *worker);