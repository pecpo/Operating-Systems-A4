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
    
}

void* passenger(void* args){
    if(current_passengers<max_capacity){
        sem_wait(&mutex);
        load(*(int*)args);
        // current_passengers++;
        // printf("%d\n",current_passengers);
        sleep(1);
        sem_post(&mutex);
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

void load(int args){
    printf("Loading passenger %d\n",args);
    return;
}

void unload(int a){
    printf("Unloading passenger %d\n",a);
    return;
}

void onboard(){
    printf("Onboarded");
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
        sem_wait(&cars);
        pass_ids[i]=i+1;
        pthread_create(&threads[i],NULL,passenger,&pass_ids[i]);
    }
    pthread_t car1;
    char name[]="cars";
    pthread_create(&car1,NULL,car,&name);
    for(int i=0;i<passengers;i++){
        sem_post(&cars);
        pthread_join(threads[i],NULL);
    }
    pthread_join(car1,NULL);
    sem_destroy(&cars);
    sem_destroy(&mutex);
}