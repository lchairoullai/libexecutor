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