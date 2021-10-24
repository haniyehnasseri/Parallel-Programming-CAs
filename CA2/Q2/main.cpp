#include 	"x86intrin.h"
#include <sys/time.h>
#include 	"stdio.h"
#include <math.h>
#define		VECTOR_SIZE		  	1048576

float* parallel_mean_std(float* fArray,size_t size){
    float fVRes;
    __m128 vec;
	__m128 sum = _mm_set1_ps(0.0f);
    float *parallel_ans = new float [2];
	for (long i = 0; i < size; i+=4) {
		vec = _mm_loadu_ps (&fArray[i]);
		sum = _mm_add_ps (sum, vec);
	}
	sum = _mm_hadd_ps (sum, sum);
	sum = _mm_hadd_ps (sum, sum);
	fVRes = _mm_cvtss_f32 (sum);
    parallel_ans[0] =  fVRes/size;
    
    __m128 mean= _mm_set1_ps(parallel_ans[0]);
    sum = _mm_set1_ps(0.0f);
    for (long i = 0; i < size; i+=4) {
		vec = _mm_loadu_ps (&fArray[i]);
        vec = _mm_sub_ps(vec,mean);
        vec = _mm_mul_ps(vec,vec);
		sum = _mm_add_ps (sum, vec);
	}

    sum = _mm_hadd_ps (sum, sum);
	sum = _mm_hadd_ps (sum, sum);
	fVRes = _mm_cvtss_f32 (sum);


    parallel_ans[1] = sqrt(fVRes/size);
    return parallel_ans;
}

float* serial_mean_std(float* fArray,size_t size){
    float fSTmpRes[4];
	float fSRes;
    float *serial_ans = new float [2];
    fSTmpRes[0] = fSTmpRes[1] = fSTmpRes[2] = fSTmpRes[3] = 0.0;
	for (long i = 0; i < size; i+=4)
		fSTmpRes[0] += fArray[i];
	for (long i = 0; i < size; i+=4)
		fSTmpRes[1] += fArray[i + 1];
	for (long i = 0; i < size; i+=4)
		fSTmpRes[2] += fArray[i + 2];
	for (long i = 0; i < size; i+=4)
		fSTmpRes[3] += fArray[i + 3];
	fSRes = fSTmpRes[0] + fSTmpRes[1] + fSTmpRes[2] + fSTmpRes[3];
    serial_ans[0] = fSRes/size;

    float mean = serial_ans[0];
    float temp;
    fSTmpRes[0] = fSTmpRes[1] = fSTmpRes[2] = fSTmpRes[3] = 0.0;
	for (long i = 0; i < size; i+=4){
		temp = (fArray[i]-mean);
		fSTmpRes[0] += temp*temp;
	}
	for (long i = 0; i < size; i+=4){
		temp = fArray[i + 1]-mean;
		fSTmpRes[1] += temp*temp;
	}
	for (long i = 0; i < size; i+=4){
		temp = fArray[i + 2]-mean;
		fSTmpRes[2] += temp*temp;
	}
	for (long i = 0; i < size; i+=4){
		temp = fArray[i + 3]-mean;
		fSTmpRes[3] += temp*temp;
	}
    fSRes = fSTmpRes[0] + fSTmpRes[1] + fSTmpRes[2] + fSTmpRes[3];
    serial_ans[1] = sqrt(fSRes/size);
    return serial_ans;
}

int main(void){

    float fSTmpRes[4];
	float fSRes;
	float fVRes;

	float *fArray;
	fArray = new float [VECTOR_SIZE];
    float *parallel_ans =  new float [2];
    float *serial_ans =  new float [2];

    if (!fArray) {
		printf ("Memory allocation error!!\n");
		return 1;
	}
	// Initialize vectors with random numbers
	for (long i = 0; i < VECTOR_SIZE; i++)
		fArray[i] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/100.0));

	printf("810196623 - 810196573\n");
    struct timeval start, end;
	gettimeofday(&start, NULL);
    parallel_ans = parallel_mean_std(fArray,VECTOR_SIZE);
    gettimeofday(&end, NULL);
    printf("parallel mean : %f\n",parallel_ans[0]);
    printf("parallel std : %f\n",parallel_ans[1]);
	long secondsP = (end.tv_sec - start.tv_sec);
	long microsP = ((secondsP * 1000000) + end.tv_usec) - (start.tv_usec);
    printf("Execution time is %ld s and %ld micros\n", secondsP, microsP);

    gettimeofday(&start, NULL);
    serial_ans = serial_mean_std(fArray,VECTOR_SIZE);
    gettimeofday(&end, NULL);
    printf("serial mean : %f\n",serial_ans[0]);
    printf("serial std : %f\n",serial_ans[1]);
	long secondsS = (end.tv_sec - start.tv_sec);
	long microsS = ((secondsS * 1000000) + end.tv_usec) - (start.tv_usec);
    printf("Execution time is %ld s and %ld micros\n", secondsS, microsS);

    printf("improvement : %f\n", (float) (secondsS + microsS) / (float) (secondsP + microsP));
}