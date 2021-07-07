/*
Manuel Castro-Mirafuentes 016418712
Anshul Chauhan 016246735
CECS 326
Assignment 1
*/

#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <math.h>

// int ShareVariable is set to 0
int SharedVariable = 0;

// Create the proper mutex for the thread
pthread_mutex_t mutex;
// Create the proper barrier for the thread
pthread_barrier_t barrier;

// void pointer SimpleThread function
void *SimpleThread(void *which) {
	// We dereference the pointer, which holds the thread number
	int *w;
	w = which;
	// int num and val is created
	int num, val;
	
	// Is activated when we call it in the command line as -DSTHREAT
	#ifdef PTHREAD_SYNC
	/* Put your synchronization-related code here */
	// Mutex shall be lock
	pthread_mutex_lock(&mutex);
	#endif
	// For loop if num is less than 20
	// If num is less than 20 then the given threat will loop 20 times
	// The value will be added and will be shared in the global variable unless we add the command mentioned before which will wait
	for(num = 0; num < 20; num++) {
		// Check if random(), (which is a nonlinear additive feedback random number generator), is greater than RAND_MAX, (which is granted to be at least 32767), divided by 2
		if (random() > RAND_MAX / 2)
			// If random() is greater than RAND_MAX divided by 2, then usleep() function will suspend excution of the calling thread for 500 microsecond intervals
			usleep(500);
		
		// Set val to SharedVariable
		val = SharedVariable;
		// Print statement
		printf("*** thread %d sees values %d\n", *w, val);
		// Set SharedVariable by adding val and 1
		SharedVariable = val + 1;
	}
	// Set val to SharedVariable
	val = SharedVariable;
	#ifdef PTHREAD_SYNC
	/* Put your synchronization-related code here */
	// Mutex become avaiable
	pthread_mutex_unlock(&mutex);
	// Barrier where all thread will read the shared variable value and display the final value
	pthread_barrier_wait(&barrier);
	#endif
	// Print statement
	// NOTE: val in step 1 was change to SharedVariable
	printf("Thread %d sees final value %d\n", *w, SharedVariable);
	// Return NULL
	return NULL;
}

// main function
int main(int argc, char** argv) {
	// int i is created
	int i;
	// Create a prthread_t to pointer threads
	pthread_t *threads;
	
	// Check if argc is not equal to 2
	if (argc != 2) {
		// If argv is not equal to 2, then it print statement
		printf("Invalid number of arguments\n");
		// Then print another statement
		printf("Usage a.out <n>\n");
		// Return
		return 0;
	}
	// If argc is equal to 2, then it proceed to in the else statement
	else {
		// int valid is set to 1
		int valid = 1;
		// i is set to 0
		i = 0;
		// While loop if i is less than string length of argv[1]
		while(i < strlen(argv[1])) {
			// Check if argv[1][i] is less than 0 OR if argv[1][i] is greater than 9
			if (argv[1][i] < '0' || argv[1][i] > '9') {
				// If argv[1][i] is less than 0 OR if argv[1][i] is greater than 9, then valid is set to 0
				valid = 0;
				// Then break
				break;
			}
			// i = i + 1
			i++;
		}
		// Check if valid is equal to 0
		if (valid == 0) {
			// If valid is equal to 0, then it print statement
			printf("\nPlease provide a positive integer as an argument\n");
		}
		// If valid is not equal to 0, then it proceed to in the else statement
		else {
			// We assigned int num_threads to atoi(argv[1])
			int num_threads = atoi(argv[1]);
			// Create the number of thread
			threads = (pthread_t *)malloc(sizeof(pthread_t) * atoi(argv[1]));
			
			// Create the proper attribute for the thread
			pthread_attr_t attr;
			// Initialized attr variable created
			pthread_attr_init(&attr);
			
			// Initialized mutex variable created
			pthread_mutex_init(&mutex, NULL);
			// Initialized barrier variable created
			pthread_barrier_init(&barrier, NULL, num_threads);
			
			// int pointer of the size memory of the thread is created
			// Since the function pthread_create takes an argument of a number which is the number of threats as a poniter
			int *t = (int *)malloc(sizeof(int) * num_threads);
			
			// i is set to 0
			i = 0;
			// While loop if i is less than num_threads
			while(i < num_threads) {
				// Give threads id
				t[i] = i;
				// i = i + 1
				i++;
			}
			
			// Create each thread
			// i is set to 0
			i = 0;
			// While loop each thread if i is less than num_threads
			while(i < num_threads) {
				// First argument is the thread
				// Second argument is the attributes of the thread
				// NOTE: NULL from step 1 was change to &attr
				// Third argument is the function
				// Fourth argument is the number of threads
				pthread_create(&threads[i], &attr, SimpleThread, &t[i]);
				// i = i + 1
				i++;
			}
			
			// Join each thread
			// i is set to 0
			i = 0;
			// While loop each thread if i is less than num_threads
			while(i < num_threads) {
				// First argument is the thread
				// Second argument is the attributes of the thread
				pthread_join(threads[i], NULL);
				// i = i + 1
				i++;
			}
			
			// pthread_mutex_destroy() function destroy the variables created for mutex
			pthread_mutex_destroy(&mutex);
			// pthread_barrier_destroy() function destroy the variables created for barrier
			pthread_barrier_destroy(&barrier);
		}
	}
	// Exit
	exit(0);
}
