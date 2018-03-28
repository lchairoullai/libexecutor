//
// Created by l.hajrullai on 14/03/2018.
//

#include <assert.h>
#include "ex_queue.h"

ex_queue *queue_init(void) {
    ex_queue *queue;
    queue = Malloc(sizeof(struct ex_queue));
    queue->first = queue->last = NULL;
    queue->length = 0;
    pthread_mutex_init(&queue->mutex, NULL);
    executor_sem_init(&queue->green_light, 'R');
    return queue;
}

void queue_enqueue(struct ex_queue *queue, struct ex_task_ *task) {

    pthread_mutex_lock(&queue->mutex);
    struct ex_task_node *node;

    node = malloc(sizeof(struct ex_task_node));
    assert(node);

    node->task = task;
    node->next = NULL;

    if (queue->first == NULL) {
        queue->first = node;
    } else {
        queue->last->next = node;
    }

    queue->last = node;
    queue->length++;

    executor_sem_post(&queue->green_light);
    pthread_mutex_unlock(&queue->mutex);
}

ex_task_ *queue_dequeue(struct ex_queue *queue) {
    pthread_mutex_lock(&queue->mutex);
    ex_task_ *task;
    ex_task_node *node;

    if (queue_is_empty(queue)) {
        pthread_mutex_unlock(&queue->mutex);
        return NULL;
    }

    task = queue->first->task;
    node = queue->first;
    queue->first = node->next;

    if (queue->length == 1) {
        queue->length = 0;
    }

    if (queue->length > 1) {

        queue->length--;
        executor_sem_post(&queue->green_light);
    }

    free(node);
    pthread_mutex_unlock(&queue->mutex);
    return task;
}

int queue_is_empty(const struct ex_queue *queue) {
    return (queue->first == NULL);
}

void queue_free(struct ex_queue *queue) {
    while (!queue_is_empty(queue)) {
        queue_dequeue(queue);
    }
    free(queue);
}