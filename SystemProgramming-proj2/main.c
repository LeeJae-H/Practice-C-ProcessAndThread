#include "proj2.h"

int main(int argc, char **argv){
	if (argc != 2) {
		printf("Use: %s <number of threads>\n", argv[0]);
		return 0;
	}
	int fd,i,num;
	char buf[BUF_SIZE];
	char *p;
	int num_thread = atoi(argv[1]);
	int *input_arr;
	int *output_arr;

	fd = open("./input.txt", O_RDONLY);
	if (fd < 0) {
		perror("open");
		exit(1);
	}

	ssize_t ret = read(fd, buf, BUF_SIZE);
	if (ret < 0) {
		perror("read");
        	close(fd);
        	exit(1);
	}

	p = strtok(buf," ");
	int input_num= atoi(p);

	input_arr = (int *)malloc(input_num * sizeof(int));

	output_arr = (int *)malloc(num_thread * sizeof(int));

	i = 0;
	while(p!=NULL){
		p =strtok(NULL," ");
		if(p!=NULL){
			num = atoi(p);
			input_arr[i++]=num;	
		}
	}

	pthread_t *threads = (pthread_t *)malloc(num_thread * sizeof(pthread_t));

	struct thread_args *args = (struct thread_args *)malloc(num_thread * sizeof(struct thread_args));

	int chunk = input_num / num_thread;
	for (int i = 0; i < num_thread; i++) {
	args[i].arr = input_arr;
	args[i].offset = i * chunk;
	if (i == num_thread - 1) {
	    args[i].size = input_num - i * chunk;
	} else {
	    args[i].size = chunk;
	}
	if (pthread_create(&threads[i], NULL, thread_func, (void *)&args[i]) != 0) {
	    perror("pthread_create");
		exit(1);
	}
	}
    		

	for (int i = 0; i < num_thread; i++) {
	if (pthread_join(threads[i], (void **)&output_arr[i]) != 0) {
	    perror("pthread_join");
		exit(1);
	}
	}
	
	printf("min: %d\n", find_min_value(output_arr,0,num_thread));
	
	free(args);
	free(threads);
	free(output_arr);
	free(input_arr);
	
	close(fd);
	
	return 0;
}
