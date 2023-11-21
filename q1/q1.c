#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define PHILOSOPHERS 5

pthread_mutex_t forks[PHILOSOPHERS];
pthread_mutex_t mutex;
pthread_cond_t bowls_available[2];
int bowls[2];
int count=0;

void thinking(int args);
void eating(int args);
void wait();

void pickupforks(int args){
    int left_fork=args;
    int right_fork=(args+1)%PHILOSOPHERS;
    if(args==3){
        pthread_mutex_lock(&forks[left_fork]);
        printf("Fork %d acquired by %d\n",left_fork,args);
        pthread_mutex_lock(&forks[right_fork]);
        printf("Fork %d acquired by %d\n",right_fork,args);
    }
    else{
        pthread_mutex_lock(&forks[right_fork]);
        printf("Fork %d acquired by %d\n",right_fork,args);
        pthread_mutex_lock(&forks[left_fork]);
        printf("Fork %d acquired by %d\n",left_fork,args);
    }
}

void putdownforks(int args){
    int left_fork=args;
    int right_fork=(args+1)%PHILOSOPHERS;
    if(args==3){
        pthread_mutex_unlock(&forks[left_fork]);
        printf("Fork %d unacquired by %d\n",left_fork,args);
        pthread_mutex_unlock(&forks[right_fork]);
        printf("Fork %d unacquired by %d\n",right_fork,args);
    }
    else{
        pthread_mutex_unlock(&forks[right_fork]);
        printf("Fork %d unacquired by %d\n",right_fork,args);
        pthread_mutex_unlock(&forks[left_fork]);
        printf("Fork %d unacquired by %d\n",left_fork,args);
    }
}

void eating(int args){
    while(count<PHILOSOPHERS){
        wait();
    }
    usleep(100000);
    int left_fork=args;
    int right_fork=(args+1)%PHILOSOPHERS;
    pickupforks(args);
    int bowl_id=-1;
    pthread_mutex_lock(&mutex);
    while(bowl_id==-1){
        for(int i=0;i<2;i++){
            if(bowls[i]==1){
                bowls[i]=0;
                bowl_id=i;
                break;
            }
        }
        if(bowl_id==-1){
            pthread_mutex_unlock(&forks[left_fork]);
            pthread_mutex_unlock(&forks[right_fork]);
            pthread_cond_wait(&bowls_available[0],&mutex);
        }
    }
    pthread_mutex_unlock(&mutex);
    printf("Philosopher %d is eating.\n",args);
    putdownforks(args);
    pthread_mutex_lock(&mutex);
    bowls[bowl_id]=1;
    pthread_cond_signal(&bowls[bowl_id]);
    pthread_mutex_unlock(&mutex);
    return;
}

void thinking(int args){
    printf("Philosopher %d is thinking.\n",args);
    return;
}

void* philosopher(void* args){
    int id=*(int*)args;
    while(1){
        thinking(id);
        pthread_mutex_lock(&mutex);
        count++;
        pthread_mutex_unlock(&mutex);
        eating(id);
    }
}

void wait(){
    return;
}

int main(){
    bowls[0]=1;
    bowls[1]=1;
    for(int i=0;i<PHILOSOPHERS;i++){
        if (pthread_mutex_init(&forks[i], NULL) != 0) {
            perror("pthread_mutex_init");
            exit(1);
        }
    }
    for(int i=0;i<2;i++){
        if (pthread_cond_init(&bowls_available[i],NULL)!=0){
            perror("pthread_cond_init");
            exit(1);
        };
    }
    pthread_mutex_init(&mutex,NULL);
    int philosopher_ids[PHILOSOPHERS];
    pthread_t threads[PHILOSOPHERS];
    for(int i=0;i<PHILOSOPHERS;i++){
        philosopher_ids[i]=i;
        int creation=pthread_create(&threads[i],NULL,philosopher,&philosopher_ids[i]);
        if (creation != 0) {
            perror("pthread_create");
            exit(1);
        }
    }
    for(int i=0;i<PHILOSOPHERS;i++){
        int joining=pthread_join(threads[i],NULL);
        if (joining != 0) {
            perror("pthread_join");
            exit(1);
        }
    }
    if (pthread_cond_destroy(&bowls_available[0]) != 0) {
        perror("pthread_cond_destroy");
        exit(1);
    }
    if (pthread_cond_destroy(&bowls_available[1]) != 0) {
        perror("pthread_cond_destroy");
        exit(1);
    }
    if(pthread_mutex_destroy(&mutex)!=0){
        perror("pthread_mutex_destroy");
        exit(1);
    }
    if (pthread_mutex_destroy(&forks[0]) != 0) {
        perror("pthread_mutex_destroy");
        exit(1);
    }
    if (pthread_mutex_destroy(&forks[1]) != 0) {
        perror("pthread_mutex_destroy");
        exit(1);
    }
    if (pthread_mutex_destroy(&forks[2]) != 0) {
        perror("pthread_mutex_destroy");
        exit(1);
    }
    if (pthread_mutex_destroy(&forks[3]) != 0) {
        perror("pthread_mutex_destroy");
        exit(1);
    }
    if (pthread_mutex_destroy(&forks[4]) != 0) {
        perror("pthread_mutex_destroy");
        exit(1);
    }
    return 0;
}