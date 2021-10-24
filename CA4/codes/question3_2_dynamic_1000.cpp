#include <stdio.h>
#include <math.h>
#include <sys/time.h> 
#include <sys/resource.h>  
#include <omp.h>

double timeGetTime() 
{     
	struct timeval time;     
	struct timezone zone;     
	gettimeofday(&time, &zone);     
	return time.tv_sec + time.tv_usec*1e-6; 
}  


const long int VERYBIG = 100000;

int main( void )
{
  int i;
  long int j, k, sum;
  double sumx, sumy, total, z;
  double starttime, elapsedtime;
  double thread_time[4];
  double thread_start[4];
  double mean_execution_time = 0.0;
  // ---------------------------------------------------------------------
  // Output a start message
  printf( "OpenMP Parallel Timings for %ld iterations \n\n", VERYBIG );
  printf("810196623 - 810196573\n\n");

  // repeat experiment several times
  for( i=0; i<6; i++ )
  {
    // get starting time
    starttime = timeGetTime();
    // reset check sum and total
    sum = 0;
    total = 0.0;
    
    // Work loop, do some work by looping VERYBIG times
    #pragma omp parallel  num_threads (4) 
    {  
      thread_start[omp_get_thread_num()] = timeGetTime();
      #pragma omp for private( sumx, sumy, k ) reduction( +: sum, total ) schedule(dynamic,1000) nowait
      for( int j=0; j<VERYBIG; j++ )
      {
        // increment check sum
        sum += 1;
       
        // Calculate first arithmetic series
        sumx = 0.0;
        for( k=0; k<j; k++ )
         sumx = sumx + (double)k;

        // Calculate second arithmetic series
        sumy = 0.0;
        for( k=j; k>0; k-- )
         sumy = sumy + (double)k;

        if( sumx > 0.0 )total = total + 1.0 / sqrt( sumx );
        if( sumy > 0.0 )total = total + 1.0 / sqrt( sumy );
      }
      thread_time[omp_get_thread_num()] = timeGetTime()-thread_start[omp_get_thread_num()];

    }
    // get ending time and use it to determine elapsed time
    elapsedtime = timeGetTime() - starttime;
    mean_execution_time += elapsedtime;
  
    // report elapsed time
    printf("Time Elapsed %10d mSecs Total=%lf Check Sum = %ld\n",
                   (int)(elapsedtime * 1000), total, sum );
    for(int c=0;c<4;c++)
      printf ("T%d excution time: %10d mSec\n", c,(int)(thread_time[c]*1000));
  }

  mean_execution_time = mean_execution_time/6;
  printf("mean execution time: %10d mSecs\n",(int)(mean_execution_time*1000));

  // return integer as required by function header
  return 0;
}
// **********************************************************************
