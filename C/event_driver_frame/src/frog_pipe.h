#ifndef FROG_PIPE_H
#define FROG_PIPE_H

#include "typedef.h"

DECLES_BEGIN

struct _FrogPipe;
typedef struct _FrogPipe FrogPipe;

FrogPipe* frog_pipe_create(void);

int frog_pipe_read(FrogPipe* thiz, void* buff, size_t length);
int frog_pipe_write(FrogPipe* thiz, const void* buff, size_t length);
int frog_pipe_get_read_handle(FrogPipe* thiz);
int frog_pipe_get_write_handle(FrogPipe* thiz);
int frog_pipe_check(FrogPipe* thiz);

void frog_pipe_destroy(FrogPipe* thiz);

DECLES_END

#endif // FROG_PIPE_H
