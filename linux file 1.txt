#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include<pthread.h>
#include<errno.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>


int SharedVariable = 0;
void *SimpleThread(void *i) {

    int *p;
    p = i;
    int num, val = 0;
    for(num = 0; num < 20; num++) {
       if (random() > RAND_MAX / 2)
          usleep(10);
       val = SharedVariable;
       //printf("------------------------\n");
       printf("*** thread %d sees value %d\n", *p, val);
       SharedVariable = val + 1;
    }
    val = SharedVariable;
    printf("Thread %d sees final value %d\n", *p, val);
}


int main(int argc, char *argv[])
{
    pthread_t *tid;
    int i;
    if (argc != 2){
       printf("Invalid number of arguments\n");
       printf("Usage a.out <n>\n");
       return 0;
    }
    else {
        int valid = 1;
        for (i = 0; i<strlen(argv[1]); i++){
            if (argv[1][i] < '0' || argv[1][i] > '9'){
                 valid = 0;
                 break;
            }
        }

        if (valid == 0){
           printf("\nPlease provide a positive integer as an argument\n");
        }
        else {
            int n = atoi(argv[1]);
            tid = (pthread_t *)malloc(sizeof(pthread_t) * atoi(argv[1]));
            int *j = (int *)malloc(sizeof(int)*n);
            for (i = 0; i<n; i++)
               j[i] = i;
           
            for (i = 0; i<n; i++){
                pthread_create(&tid[i], NULL, SimpleThread, &j[i]);
               
            }
            for (i = 0; i<n; i++){
               pthread_join(tid[i], NULL);
            }
        }
    }
    exit(0);
}