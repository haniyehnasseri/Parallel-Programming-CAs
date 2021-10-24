#include "pthread.h"
#include "time.h"
#include <sys/time.h>
#include "stdio.h"
#include "stdlib.h"
#include <iostream>

#define NUM_THREADS 8
#define DATA_SIZE 1048576
using namespace std;

typedef struct{ 
    int tid;
    int start_index;
    int end_index; 
} in_param_t;

//int max_indices_parallel[NUM_THREADS];
//float max_values_parallel[NUM_THREADS];
pthread_mutex_t lock_mutex;

float max_value_parallel = -2000000000.0;
int max_index_parallel = -1;

float arr[DATA_SIZE];
int max_index_serial;
float max_value_serial =  -2000000000.0;


void * find_local_max(void *arg) {
    in_param_t *inp = (in_param_t *) arg; 
    float local_max = -2000000000.0;
    int local_index = -1;
    for (int i=inp->start_index; i<inp->end_index; i++)
        if(arr[i]>local_max){
            local_max = arr[i];
            local_index = i;
        } 

    pthread_mutex_lock (&lock_mutex);
    if(local_max>max_value_parallel){
        max_value_parallel = local_max;
        max_index_parallel = local_index;
    }   
    pthread_mutex_unlock (&lock_mutex);   
    pthread_exit (NULL);
}

void serial_max(float arr[], int i, int j){
    for (int idx=i; idx < j; idx++)
        if(arr[idx] > max_value_serial){
            max_value_serial = arr[idx];
            max_index_serial = idx;
        }
}



int main(void) {
    pthread_t th[NUM_THREADS]; 
    in_param_t in_param [NUM_THREADS] ;

    struct timeval start_s,end_s,start_p,end_p;
    pthread_mutex_init (&lock_mutex, NULL);

    printf("810196623 - 810196573\n");
 

    for(int i=0;i<NUM_THREADS;i++){
        in_param[i].tid = i ;
        in_param[i].start_index = i*DATA_SIZE/NUM_THREADS;
        in_param[i].end_index = (i+1)*DATA_SIZE/NUM_THREADS;
    }

    
    srand((unsigned int)time(NULL));
    float max_possible = 100.0;
    for (int i=0;i<DATA_SIZE;i++){
        arr[i] = (float(rand())/float((RAND_MAX)) * max_possible);
    }



    gettimeofday(&start_p,NULL);
    for (int i = 0; i < NUM_THREADS; i++) 
        pthread_create (&th[i], NULL, find_local_max,(void *) &in_param[i]);

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join (th[i], NULL); 


    gettimeofday(&end_p,NULL);

    printf("parallel\n");
    printf("max value: %f , max index: %d\n",max_value_parallel,max_index_parallel);

    gettimeofday(&start_s,NULL);
    serial_max(arr,0,DATA_SIZE);
    gettimeofday(&end_s,NULL);

    printf("serial\n");
    printf("max value: %f , max index: %d\n",max_value_serial,max_index_serial);

    long seconds_p = (end_p.tv_sec - start_p.tv_sec);
    long micros_p = ((seconds_p * 1000000) + end_p.tv_usec) - (start_p.tv_usec);
    printf("PARALLEL Execution time is %ld s and %ld micros\n", seconds_p, micros_p);

    long seconds_s = (end_s.tv_sec - start_s.tv_sec);
    long micros_s = ((seconds_s * 1000000) + end_s.tv_usec) - (start_s.tv_usec);
    printf("SERIAL Execution time is %ld s and %ld micros\n", seconds_s, micros_s);

    printf("improvement : %f\n", (float) (seconds_s *1000 + micros_s) / (float) (seconds_p* 1000 + micros_p));

    pthread_mutex_destroy (&lock_mutex);
    pthread_exit (0);

}