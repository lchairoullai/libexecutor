//
// Created by l.hajrullai on 14/03/2018.
//

/* Report an error and abort */
#include <stdlib.h>
#include <stdio.h>

#define FATAL_ERROR(message)                        \
  {                                    \
    fprintf(stderr,"In %s(%d) [function %s]: %s\n",            \
        __FILE__, __LINE__, __FUNCTION__ , (message)  );        \
    abort();                                \
  }                                    \

/* Report a posix error (similar to perror) and abort */
#define FATAL_PERROR(errcode) FATAL_ERROR(strerror(errcode))

/* check return code of command and abort on error with suitable
   message */

#define CHECK_ERRNO(command) { if((command) == -1) FATAL_PERROR(errno); }

void *Malloc(size_t n);

/* Type of function that can be ran in a worker */
typedef void *(*worker_proc)(void *, void *, int);
