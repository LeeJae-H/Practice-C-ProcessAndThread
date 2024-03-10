#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#define BUFFER_SIZE 32
#define NUM_PRODUCERS 5
#define NUM_CONSUMERS 5
#define NUM_DATA 1000
int circular_buffer[BUFFER_SIZE];  // 원형 버퍼 배열
int counter = 0;  // 버퍼에 저장된 아이템 수
int in = 0;  // 생산자가 다음에 아이템을 저장할 인덱스
int out = 0;  // 소비자가 다음에 아이템을 소비할 인덱스
sem_t full_sem;  // 버퍼가 가득 찼을 때 기다리는 세마포어
sem_t empty_sem;  // 버퍼가 비어있을 때 기다리는 세마포어
pthread_mutex_t mutex;  // 상호 배제를 위한 뮤텍스

// 생산자 스레드 함수
void *producer(void *arg) {
    int data;
    for (int i = 0; i < NUM_DATA; i++) {
        data = rand() % 100;  // 0부터 99까지의 난수 생성
        if(counter==BUFFER_SIZE){
            printf("prod. full!\n");
        }
        sem_wait(&full_sem);  // 버퍼가 가득 차있으면 기다림
        pthread_mutex_lock(&mutex);  // 뮤텍스 잠금
        circular_buffer[in] = data;  // 데이터를 버퍼에 저장
        in = (in + 1) % BUFFER_SIZE;  // 다음 데이터를 저장할 인덱스 업데이트
        counter++;
        printf("in: %d, counter: %d, data: %d\n", in, counter, data);
        pthread_mutex_unlock(&mutex);  // 뮤텍스 해제
        sem_post(&empty_sem);  // 버퍼가 비어있지 않음을 알림
    }
    pthread_exit(NULL);
}

// 소비자 스레드 함수
void *consumer(void *arg) {
    int data;
    for (int i = 0; i < NUM_DATA; i++) {
        if(counter==0){
            printf("cons. empty!\n");
        }
        sem_wait(&empty_sem);  //버퍼가 비어있으면 기다림
        pthread_mutex_lock(&mutex);  // 뮤텍스 잠금
        data = circular_buffer[out];  // 버퍼에서 데이터를 가져옴
        out = (out + 1) % BUFFER_SIZE;  // 다음 데이터를 소비할 인덱스 업데이트
        counter--;
        printf("out: %d, counter: %d, data: %d\n", out, counter, data);
        pthread_mutex_unlock(&mutex);  // 뮤텍스 해제
        sem_post(&full_sem);  // 버퍼가 가득 차있지 않음을 알림
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t producers[NUM_PRODUCERS];  // 생산자 스레드 배열
    pthread_t consumers[NUM_CONSUMERS];  // 소비자 스레드 배열
    srand(time(NULL));  // 시드 설정
    sem_init(&full_sem, 0, BUFFER_SIZE);  // full 세마포어 초기화
    sem_init(&empty_sem, 0, 0);  // empty 세마포어 초기화
    pthread_mutex_init(&mutex, NULL);  // 뮤텍스 초기화
    // 생산자 스레드 생성
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_create(&producers[i], NULL, producer, NULL);
    }
    // 소비자 스레드 생성
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_create(&consumers[i], NULL, consumer, NULL);
    }
    // 생산자 스레드 종료 대기
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(producers[i], NULL);
    }
    // 소비자 스레드 종료 대기
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(consumers[i], NULL);
    }
    sem_destroy(&full_sem);  // full 세마포어 제거
    sem_destroy(&empty_sem);  // empty 세마포어 제거
    pthread_mutex_destroy(&mutex);  // 뮤텍스 제거
    return 0;
}