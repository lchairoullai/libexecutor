//
// Created by l.hajrullai on 14/03/2018.
//

#include "../ex_commons/ex_commons.h"
#include "../ex_semaphore/ex_semaphore.h"

typedef struct ex_task_ *ex_task;

typedef struct ex_task_ {
    worker_proc function;
    void *arg;
} ex_task_;

typedef struct ex_task_node {
    struct ex_task_node *next;
    ex_task task;
} ex_task_node;

typedef struct ex_queue {
    pthread_mutex_t mutex;
    executor_semaphore_ green_light;
    ex_task_node *first;
    ex_task_node *last;
    int length;
} ex_queue;

ex_queue *queue_init(void);

void queue_enqueue(struct ex_queue *queue, struct ex_task_ *task);

ex_task_ *queue_dequeue(struct ex_queue *queue);

int queue_is_empty(const struct ex_queue *queue);

void queue_free(struct ex_queue *queue);