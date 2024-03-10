#include "proj1.h"

void generate_input(int*arr, int ninput){
   srand(time(NULL));
   int result = 0;
   int cnt =0;
   while(cnt < ninput){
   	result = rand()%BILLION;
	arr[cnt]=result;
	cnt++;
   } 
}

void *sets_shared_memory(){ // FIX ME
    void *ret = NULL;
    int fd = shm_open(SHM_NAME, O_RDWR|O_CREAT, 0666);

    if (fd == -1) {
        perror("open failed\n");
    }
    ftruncate(fd, MEM_SIZE);
    ret = mmap(0, MEM_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    return ret;
}

int child_find_min_value(int id, int *arr, int chunk){ // FIX ME
    int min = BILLION;
    int start = id * chunk;
    int end = start + chunk;

    for (int i = start; i < end; i++) {
        if (arr[i] < min) {
            min = arr[i];
        }
    }

    return min;
}

void put_value(int *arr, int n_input, int id, int min){ // FIX ME
    int *dest = ((int *)arr) + (n_input + id);
    *dest = min;
    return;
}

int parent_find_min_value(void *arr,int n_input, int n_process){ // FIX ME
	int min = BILLION;

    for (int i = 0; i < n_process; i++) {
        int *result = (int *)arr + (n_input + i);
        if (*result < min) {
            min = *result;
        }
    }

	return min;
}
