#include "proj1.h"

int main(int argc, char**argv){
    if (argc != 3) {
        printf("Use %s <number of inputs> <number of processes>\n", argv[0]);
        return 0;
    }
    int n_input = atoi(argv[1]);
    int n_process = atoi(argv[2]);

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
	    
        if (pid == 0) {
            int min = child_find_min_value(i, (int *)arr, chunk);
            put_value((int *)arr, n_input, i, min);
            exit(0);
        }
    }

    int status;
    for (int i = 0; i < n_process; i++) {
        wait(&status);
    }

    int min_value = parent_find_min_value(arr, n_input, n_process);

    printf("minimum value: %d\n", min_value);

    munmap(arr, MEM_SIZE);
    shm_unlink(SHM_NAME);
       
    return 0;
}
