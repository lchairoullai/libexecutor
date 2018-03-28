//
// Created by l.hajrullai on 14/03/2018.
//

typedef void *(*task_proc)(void *);
typedef void *(task_arg);

typedef struct executor_ *executor;

executor executor_create(int workers);

void executor_add_task(executor, task_proc, task_arg);

void executor_wait(executor);

void executor_shutdown(executor);

