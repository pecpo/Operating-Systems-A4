#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int max_capacity=0;
int passengers=0;
int current_passengers=0;
int count=0;
sem_t cars,mutex,all_loaded,drive_car,loading,unloading;

void load();
void drive();
void unload();
void board(int args);
void unboard(int args);
void wait();

void* car(void* args){
    while(1){
        load();
        sem_wait(&drive_car);
        sem_wait(&all_loaded);
        drive();
        sem_post(&drive_car);
        unload();
        sem_post(&all_loaded);
        usleep(100000);
    }
    printf("All passengers have been served.\n");
    pthread_exit(NULL);
}

void* passenger(void* args){
    while(1){
    printf("Passenger %d is waiting for the car.\n",*(int*)args);
    count++;
    sem_wait(&cars);
    sem_wait(&loading);
    int value=0;
    board(*(int*)args);
    sem_post(&loading);
    sem_getvalue(&all_loaded,&value);
    while(value>0){
        wait();
        sem_getvalue(&all_loaded,&value);
    }
    sem_getvalue(&drive_car,&value);
    while(value==0){
        wait();
        sem_getvalue(&drive_car,&value);
    }
    sem_wait(&unloading);
    unboard(*(int*)args);
    sem_post(&unloading);
    }
    pthread_exit(NULL);
}

void wait(){
    return;
}

void load(){
    while(count<passengers){
        wait();
    }
    printf("The car is loading.\n");
    sem_post(&loading);
    while(current_passengers!=max_capacity){
        wait();
    }
    sem_wait(&loading);
    return;
}

void unload(){
    printf("The car is unloading.\n");
    sem_post(&unloading);
    int value=0;
    while (current_passengers!=0){
        wait();
    }
    sem_wait(&mutex);
    for(int i=0; i<max_capacity; i++){
        sem_post(&cars);
    }
    sem_post(&mutex);
    sem_wait(&unloading);
    return;
}

void board(int args){
    sem_wait(&mutex);
    printf("Boarding passenger %d\n",args);
    current_passengers++;
    sem_post(&mutex);
    return;
}

void unboard(int args){
    sem_wait(&mutex);
    printf("Unboarding passenger %d\n",args);
    current_passengers--;
    sem_post(&mutex);
    return;
}

void drive(){
    printf("Car is driving.\n");
    usleep(100000);
    return;
}

int main(){
    printf("Enter total no of passengers: ");
    scanf("%d",&passengers);
    printf("Enter max capacity of car: ");
    scanf("%d",&max_capacity);
    if(max_capacity<=0){
        printf("Invalid max capacity.\n");
        exit(0);
    }
    if(passengers<=0){
        printf("Invalid no of passengers.\n");
        exit(0);
    }
    if(passengers<max_capacity){
        printf("Max capacity cannot be greater than no of passengers.\n");
        exit(0);
    }
    if (sem_init(&loading, 0, 0) == -1) {
        perror("sem_init");
        exit(1);
    }
    if (sem_init(&unloading, 0, 0) == -1) {
        perror("sem_init");
        exit(1);
    }
    if (sem_init(&drive_car, 0, 1) == -1) {
        perror("sem_init");
        exit(1);
    }
    if (sem_init(&all_loaded, 0, 1) == -1) {
        perror("sem_init");
        exit(1);
    }
    if(sem_init(&cars,0,max_capacity)==-1){
        perror("sem_init");
        exit(1);
    }
    if(sem_init(&mutex,0,1)==-1){
        perror("sem_init");
        exit(1);
    }
    pthread_t threads[passengers];
    int pass_ids[passengers];
    for(int i=0;i<passengers;i++){
        pass_ids[i]=i+1;
        int creation_passengers=pthread_create(&threads[i],NULL,passenger,&pass_ids[i]);
        if (creation_passengers != 0) {
            perror("Failed to create thread");
            exit(1);
        }
    }
    pthread_t car1;
    char name[]="cars";
    int creation_car=pthread_create(&car1,NULL,car,&name);
    if (creation_car != 0) {
        perror("Failed to create thread");
        exit(1);
    }
    for(int i=0;i<passengers;i++){
        int joining_passengers=pthread_join(threads[i],NULL);
        if(joining_passengers!=0){
            perror("Failed to join thread");
            exit(1);
        }
    }
    int joining_car=pthread_join(car1,NULL);
    if(joining_car!=0){
        perror("Failed to join thread");
        exit(1);
    }
    if (sem_destroy(&loading) == -1) {
        perror("sem_destroy");
        exit(1);
    }
    if (sem_destroy(&unloading) == -1) {
        perror("sem_destroy");
        exit(1);
    }
    if (sem_destroy(&drive_car) == -1) {
        perror("sem_destroy");
        exit(1);
    }
    if (sem_destroy(&all_loaded) == -1) {
        perror("sem_destroy");
        exit(1);
    }
    if (sem_destroy(&cars)==-1){
        perror("sem_destroy");
        exit(1);
    }
    if (sem_destroy(&mutex)==-1){
        perror("sem_destroy");
        exit(1);
    }
}