#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
int max_capacity=0;
int passengers=0;
int current_passengers=0;

sem_t cars,mutex;

void* car(void* args){
    int max=max_capacity;
    if(current_passengers<max_capacity){
        sem_wait(&cars);
        sem_wait(&mutex);
        load((int)args);
        current_passengers++;
        sleep(10);
        sem_wait(&mutex);
        sem_post(&cars);
    }
    else if(current_passengers==max_capacity){
        sem_wait(&mutex);
        for(int i=0;i<current_passengers;i++){
            unload(i);
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

void load(int args){
    printf("Loading passenger %d",args);
}

void unload(int a){
    printf("Unloading passenger %d",a);
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
    sem_init(&cars,0,max_capacity);
    sem_init(&mutex,0,1);
    pthread_t threads[passengers];
    int pass_ids[passengers];
    for(int i=0;i<passengers;i++){
        pass_ids[i]=i;
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
}