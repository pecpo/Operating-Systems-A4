#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5
#define NUM_FORKS 5
#define NUM_BOWLS 2

pthread_mutex_t forks[NUM_FORKS];
pthread_mutex_t bowls[NUM_BOWLS];
pthread_cond_t bowl_available[NUM_BOWLS];

void pickup_forks(int left, int right) {
    pthread_mutex_lock(&forks[left]);
    pthread_mutex_lock(&forks[right]);
}

void putdown_forks(int left, int right) {
    pthread_mutex_unlock(&forks[left]);
    pthread_mutex_unlock(&forks[right]);
}

void pickup_bowl(int bowl) {
    pthread_mutex_lock(&bowls[bowl]);
}

void putdown_bowl(int bowl) {
    pthread_mutex_unlock(&bowls[bowl]);
}

void eating(int philosopher_id) {
    int left_fork = philosopher_id;
    int right_fork = (philosopher_id + 1) % NUM_FORKS;
    int bowl = philosopher_id % NUM_BOWLS;

    pickup_forks(left_fork, right_fork);
    pickup_bowl(bowl);

    printf("Philosopher %d is eating\n", philosopher_id);
    sleep(2); // Simulating eating time

    putdown_bowl(bowl);
    putdown_forks(left_fork, right_fork);
}

void thinking(int philosopher_id) {
    printf("Philosopher %d is thinking\n", philosopher_id);
    sleep(2); // Simulating thinking time
}

void* philosopher(void* args) {
    int philosopher_id = *((int*)args);

    while (1) {
        thinking(philosopher_id);
        eating(philosopher_id);
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
        pthread_mutex_init(&bowls[i], NULL);
        pthread_cond_init(&bowl_available[i], NULL);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        philosopher_ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &philosopher_ids[i]);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_join(philosophers[i], NULL);
    }

    for (int i = 0; i < NUM_FORKS; ++i) {
        pthread_mutex_destroy(&forks[i]);
    }

    for (int i = 0; i < NUM_BOWLS; ++i) {
        pthread_mutex_destroy(&bowls[i]);
        pthread_cond_destroy(&bowl_available[i]);
    }

    return 0;
}
