#include 	"stdio.h"
#include 	"stdlib.h"
#include 	"time.h"
#include 	"omp.h"
#include 	"x86intrin.h"
#include <sys/time.h>

#define SIZE 1048576



void serial_max(float A[], int i, int j){
    float max_value = -1.0;
    int max_index;
    for (int idx=i; idx < j; idx++){
        if(A[idx] > max_value){
            max_value = A[idx];
            max_index = idx;
        }
    }

    printf("%f\n", max_value);
    printf("%d\n", max_index);


}


void omp_parallel_max(float A[], int first, int end)
{
    int i;
    float max_val = -10000000000000.0; 
    int indexes[6];
    float maxs[6] = {-10000000000000.0,-10000000000000.0,-10000000000000.0,-10000000000000.0,-10000000000000.0,-10000000000000.0};
    int max_index;

    #pragma omp parallel num_threads(6) default(shared) 
    {
        int threadNum = omp_get_thread_num();
        #pragma omp for private(i) nowait 
            for (i = first; i < end; i++){
                if(A[i] > maxs[threadNum]){
                    maxs[threadNum] = A[i];
                    indexes[threadNum] = i;
                }

            }

    }

    for(int i = 0; i < 6; i++){
        if(maxs[i] > max_val){
            max_val = maxs[i];
            max_index = indexes[i];
        }
    }


    
    printf("%f\n", max_val);
    printf("%d\n", max_index);
}

int main (void)
{


	printf("810196573-810196623\n\n");

    struct timeval start,end,start1,end1;

    float a[SIZE];
    srand((unsigned int)time(NULL));
    float max_possible = 100.0;
    for (int i=0;i<SIZE;i++){
        a[i] = (float(rand())/float((RAND_MAX)) * max_possible);
    }

    gettimeofday(&start,NULL);
	omp_parallel_max(a,0,SIZE);
    gettimeofday(&end,NULL);


    long seconds = (end.tv_sec - start.tv_sec);
    long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
    printf("OMP PARALLEL Execution time is %ld s and %ld micros\n\n", seconds, micros);

    gettimeofday(&start1,NULL);
    serial_max(a,0,SIZE);
    gettimeofday(&end1,NULL);

    long seconds1 = (end1.tv_sec - start1.tv_sec);
    long micros1 = ((seconds1 * 1000000) + end1.tv_usec) - (start1.tv_usec);
    printf("SERIAL Execution time is %ld s and %ld micros\n\n", seconds1, micros1);

    printf("improvement : %f\n", (float) (seconds1 * 1000 + micros1) / (float) (seconds * 1000 + micros));
    
	return 0;
}
