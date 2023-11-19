#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5
#define NUM_FORKS 5
#define NUM_BOWLS 2

pthread_mutex_t forks[NUM_FORKS];
pthread_cond_t bowl_available[NUM_BOWLS];

void pickup_forks(int left, int right) {
    pthread_mutex_lock(&forks[left]);
    pthread_mutex_lock(&forks[right]);
}

void putdown_forks(int left, int right) {
    pthread_mutex_unlock(&forks[left]);
    pthread_mutex_unlock(&forks[right]);
}

void pickup_bowl() {
    for(int i=0;i<NUM_BOWLS;i++){
        if(pthread_cond_wait(&bowl_available[i], &forks[i]) == 0){
            pthread_cond_signal(&bowl_available[i]);
        }
    }
}

// void putdown_bowl(int bowl) {
//     pthread_mutex_unlock(&bowls[bowl]);
// }

void eating(int args) {
    int left_fork = args;
    int right_fork = (args + 1) % NUM_FORKS;
    pickup_forks(left_fork, right_fork);
    pickup_bowl();

    printf("Philosopher %d is eating\n", args);
    sleep(1); 

    // putdown_bowl(bowl);
    putdown_forks(left_fork, right_fork);
}

void thinking(int philosopher_id) {
    printf("Philosopher %d is thinking\n", philosopher_id);
    sleep(1);
}

void* philosopher(void* args) {
    while (1) {
        thinking(*(int*)args);
        eating(*(int*)args);
    }

    return NULL;
}

int main() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int philosopher_ids[NUM_PHILOSOPHERS];

    for (int i = 0; i < NUM_FORKS; ++i) {
        pthread_mutex_init(&forks[i], NULL);
    }

    for (int i = 0; i < NUM_BOWLS; ++i) {
        pthread_cond_init(&bowl_available[i], NULL);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        philosopher_ids[i] = i+1;
        pthread_create(&philosophers[i], NULL, philosopher, &philosopher_ids[i]);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_join(philosophers[i], NULL);
    }

    for (int i = 0; i < NUM_FORKS; ++i) {
        pthread_mutex_destroy(&forks[i]);
    }

    for (int i = 0; i < NUM_BOWLS; ++i) {
        pthread_cond_destroy(&bowl_available[i]);
    }

    return 0;
}
