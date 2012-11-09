#include "frog_pipe.h"

#include <stdlib.h>
#include <unistd.h>

struct _FrogPipe
{
	int read_fd;
	int write_fd;
};

static Ret frog_pipe_make_socket(int pipes[2]);

static Ret frog_pipe_make_socket(int pipes[2])
{
	int ret = 0;

	ret = pipe(pipes);

	return ret ? RET_FAIL : RET_OK;
}

FrogPipe* frog_pipe_create(void)
{
	FrogPipe* thiz = (FrogPipe*) malloc(sizeof(FrogPipe));
	int pipes[2] = {0};

	if (thiz != NULL) {
		frog_pipe_make_socket(pipes);
		thiz->read_fd = pipes[0];
		thiz->write_fd = pipes[1];
	}

	return thiz;
}

int frog_pipe_read(FrogPipe* thiz, void* buff, size_t length)
{
	return_val_if_fail(thiz != NULL && length > 0, -1);

	return read(thiz->read_fd, buff, length);
}

int frog_pipe_write(FrogPipe* thiz, const void* buff, size_t length)
{
	return_val_if_fail(thiz != NULL, -1);

	return write(thiz->write_fd, buff, length);
}

int frog_pipe_get_read_handle(FrogPipe* thiz)
{
	return_val_if_fail(thiz != NULL, -1);

	return thiz->read_fd;
}

int frog_pipe_get_write_handle(FrogPipe* thiz)
{
	return_val_if_fail(thiz != NULL, -1);

	return thiz->write_fd;
}

int frog_pipe_check(FrogPipe* thiz)
{
	return_val_if_fail(thiz != NULL, -1);

	return -1;
}

void frog_pipe_destroy(FrogPipe* thiz)
{
	if (thiz != NULL) {
		if (thiz->read_fd >= 0) {
			close(thiz->read_fd);
		}

		if (thiz->write_fd >= 0) {
			close(thiz->write_fd);
		}

		free(thiz);
	}
}
