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