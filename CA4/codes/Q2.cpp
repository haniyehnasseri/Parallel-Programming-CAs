#include 	"stdio.h"
#include 	"stdlib.h"
#include 	"time.h"
#include 	"omp.h"
#include 	"x86intrin.h"
#include <sys/time.h>
#include <bits/stdc++.h> 
using namespace std; 


#define SIZE 1048576





void swap(float* aa, float* bb) 
{ 
	float t = *aa; 
	*aa = *bb; 
	*bb = t; 
} 





int partition (float arr[], int low, int high) 
{ 
	float pivot = arr[high]; 
	int smaller = low - 1;

	for (int i = low; i < high ; i++) 
	{ 
		
		if (arr[i] < pivot) 
		{ 
			
			swap(&arr[smaller + 1], &arr[i]); 
			smaller++; 
		} 
	} 
	swap(&arr[smaller + 1], &arr[high]); 
	return smaller + 1; 
} 


void quickSort(float arr[], int low, int high) 
{ 
	if (low < high) 
	{ 

		int pivot = partition(arr, low, high); 

		quickSort(arr, low, pivot - 1); 
		quickSort(arr, pivot + 1, high); 
	} 
} 


void printArray(float arr[], int size) 
{ 

} 


void omp_parallel_sort(float arr[]){


	int pivot = -1;
	int pivot2 = -1;
	int pivot3 = -1;
	int pivot4 = -1;
	int pivot5 = -1;
	int pivot6 = -1;
	int pivot7 = -1;

	

	if(0 < SIZE-1)
		pivot = partition(arr, 0, SIZE-1); 

	if(0 < pivot-1 && pivot != -1)
		pivot2 = partition(arr, 0, pivot - 1);
	
	if(pivot+1<SIZE-1 && pivot!= -1) 
		pivot3 = partition(arr, pivot + 1, SIZE-1);

	if(0 < pivot2-1 && pivot2!=-1)
		pivot4 = partition(arr, 0, pivot2-1); 

	if(pivot2+1<pivot-1 && pivot2!= -1 && pivot != -1)
		pivot5 = partition(arr, pivot2+1, pivot-1);

	if(pivot+1<pivot3-1 && pivot!=-1 && pivot3!=-1)   
		pivot6 = partition(arr, pivot+1, pivot3-1);  

	if(pivot3+1<SIZE-1 && pivot3!=-1)
		pivot7 = partition(arr, pivot3+1, SIZE-1); 



    #pragma omp parallel sections num_threads(8)
	{
		#pragma omp section
		{
			if(pivot4 != -1)
            	quickSort(arr,0, pivot4 - 1);
    	}
		#pragma omp section
		{
			if((pivot4 != -1) && (pivot2 != -1))
        		quickSort(arr, pivot4 + 1, pivot2 - 1);
   		}

		#pragma omp section
		{
			if((pivot2 != -1) && (pivot5 != -1))
            	quickSort(arr,pivot2+1, pivot5 - 1);
    	}
		#pragma omp section
		{
			if((pivot5 != -1) && (pivot != -1))
        		quickSort(arr, pivot5 + 1, pivot-1);
   		}

		#pragma omp section
		{
			if((pivot != -1) && (pivot6 != -1))
            	quickSort(arr,pivot+1, pivot6 - 1);
    	}
		#pragma omp section
		{
			if((pivot6 != -1) && (pivot3 != -1))
        		quickSort(arr, pivot6 + 1, pivot3 - 1);
   		}

		#pragma omp section
		{
			if((pivot3 != -1) && (pivot7 != -1))
            	quickSort(arr,pivot3+1, pivot7 - 1);
    	}
		#pragma omp section
		{
			if(pivot7 != -1)
        		quickSort(arr, pivot7 + 1, SIZE-1);
   		}
	}




	/*printf("OMP PARALLEL Sorted array: \n");
	int i; 
	for (i = 0; i < SIZE; i++) 
		printf("%f ", arr[i]);
	printf("\n");*/

}


void serial_sort(float arr[]){


	quickSort(arr, 0, SIZE - 1);


	/*printf("Serial Sorted array: \n");
	int i; 
	for (i = 0; i < SIZE; i++) 
		printf("%f ", arr[i]);
	printf("\n");*/

}


int main() 
{ 

	struct timeval start,end,start1,end1;
	omp_set_nested(1);
	
	float* a = new float[SIZE]();
	float* b = new float[SIZE]();
    srand((unsigned int)time(NULL));
    float max_possible = 10.0;
    for (int i=0;i<SIZE;i++){
        a[i] = (float(rand())/float((RAND_MAX)) * max_possible);
    }

	for(int i = 0; i < SIZE; i++){
		b[i] = a[i];
	}




	gettimeofday(&start,NULL);
	omp_parallel_sort(a);
	gettimeofday(&end,NULL);


    long seconds = (end.tv_sec - start.tv_sec);
    long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
    printf("OMP PARALLEL Execution time is %ld s and %ld micros\n\n", seconds, micros);


	gettimeofday(&start1,NULL);
	serial_sort(b);
	gettimeofday(&end1,NULL);


	long seconds1 = (end1.tv_sec - start1.tv_sec);
    long micros1 = ((seconds1 * 1000000) + end1.tv_usec) - (start1.tv_usec);
    printf("SERIAL Execution time is %ld s and %ld micros\n\n", seconds1, micros1);

    printf("improvement : %f\n", (float) (seconds1 * 1000 + micros1) / (float) (seconds * 1000 + micros));
 

    

	return 0; 
} 

