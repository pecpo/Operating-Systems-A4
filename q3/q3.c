#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define MAX_CARS 10000
#define MAX_BRIDGE_CARS 5

sem_t leftSem, rightSem;
sem_t mutex; 

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
        int i=MAX_BRIDGE_CARS;
        while(i--){
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
        left_active++;
        left_waiting--;
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
        int i=MAX_BRIDGE_CARS;
        while(i--){
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
        right_active++;
        right_waiting--;
    }
    sem_post(&mutex);
    pthread_exit(NULL);
}

void passing(int dir, int args) {
    if(dir==1){
        usleep(1000);
        printf("Car %d is crossing the bridge from right to left\n", (args));
    }
    else{
        usleep(1000);
        printf("Car %d is crossing the bridge from left to right\n", (args));
    }
}

int main() {
    pthread_t leftThreads[MAX_CARS], rightThreads[MAX_CARS];
    int leftCarIDs[MAX_CARS], rightCarIDs[MAX_CARS];

    sem_init(&rightSem, 0, 0);
    if (sem_init(&rightSem, 0, 0) != 0) {
        perror("Failed to initialize rightSem");
        exit(1);
    }

    sem_init(&mutex, 0, 1);
    if (sem_init(&mutex, 0, 1) != 0) {
        perror("Failed to initialize mutex");
        exit(1);
    }

    sem_init(&leftSem, 0, 0);
    if (sem_init(&leftSem, 0, 0) != 0) {
        perror("Failed to initialize leftSem");
        exit(1);
    }

    printf("Enter number of cars on the left side: ");
    scanf("%d", &leftCars);
    printf("Enter number of cars on the right side: ");
    scanf("%d", &rightCars);
    if(leftCars<0 || rightCars<0){
        printf("Invalid number of cars.\n");
        exit(1);
    }

    for (int i = 0; i < leftCars; ++i) {
        leftCarIDs[i] = i;
        int createLeftThreadResult = pthread_create(&leftThreads[i], NULL, left, &leftCarIDs[i]);
        if (createLeftThreadResult != 0) {
            perror("Failed to create left thread");
            exit(1);
        }
    }

    for (int i = 0; i < rightCars; ++i) {
        rightCarIDs[i] = leftCars + i;
        int createRightThreadResult =  pthread_create(&rightThreads[i], NULL, right, &rightCarIDs[i]);
        if (createRightThreadResult != 0) {
            perror("Failed to create right thread");
            exit(1);
        }
    }

    for (int i = 0; i < leftCars; ++i) {
        int joinLeftThreadResult = pthread_join(leftThreads[i], NULL);
        if (joinLeftThreadResult != 0) {
            perror("Failed to join left thread");
            exit(1);
        }
    }

    for (int i = 0; i < rightCars; ++i) {
        int joinRightThreadResult = pthread_join(rightThreads[i], NULL);
        if (joinRightThreadResult != 0) {
            perror("Failed to join right thread");
            exit(1);
        }
    }

    if (sem_destroy(&leftSem) != 0) {
        perror("Error destroying leftSem");
    }

    if (sem_destroy(&rightSem) != 0) {
        perror("Error destroying rightSem");
    }

    if (sem_destroy(&mutex) != 0) {
        perror("Error destroying mutex");
    }

    return 0;
}
