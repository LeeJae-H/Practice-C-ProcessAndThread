#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <pthread.h>

#define BUF_SIZE 4096

struct thread_args{
	int *arr;
	int offset;
	int size;
};

void *thread_func(void *args);

int find_min_value(int *arr, int offset, int size);
