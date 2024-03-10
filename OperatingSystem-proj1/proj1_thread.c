#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define BUFFER_SIZE 5 // 공유 버퍼 크기
#define DATA_SIZE 500 // 생성할 데이터 개수
int buffer[BUFFER_SIZE]; // 공유 버퍼
int in = 0; // 생산자가 데이터를 넣을 인덱스
int out = 0; // 소비자가 데이터를 가져올 인덱스
int count = 0; // 현재 버퍼에 들어있는 데이터 개수
void *producer(void *arg); // 생산자 함수
void *consumer(void *arg); // 소비자 함수

int main() {
    pthread_t producer_thread, consumer_thread;
    pthread_create(&producer_thread, NULL, producer, NULL); // 생산자 스레드 생성
    pthread_create(&consumer_thread, NULL, consumer, NULL); // 소비자 스레드 생성
    pthread_join(producer_thread, NULL); // 생산자 스레드가 끝나기를 기다림
    pthread_join(consumer_thread, NULL); // 소비자 스레드가 끝나기를 기다림
}

void *producer(void *arg) {
    int data;
    for(int i=0; i<DATA_SIZE; i++) {
        data = rand() % 1000; // 랜덤 데이터 생성
        buffer[in] = data; // 데이터 생성 후 버퍼에 넣기
        in = (in + 1) % BUFFER_SIZE;
        count++;
        printf("Producer: %d\n", data);
        usleep(10000); // 문맥 교환을 위한 sleep
    }
    pthread_exit(NULL);
}

void *consumer(void *arg) {
    int data;
    for(int i=0; i<DATA_SIZE; i++)
    {
        while (count == 0); // 공유 버퍼에 데이터가 없으면 기다림
        data = buffer[out]; // 버퍼에서 데이터 가져오기
        out = (out + 1) % BUFFER_SIZE;
        count--;
        printf("Consumer: %d\n", data);
        usleep(5000); // 문맥 교환을 위한 sleep
    }
    pthread_exit(NULL);
}