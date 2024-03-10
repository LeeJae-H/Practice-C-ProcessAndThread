#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#define BUFFER_SIZE 500

int main() {
    int i, pid, pipefd[2], buffer[BUFFER_SIZE];

    // 파이프 생성
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork(); // 자식 프로세스 생성

    if (pid == -1) { // fork 실패
        perror("fork");
        exit(EXIT_FAILURE);
        
    } else if (pid == 0) { // 자식 프로세스
        close(pipefd[1]); // 파이프의 쓰기측 닫기
        for (i = 0; i < BUFFER_SIZE; i++) {
            read(pipefd[0], &buffer[i]); // 파이프로부터 데이터 읽기
            printf("Consumer: %d\n", buffer[i]); // 소비자가 데이터 출력
            usleep(5000); // 문맥 교환을 위한 sleep
        }
        close(pipefd[0]); // 파이프의 읽기측 닫기
        _exit(EXIT_SUCCESS);

    } else { // 부모 프로세스
        close(pipefd[0]); // 파이프의 읽기측 닫기
        for (i = 0; i < BUFFER_SIZE; i++) {
            buffer[i] = rand() % 1000; // 랜덤 데이터 생성
            write(pipefd[1], &buffer[i]); // 파이프에 데이터 쓰기
            printf("Producer: %d\n", buffer[i]); // 생산자가 데이터 출력
            usleep(10000); // 문맥 교환을 위한 sleep
        }
        close(pipefd[1]); // 파이프의 쓰기측 닫기
        wait(NULL); // 자식 프로세스가 끝나기를 기다림
        exit(EXIT_SUCCESS);
    }
}