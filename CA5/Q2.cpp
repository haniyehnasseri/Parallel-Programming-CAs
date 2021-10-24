#include <stdio.h> 
#include "pthread.h"
#include "time.h"
#include <sys/time.h>
#include "stdio.h"
#include "stdlib.h"
#include <iostream>

#define NUM_THREADS 8
#define DATA_SIZE 1048576
using namespace std;
float* arr_p = new float[DATA_SIZE]();
float* arr_s = new float[DATA_SIZE]();
//type = 1 parallel else serial
typedef struct{ 
    int low;
    int high; 
    int type;
} in_param_t;

void swap(float* a, float* b) { 
    float t = *a; 
    *a = *b; 
    *b = t; 
} 

int partition(float arr[], int low, int high) { 
    float x = arr[high]; 
    int i = (low - 1); 
    float temp;
  
    for (int j = low; j <= high - 1; j++) { 
        if (arr[j] <= x) { 
            i++; 
            swap(&arr[i], &arr[j]); 
        } 
    } 
    swap(&arr[i + 1], &arr[high]); 
    return (i + 1); 
} 
  

void* quickSort(void* arg) { 
    in_param_t *inp = (in_param_t *) arg;
    
    int low = inp->low;
    int high = inp->high;
    int type = inp->type;
    
    if(high <= low){
        return NULL;
    }

    int *stack = new int[high - low + 1];
  
    int head = 0; 
    
    stack[head] = low; 
    head +=1;
    stack[head] = high; 
 
    while (head >= 0) { 

        high = stack[head]; 
        head -=1;
        low = stack[head]; 
        head -= 1;
        int partition_index;

        if(type == 1)
            partition_index = partition(arr_p, low, high); 
        else
            partition_index = partition(arr_s, low, high); 
        

        if (partition_index > low+1) { 
            head +=1 ;
            stack[head] = low; 
            head +=1;
            stack[head] = partition_index - 1; 
        } 
        if (partition_index  < high-1 ) { 
            head +=1 ;
            stack[head] = partition_index + 1; 
            head +=1 ;
            stack[head] = high; 
        } 
    } 

    return NULL;
} 


int main(){

    pthread_t th[NUM_THREADS]; 
    in_param_t in_param [NUM_THREADS+1] ;

    struct timeval start_s,end_s,start_p,end_p;

    printf("810196623 - 810196573\n");

  
    srand((unsigned int)time(NULL));
    float max_possible = 100.0;
    for (int i=0;i<DATA_SIZE;i++){
        arr_p[i] = (float(rand())/float((RAND_MAX)) * max_possible);
    }

    for (int i=0;i<DATA_SIZE;i++){
        arr_s[i] = arr_p[i];
    }

   /* printf("orginal array:\n");
    for (int i = 0; i < DATA_SIZE; ++i) 
        printf("%f ", arr_p[i]); 
    printf("\n");
    */

    gettimeofday(&start_p,NULL);
    int pivot[NUM_THREADS-1] = {-1,-1,-1,-1,-1,-1,-1};


    if(0 < DATA_SIZE-1)
		pivot[0] = partition(arr_p, 0, DATA_SIZE-1); 

	if(0 < pivot[0]-1)
		pivot[1] = partition(arr_p, 0, pivot[0] - 1);
	
	if(pivot[0]+1<DATA_SIZE-1 && pivot[0]!= -1) 
		pivot[2] = partition(arr_p, pivot[0] + 1, DATA_SIZE-1);

	if(0 < pivot[1]-1)
		pivot[3] = partition(arr_p, 0, pivot[1]-1); 

	if(pivot[1]+1<pivot[0]-1 && pivot[1]!= -1 && pivot[0] != -1)
		pivot[4] = partition(arr_p, pivot[1]+1, pivot[0]-1);

	if(pivot[0]+1<pivot[2]-1 && pivot[0]!=-1 && pivot[2]!=-1)   
		pivot[5] = partition(arr_p, pivot[0]+1, pivot[2]-1);  

	if(pivot[2]+1<DATA_SIZE-1 && pivot[2]!=-1)
		pivot[6] = partition(arr_p, pivot[2]+1, DATA_SIZE-1); 


    in_param[0].low = 0;
    in_param[0].high = pivot[3]-1;
    in_param[0].type = 1;

    in_param[1].low = pivot[3]+1;
    in_param[1].high = pivot[1]-1;
    in_param[1].type = 1;

    in_param[2].low = pivot[1]+1;
    in_param[2].high = pivot[4]-1;
    in_param[2].type = 1;

    in_param[3].low = pivot[4]+1;
    in_param[3].high = pivot[0]-1;
    in_param[3].type = 1;

    in_param[4].low = pivot[0]+1;
    in_param[4].high = pivot[5]-1;
    in_param[4].type = 1;

    in_param[5].low = pivot[5]+1;
    in_param[5].high = pivot[2]-1;
    in_param[5].type = 1;

    in_param[6].low = pivot[2]+1;
    in_param[6].high = pivot[6]-1;
    in_param[6].type = 1;

    in_param[7].low = pivot[6]+1;
    in_param[7].high = DATA_SIZE-1;
    in_param[7].type = 1;


    for (int i = 0; i < NUM_THREADS; i++) 
        pthread_create (&th[i], NULL, quickSort,(void *) &in_param[i]);

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join (th[i], NULL); 


    gettimeofday(&end_p,NULL);


    gettimeofday(&start_s,NULL);
    in_param[8].low = 0;
    in_param[8].high = DATA_SIZE-1;
    in_param[8].type = 0;
    quickSort((void *) &in_param[8]);
    gettimeofday(&end_s,NULL);


    long seconds_p = (end_p.tv_sec - start_p.tv_sec);
    long micros_p = ((seconds_p * 1000000) + end_p.tv_usec) - (start_p.tv_usec);
    printf("PARALLEL Execution time is %ld s and %ld micros\n", seconds_p, micros_p);

    long seconds_s = (end_s.tv_sec - start_s.tv_sec);
    long micros_s = ((seconds_s * 1000000) + end_s.tv_usec) - (start_s.tv_usec);
    printf("SERIAL Execution time is %ld s and %ld micros\n", seconds_s, micros_s);

    printf("improvement : %f\n", (float) (seconds_s *1000 + micros_s) / (float) (seconds_p* 1000 + micros_p));

    /*printf("serial sort:\n");
    for (int i = 0; i < DATA_SIZE; ++i) 
        printf("%f ", arr_s[i]); 

    printf("\n")

    printf("parallel sort:\n");
    for (int i = 0; i < DATA_SIZE; ++i) 
        printf("%f ", arr_p[i]); */

    

    pthread_exit (0);

}
  