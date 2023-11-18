#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
int max_capacity=0;
int passengers=0;
int current_passengers=0;

sem_t cars,mutex;

void* car(void* args){
    // if(max_capacity==current_passengers){
    //     for(int i=0;i<max_capacity;i++){
    //         unload(i+1);
    //     }
    // }
    // else{
    //     printf("Car is full");
    // }
}

void* passenger(void* args){
    sem_wait(&mutex);
    bool boarded=false;
    bool car_loaded=false;
    if(current_passengers<max_capacity){
        board(*(int*)args);
        current_passengers++;
        boarded=true;
        // printf("%d\n",current_passengers);   
    }
    else{
        printf("Car is full. Please wait for the car.\n");
    }
    sem_post(&mutex);
    if(current_passengers==max_capacity && boarded){
        unboard(*(int*)args);
    }
}

void load(int args){
    board(args);
    return;
}

void unload(int args){
    unboard(args);
    return;
}

void board(int args){
    printf("Loading passenger %d\n",args);
    return;
}

void unboard(int args){
    printf("Unloading passenger %d\n",args);
}

void drive(){
    printf("Car is driving.\n");
    return;
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
}