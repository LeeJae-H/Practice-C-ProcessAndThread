#include "proj1.h"

int main(int argc, char**argv){
	// 1. Set the number of input and process using arguments.
    if (argc != 3) {
        printf("Use %s <number of inputs> <number of processes>\n", argv[0]);
        return 0;
    }
    int n_input = atoi(argv[1]);
    int n_process = atoi(argv[2]);

	// 2. Fill the sets_shared memory function in proj1.c file
	void *arr= sets_shared_memory();
	generate_input((int *)arr,n_input);

    pid_t pid;
    int chunk = n_input / n_process;

    for (int i = 0; i < n_process; i++) {
        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        }

        // 3-1. Create child processes and divides the numbers..
        //
	    // 3-2. Childs: Find the minimum value from their chunk.
	    // 4. Childs: After finding the minimum value, store it to the share memory spaces.
	    //            Dest : arr + (size of int * number of input + child process index)
        if (pid == 0) {
            int min = child_find_min_value(i, (int *)arr, chunk);
            put_value((int *)arr, n_input, i, min);
            exit(0);
        }
    }

	//5-1. Parent waits and reaps child processes
    int status;
    for (int i = 0; i < n_process; i++) {
        wait(&status);
    }

	//5-2. Find min values from the results of child processes.
	//parent_find_min_value(arr, n_input, n_process);
    int min_value = parent_find_min_value(arr, n_input, n_process);

	//6-1. Print the minimum value.
    printf("minimum value: %d\n", min_value);

	//6-2. Unmap and Unlink shared memory region
    munmap(arr, MEM_SIZE);
    shm_unlink(SHM_NAME);
       
	return 0;
}
