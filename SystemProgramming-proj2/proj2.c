#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <pthread.h>


struct thread_args{
	int *arr;
	int offset;
	int size;
};

void *thread_func(void *args){
	int min =- -1;

	//implement the code
	struct thread_args* threadArgs = (struct thread_args *)args;
    int *arr = threadArgs->arr;
    int offset = threadArgs->offset;
    int size = threadArgs->size;
	min = arr[offset];
    for (int i = offset+1; i < offset + size; i++) {
        if (arr[i] < min) {
            min = arr[i];
        }
    }

	return min;
}

int find_min_value(int *arr, int offset, int size){
	int min =- -1;
	
	//implement the code
	min = arr[offset];
    for (int i = offset+1; i < offset + size; i++) {
        if (arr[i] < min) {
            min = arr[i];
        }
    }	

	return min;
}
