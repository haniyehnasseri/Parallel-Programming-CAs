#include "stdio.h"
#include "x86intrin.h"
#include <iostream> 
#include <cstdlib>
#include <ctime>
#include <math.h> 
#include <sys/time.h>

#define SIZE 1048576
using namespace std;

//float sa [8] = {10.5,0.99,87.1,56.3,111.0,1166.6,98.0,1180.7};

float* max_parallel(float* array, size_t size) {

    float* response = new float[2];
    __m128i indexes  = _mm_set_epi32(3,2,1,0);
    __m128i maxindexes = _mm_set_epi32(3,2,1,0);
    
    __m128 maxvalues  = _mm_loadu_ps(&array[0]);

    for (int i=4; i < size; i += 4) {

        indexes = _mm_add_epi32(indexes, _mm_set1_epi32(4));
        __m128 values = _mm_loadu_ps(&array[i]);
        __m128 mask  = _mm_cmpgt_ps(values, maxvalues);
        maxvalues  = _mm_max_ps(values, maxvalues);
        //maxindices = _mm_blendv_epi8(maxindices, indices,(__m128i) gt);
        //maxindices = ((__m128i)gt && indices)  _mm_andnot_si128((__m128i) gt, maxindices);
        maxindexes = _mm_or_si128(_mm_and_si128(_mm_castps_si128(mask), indexes),
                           _mm_andnot_si128(_mm_castps_si128(mask), maxindexes));
        //
        //maxindices = _mm_blendv_epi8(maxindices, indices, _mm_castps_si128(mask));
        
        
    }

    
    float values[4];
    int indices[4];

    _mm_storeu_ps(values, maxvalues);
    _mm_storeu_si128((__m128i*)indices, maxindexes);

    int maxindex = indices[0];
    float maxvalue = values[0];
    for (int i=1; i < 4; i++) {
        if (values[i] > maxvalue) {
            maxvalue = values[i];
            maxindex = indices[i];
        } 
    }

    response [0] = maxindex;
    response [1] = maxvalue;

    return response;
}

float* serial_operation(float* a, size_t size){

    float max_index1 = 0.0;
    float max_index2 = 1.0;
    float max_index3 = 2.0;
    float max_index4 = 3.0;
    float max1 = a[0];
    float max2 = a[1];
    float max3 = a[2];
    float max4 = a[3];

    /*for(int i =4; i < size; i++){
        if(i%4 == 0){
            if(a[i] > max1){
                max_index1 = i;
                max1 = a[i];
            }
        }
        if(i%4 == 1){
            if(a[i] > max2){
                max_index2 = i;
                max2 = a[i];
            }   
        }
        if(i%4 == 2){
            if(a[i] > max3){
                max_index3 = i;
                max3 = a[i];
            }
        }
        if(i%4 == 3){
            if(a[i] > max4){
                max_index4 = i;
                max4 = a[i];
            }
        }
    }*/


    for (int i = 4; i < size; i += 4){
        if(a[i] > max1){
            max_index1 = i;
            max1 = a[i];
        }
    }
    for (int i = 5; i < size; i += 4){
        if(a[i] > max2){
            max_index2 = i;
            max2 = a[i];
        }
    }
    for (int i = 6; i < size; i += 4){
        if(a[i] > max3){
            max_index3 = i;
            max3 = a[i];
        }
    }
    for (int i = 7; i < size; i += 4){
        if(a[i] > max4){
            max_index4 = i;
            max4 = a[i];
        }
    }
    float max = max1;
    float max_ind = max_index1;
    if(max2 > max){
        max = max2;
        max_ind = max_index2;
    }
    if(max3 > max){
        max = max3;
        max_ind = max_index3;
    }
    if(max4 > max){
        max = max4;
        max_ind = max_index4;
    }

    float* response = new float[2];
    response[0] = max_ind;
    response[1] = max;
    return response;
}

int main(void)
{

    printf("810196573-810196623\n\n");
    //float sa [16] = {10.5,0.99,87.1,56.3,111.0,99999999.6,98.0,1180.0,7.0,0.11,0.9,90.74,99999.99,0.65789,110.67,41.76};
    float a[SIZE];
    srand((unsigned int)time(NULL));
    float max_possible = 100.0;
    for (int i=0;i<SIZE;i++){
        a[i] = (float(rand())/float((RAND_MAX)) * max_possible);
    }


    struct timeval start,end,start1,end1;
    
    gettimeofday(&start,NULL);
    float* response_parallel = max_parallel(a, SIZE);
    gettimeofday(&end,NULL);

    printf("parallel - index : %.0f\n", response_parallel[0]);
    printf("parallel - float number : %f\n",response_parallel[1]);
    long seconds = (end.tv_sec - start.tv_sec);
    long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);

    printf("Execution time is %ld s and %ld micros\n", seconds, micros);


    gettimeofday(&start1,NULL);
    float* response_serial = serial_operation(a, SIZE);
    gettimeofday(&end1,NULL);

    printf("serial - index : %.0f\n", response_serial[0]);
    printf("serial - float number : %f\n",response_serial[1]);
    long seconds1 = (end1.tv_sec - start1.tv_sec);
    long micros1 = ((seconds1 * 1000000) + end1.tv_usec) - (start1.tv_usec);

    printf("Execution time is %ld s and %ld micros\n", seconds1, micros1);

    printf("improvement : %f\n", (float) (seconds1 + micros1) / (float) (seconds + micros));

    return 0;
}
