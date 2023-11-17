#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define MAX_CARS 1000
#define MAX_BRIDGE_CARS 5

sem_t bridgeSem;
sem_t leftSem, rightSem;
sem_t mutex; // Semaphore for implementing mutex lock

int leftCars, rightCars;

void* left(void* args) {
    
}

void* right(void* args) {
    
}

void passing(int dir) {

}

int main() {
    pthread_t leftThreads[MAX_CARS], rightThreads[MAX_CARS];
    int leftCarIDs[MAX_CARS], rightCarIDs[MAX_CARS];
    // Initialization and semaphore creation
    sem_init(&bridgeSem, 0, MAX_BRIDGE_CARS);
    sem_init(&leftSem, 0, 1);
    sem_init(&rightSem, 0, 1);
    sem_init(&mutex, 0, 1); // Initializing the mutex semaphore

    // Take input for the number of cars on the left and right
    printf("Enter number of cars on the left side: ");
    scanf("%d", &leftCars);
    printf("Enter number of cars on the right side: ");
    scanf("%d", &rightCars);

    for (int i = 0; i < leftCars; ++i) {
        leftCarIDs[i] = i;
        pthread_create(&leftThreads[i], NULL, left, &leftCarIDs[i]);
    }


    for (int i = 0; i < rightCars; ++i) {
        rightCarIDs[i] = i;
        pthread_create(&rightThreads[i], NULL, right, &rightCarIDs[i]);
    }
    for (int i = 0; i < leftCars; ++i) {
        pthread_join(leftThreads[i], NULL);
    }


    for (int i = 0; i < rightCars; ++i) {
        pthread_join(rightThreads[i], NULL);
    }

    // Clean up and semaphore destruction
    sem_destroy(&bridgeSem);
    sem_destroy(&leftSem);
    sem_destroy(&rightSem);
    sem_destroy(&mutex); // Destroying the mutex semaphore

    return 0;
}
