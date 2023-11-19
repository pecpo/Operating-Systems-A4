#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int max_capacity=0;
int passengers=0;
int current_passengers=0;
sem_t cars,mutex,all_loaded,drive_car;

void* car(void* args){
    while(passengers>0){
        load();
        while(current_passengers<max_capacity){
            wait();
        }
        drive();
        unload();
        sem_post(&all_loaded);
        sem_wait(&mutex);
        passengers-=max_capacity;
        if(passengers<max_capacity){
            max_capacity=passengers;
        }
        sem_post(&mutex);
        printf("%d\n",passengers);
    }
}

void* passenger(void* args){
    printf("Passenger %d is waiting for the car.\n",*(int*)args);
    sem_wait(&cars);
    int value=0;
    sem_getvalue(&all_loaded,&value);
    while(value==1){
        wait();
        sem_getvalue(&all_loaded,&value);
    }
    while(current_passengers==max_capacity){
        wait();
    }
    board(*(int*)args);
    sem_getvalue(&drive_car,&value);
    while(value==0){
        wait();
        sem_getvalue(&drive_car,&value);
    }
    unboard(*(int*)args);
}

void wait(){
    return;
}

void load(int args){
    printf("The car is loading.\n");
    sem_wait(&mutex);
    for(int i=0;i<max_capacity;i++){
        sem_post(&cars);
    }
    sem_wait(&all_loaded);
    sem_post(&mutex);
}

void unload(){
    printf("The car is unloading.\n");
    sem_post(&drive_car);
}

void board(int args){
    sem_wait(&mutex);
    printf("Passenger %d has boarded the car.\n",args);
    current_passengers++;
    sem_wait(&drive_car);
    sem_post(&mutex);
}

void unboard(int args){
    sem_wait(&mutex);
    printf("Passenger %d has unboarded the car.\n",args);
    current_passengers--;
    sem_post(&mutex);
}

void drive(){
    printf("Car is driving.\n");
    sem_wait(&drive_car);
    return;
}

int main(){
    printf("Enter total no of passengers: ");
    scanf("%d",&passengers);
    printf("Enter max capacity of car: ");
    scanf("%d",&max_capacity);
    sem_init(&cars,0,max_capacity);
    sem_init(&mutex,0,1);
    sem_init(&all_loaded,0,1);
    sem_init(&drive_car,0,1);
    pthread_t threads[passengers];
    int pass_ids[passengers];
    for(int i=0;i<passengers;i++){
        pass_ids[i]=i+1;
        pthread_create(&threads[i],NULL,passenger,&pass_ids[i]);
    }
    pthread_t car1;
    char name[]="cars";
    pthread_create(&car1,NULL,car,&name);
    for(int i=0;i<passengers;i++){
        pthread_join(threads[i],NULL);
    }
    pthread_join(car1,NULL);
    sem_destroy(&cars);
    sem_destroy(&mutex);
    sem_destroy(&all_loaded);
    sem_destroy(&drive_car);
}