#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t left_sem, right_sem, bridge_sem;
pthread_t left_thread, right_thread;

void passing(int dir) {
    if (dir == 0) {
        printf("Car from left side is passing the bridge with thread: %ld\n", pthread_self());
    } else {
        printf("Car from right side is passing the bridge with thread: %ld\n", pthread_self());
    }
}

void* left(void* args) {
    int num_cars = *((int*) args);
    for (int i = 0; i < num_cars; i++) {
        sem_wait(&left_sem);
        sem_wait(&bridge_sem);
        passing(0);
        sleep(1);
        sem_post(&bridge_sem);
        sem_post(&right_sem);
    }
    pthread_exit(NULL);
}

void* right(void* args) {
    int num_cars = *((int*) args);
    for (int i = 0; i < num_cars; i++) {
        sem_wait(&right_sem);
        sem_wait(&bridge_sem);
        passing(1);
        sleep(1);
        sem_post(&bridge_sem);
        sem_post(&left_sem);
    }
    pthread_exit(NULL);
}

int main() {
    sem_init(&left_sem, 0, 5);
    sem_init(&right_sem, 0, 5);
    sem_init(&bridge_sem, 0, 5);

    int num_left_cars, num_right_cars;
    printf("Enter the number of cars on the left side: ");
    scanf("%d", &num_left_cars);
    printf("Enter the number of cars on the right side: ");
    scanf("%d", &num_right_cars);

    pthread_t left_thread, right_thread;
    pthread_create(&left_thread, NULL, left, &num_left_cars);
    pthread_create(&right_thread, NULL, right, &num_right_cars);

    pthread_join(left_thread, NULL);
    pthread_join(right_thread, NULL);

    sem_destroy(&left_sem);
    sem_destroy(&right_sem);
    sem_destroy(&bridge_sem);

    return 0;
}
