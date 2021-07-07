#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <stdlib.h>

#define NUMTHREADS 4 // not sure if this is command line argument.

int SharedVariable = 0;
pthread_mutex_t lock;
pthread_barrier_t barr;

void threadFunc(void arg) {

int threadId = *((int*)arg);
int i;

// 20 is the number given in example
for(i = 0; i< 20; i++) {
// macro for compilation
#ifdef PTHREAD_SYNC
pthread_mutex_lock(&lock);
#endif
// incrementing shared variable
printf("thread %d sees value %d\n", threadId, SharedVariable);
SharedVariable++;

#ifdef PTHREAD_SYNC
pthread_mutex_unlock(&lock);
#endif

}

// barrier so that all thread will read the shared variable value and display the final value.
pthread_barrier_wait(&barr);
printf("Thread %d sees final value %d\n", threadId,SharedVariable);
return NULL;

}

int main() {

int i,err;
// NUMTHREADS is 4 as in the example.
// The question mentions nothing about the command line argument.
// if it is number of threads or number of iterations.
pthread_t threads[NUMTHREADS];
int threadIds[NUMTHREADS]={0};

// initialixing mutex.
pthread_mutex_init(&lock, NULL);
// initialixing barrier.
pthread_barrier_init(&barr, NULL, NUMTHREADS);

for(i = 0; i < NUMTHREADS; i++) {

// giving thread ids.
threadIds[i]=i;

// creating threads
err = pthread_create(&threads[i], NULL, threadFunc, (void*)&threadIds[i]);
if(err){
printf("ERROR creating THREADS, return code %d\n",err);
exit(-1);
}
}


// waiting for all the threads to finish.
for(i = 0; i < NUMTHREADS; i++) {
pthread_join(threads[i], NULL);
}


return 0;
}