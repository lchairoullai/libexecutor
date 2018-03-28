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
