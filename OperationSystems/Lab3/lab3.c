#include "unistd.h"
#include "time.h"
#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "string.h"


                     // Mutex используется, чтобы защитить код от выполнения другими потоками одновременно

int prime(int digit) {
    int i, j;
    int number = 1000;
    int primes[number + 1];
    //populating array with naturals numbers
    for (i = 2; i <= number; i++)
        primes[i] = i;

    i = 2;
    while ((i * i) <= number) {
        if (primes[i] != 0) {
            for (j = 2; j < number; j++) {
                if (primes[i] * j > number)
                    break;
                else
                    // Instead of deleteing , making elemnets 0
                    primes[primes[i] * j] = 0;
            }
        }
        i++;
    }
    for (int k = 0; k < 1001; k++) {
        if (primes[k] == digit) {
            return 1;
        }
    }
    return 2;
}



void *routine(void *arg) {
    int number = *(int *) arg;
    int t = prime(number);
    if (t == 1){
        printf("The number %d is prime\n", number);
    }
    else{
        printf("The number %d is not prime\n", number);
    }

    free(arg);
    return NULL;
}

int main(int argc, char* argv[]){

    int n = atoi(argv[1]);
    pthread_t pid[n];

    int data[n];
    printf("Enter %d numbers!\n", n);
    for(int i = 0; i < n; i++){
        int a;
        scanf("%d", &a);
        data[i] = a;
    }

    double start = clock();;

    if (argc == 2) {
        for (int i = 0; i < n; i++) {
            int * a = malloc(sizeof(int));
            *a = data[i];
            if (pthread_create(&pid[i], NULL, &routine, a) != 0) {
                perror("Couldn't create a thread\n");
                return 1;
            }
            printf("Thread %d has started\n", i);
        }
        for(int i = 0; i < n; i++){
            if (pthread_join(pid[i], NULL) != 0){           // Если pthread_join() оставить в предыдущем for, то не будет
                return 2;                                   // Одновременности выполнения, т.к. поток создатся и программа
            }                                               // Будет ждать завершения работы потока для создания нового
            printf("Thread has finished execution!\n");
        }
    }
    else{
        printf("Please enter an appropriate program key !\n");
    }
    printf("Count of threads: %d\n", n);
    printf("Программа работала %.4lf секунд\n", (clock() - start) / (CLOCKS_PER_SEC));
    return 0;
}
