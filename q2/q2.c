#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
int max_capacity=0;
int passengers=0;
int current_passengers=0;

sem_t car,mutex;

void* car(void* args){
    int max=max_capacity;
    if(current_passengers<=max_capacity){
        sem_wait(&car);
        sem_wait(&mutex);
        load();
        current_passengers++;
        sleep(10);
        sem_wait(&mutex);
        sem_post(&car);
    }
    else if{
        sem_wait(&mutex);
        for(int i=0;i<current_passengers;i++){
            unload();
        }
        sem_post(&mutex);
    }
    else{
        sem_wait(&mutex);
        current_passengers=0;
        sem_post(&mutex);
    }
}

void* passenger(void* args){

}

void load(){

}

void unload(){

}

void board(){

}

void offboard(){

}

int main(){
    printf("Enter total no of passengers: ");
    scanf("%d",&passengers);
    printf("Enter max capacity of car: ");
    scanf("%d",&max_capacity);
    sem_init(&car,0,max_capacity);
    sem_init(&mutex,0,1);
    pthread_t threads[passengers];
    for(int i=0;i<passengers;i++){
        int pass_id=i+1;
        pthread_create(&threads[i],NULL,passenger,(void*)pass_id);
    }
    pthread_t car;
    char* name=(char*)malloc(3);
    name="car";
    pthread_create(&car,NULL,car,(void*)name);
}