#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define MAX_CARS 1000
#define MAX_BRIDGE_CARS 5

sem_t leftSem, rightSem;
sem_t mutex; // Semaphore for implementing &mutex lock

int leftCars, rightCars;
int left_waiting=0, right_waiting=0;
int left_active=0, right_active=0;
void passing();

void passing(int dir, int args);

void* left(void* args) {
    sem_wait(&mutex);
    if((right_waiting+right_active==0)&&(left_active<MAX_BRIDGE_CARS)){
        left_active++;
        sem_post(&leftSem);
    }
    else{
        left_waiting++;
    }
    sem_post(&mutex);
    sem_wait(&leftSem);

    passing(0, *(int*)args);

    sem_wait(&mutex);
    left_active--;
    if((left_active==0)&&(right_waiting>0)){
        int i;
        for(i=0;i<MAX_BRIDGE_CARS;i++){
            if(right_waiting>0){
                right_waiting--;
                right_active++;
                sem_post(&rightSem);
            }
            else{
                break;
            }
        }
    }
    else if((right_waiting==0)&&(left_waiting>0)){
        sem_post(&leftSem);
        right_active++;
        right_waiting--;
    }
    sem_post(&mutex);
    pthread_exit(NULL);
}

void* right(void* args) {
    sem_wait(&mutex);
    if((left_waiting+left_active==0)&&(right_active<MAX_BRIDGE_CARS)){
        right_active++;
        sem_post(&rightSem);
    }
    else{
        right_waiting++;
    }
    sem_post(&mutex);
    sem_wait(&rightSem);

    passing(1, *(int*)args);

    sem_wait(&mutex);
    right_active--;
    if((right_active==0)&&(left_waiting>0)){
        int i;
        for(i=0;i<MAX_BRIDGE_CARS;i++){
            if(left_waiting>0){
                left_waiting--;
                left_active++;
                sem_post(&leftSem);
            }
            else{
                break;
            }
        }
    }
    else if((left_waiting==0)&&(right_waiting>0)){
        sem_post(&rightSem);
        left_active++;
        left_waiting--;
    }
    sem_post(&mutex);
    pthread_exit(NULL);
}

void passing(int dir, int args) {
    if(dir==1){
        printf("Car %d is crossing the bridge from right to left\n", (args));
    }
    else{
        printf("Car %d is crossing the bridge from left to right\n", (args));
    }
}

int main() {
    pthread_t leftThreads[MAX_CARS], rightThreads[MAX_CARS];
    int leftCarIDs[MAX_CARS], rightCarIDs[MAX_CARS];

    sem_init(&leftSem, 0, 1);
    sem_init(&rightSem, 0, 1);
    sem_init(&mutex, 0, 1); 

    printf("Enter number of cars on the left side: ");
    scanf("%d", &leftCars);
    printf("Enter number of cars on the right side: ");
    scanf("%d", &rightCars);

    for (int i = 0; i < leftCars; ++i) {
        leftCarIDs[i] = i;
        pthread_create(&leftThreads[i], NULL, left, &leftCarIDs[i]);
    }

    for (int i = 0; i < rightCars; ++i) {
        rightCarIDs[i] = leftCars + i;
        pthread_create(&rightThreads[i], NULL, right, &rightCarIDs[i]);
    }

    for (int i = 0; i < leftCars; ++i) {
        pthread_join(leftThreads[i], NULL);
    }

    for (int i = 0; i < rightCars; ++i) {
        pthread_join(rightThreads[i], NULL);
    }

    sem_destroy(&leftSem);
    sem_destroy(&rightSem);
    sem_destroy(&mutex); 

    return 0;
}
