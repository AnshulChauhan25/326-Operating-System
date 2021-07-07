/*
Manuel Castro-Mirafuentes 016418712
Anshul Chauhan 016246735
CECS 326
Assignment 2

This project asks to write a multithreaded sorting program that works as follows: A list of integers
is divided into two smaller lists of equal size. Two separate threads (which we will term sorting threads)
sort each sublist using a sorting algorithm of your choice. The two sublists are then merged by a third thread—a merging
thread—which merges the two sublists into a single sorted list.
*/

#include <stdio.h>	//  header file to use all standard input/output function
#include <math.h>	// header to declares a set of functions to compute common mathematical operations and transformations
#include <stdlib.h>	//  header of the general purpose standard library of C
#include <pthread.h>	//  header file to use all the functions of the pthreads library.
#include <stdint.h>	// header to declare sets of integer types having specified widths/define macros that specify limits of integer types

#define ARRAY_SIZE 200   // array size 
#define THREAD_COUNT 2   // a count of the number of threads

//easiest way to set up the data is to create a
//global array. Each sorting thread will work on 
//one half of this array
int data[ARRAY_SIZE];

//A second global array of the same size as the
//unsorted integer array will also be established
int sortedData[ARRAY_SIZE];

// merge thread function accept pointer of array of 2 elements
// first element is low index second element is high index
void* sort(void* arg);  /* thread that performs basic sorting algorithm */

// merge thread function accepts pointer to array of sorting threads
void* merge(void* arg);   /* thread that performs merging of results */

// start the main program
int main(int argc, char **argv)
{
	int i;
	int *lowHigh1=(int *) malloc(sizeof(int)*2);  // dynamically to allocate memory
	int lowHigh2[2];
	pthread_t sortThreads[THREAD_COUNT];
	pthread_t mergeThread;
	lowHigh1[0] = 0;
	lowHigh1[1] = ARRAY_SIZE / 2-1;
	lowHigh2[0] = lowHigh1[1]+1;
	lowHigh2[1] = ARRAY_SIZE - 1;
	
	printf("Array before sorting \n");
	for (i = 0; i < ARRAY_SIZE; i++)
	{
		data[i] = rand() % ARRAY_SIZE;
		printf("%d ", data[i]);
	}

	pthread_create(&sortThreads[0], NULL, sort, (void *)lowHigh1);      //  the first sorting thread
	pthread_create(&sortThreads[1], NULL, sort, (void *)lowHigh2);      //  the second sorting thread
	pthread_create(&mergeThread, NULL, merge, (void *)(sortThreads));   // the merge thread
	pthread_join(mergeThread, NULL);  // wait for the merge thread to finish

	printf("\n\nArray after sorting \n");
	for (i = 0; i < ARRAY_SIZE; i++)
	{
		printf("%d ", sortedData[i]);
	}
	printf("\n");
	return 0;
}

// used quicksort which picks an element as pivot (meidan here) and partitions the given array around the picked pivot. The key process in quickSort is partition(). Target of partitions is, given an array and an element x of array as pivot, put x at its correct position in sorted array and put all smaller elements (smaller than x) before x, and put all greater elements (greater than x) after x.
void quickSort(int lowerIndex, int higherIndex) {
	int temp;
	int i = lowerIndex;
	int j = higherIndex;
	int pivot = data[lowerIndex + (higherIndex - lowerIndex) / 2];  // median as pivot

	while (i < j) {
		while (data[i] < pivot) {   // element less than pivot
			i++;
		}
		while (data[j] > pivot) {  // element greater than pivot
			j--;
		}
		if (i <= j) {
			temp = data[i];
			data[i] = data[j];
			data[j] = temp;
			i++;
			j--;
		}
	}
	if (lowerIndex < j)
		quickSort(lowerIndex, j);
	if (i < higherIndex)
		quickSort(i, higherIndex);
}

// start to sort the array by calling quicSort function
void* sort(void* arg)
{
	int *lowHigh = ((int *)arg);
	int low = lowHigh[0];
	int high = lowHigh[1];
	quickSort(low, high);
}

// after sort both side of the array, we have to megre them with merge funcion:
void* merge(void* arg)
{
	int i, index1, index2;
	pthread_t *sortThreads;
	sortThreads = (pthread_t *)arg;
	for (i = 0; i < THREAD_COUNT; i++) //wait sort threads
	{
		pthread_join(sortThreads[i], NULL);
	}
	index1 = 0;
	index2 = ARRAY_SIZE / 2;
	for (i = 0; i < ARRAY_SIZE; i++)
	{
		if (index1 >= ARRAY_SIZE / 2)
		{
			sortedData[i] = data[index2++];
		}
		else if (index2 >= ARRAY_SIZE)
		{
			sortedData[i] = data[index1++];
		}
		else if (data[index1] < data[index2])
		{
			sortedData[i] = data[index1++];
		}
		else
		{
			sortedData[i] = data[index2++];
		}
	}
}
