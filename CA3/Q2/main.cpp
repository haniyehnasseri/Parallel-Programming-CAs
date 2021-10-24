#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include "x86intrin.h"
#include <sys/time.h>

using namespace cv;
using namespace std;

int main( )
{

    struct timeval start,end,start1,end1;

	// LOAD images
	cv::Mat img_1 = cv::imread("CA03__Q2__Image__01.png", IMREAD_GRAYSCALE);
    cv::Mat img_2 = cv::imread("CA03__Q2__Image__02.png", IMREAD_GRAYSCALE);

    printf("810196623 - 810196573\n");

	unsigned int NCOLS = img_1.cols;
	unsigned int NROWS = img_1.rows;

    unsigned int img2_rows = img_2.rows;
    unsigned int img2_cols = img_2.cols;

	// Create output images
	cv::Mat out_img_serial  (img_1.rows, img_1.cols, CV_8U) ;
	cv::Mat out_img_parallel (img_1.rows, img_1.cols, CV_8U) ;



	// Pointer to the image data and output image (Matrix of pixels)
	unsigned char *in_image_1 = (unsigned char *) img_1.data; 
    unsigned char *in_image_2 = (unsigned char *) img_2.data; 
	unsigned char *p = (unsigned char *) out_img_serial.data;

    
    gettimeofday(&start,NULL);
	for (int row = 0; row < NROWS; row++){
		for (int col = 0; col < NCOLS; col++){
            unsigned short temp;
            if(row < img2_rows && col < img2_cols){
                temp = (unsigned short) ((unsigned short)(*(in_image_1 + row * NCOLS + col)) + (unsigned short)(*(in_image_2 + (row * img2_cols + col)) >> 1));
                if(temp > 255)
                    temp = 255;
                if(temp < 0)
                    temp = 0;
                *(p + row * NCOLS + col)  = (unsigned char) temp;    
            }
            else{
                *(p + row * NCOLS + col) = *(in_image_1 + row * NCOLS + col);
            }

            
        }
    }

            
    gettimeofday(&end,NULL);

    long seconds = (end.tv_sec - start.tv_sec);
    long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);

    printf("Execution time is %ld s and %ld micros\n", seconds, micros);


    __m128i *in_image_1_p = (__m128i  *) img_1.data; 
    __m128i *in_image_2_p = (__m128i  *) img_2.data; 
	__m128i *p2 = (__m128i *) out_img_parallel.data;


    NCOLS = img_1.cols / 16;
    
    img2_cols = img_2.cols / 16;


    __m128i m1,m2,mask,res;


    __m128i temp = _mm_set1_epi8((unsigned char) 128);
    gettimeofday(&start1,NULL);
    for(int row = 0; row < NROWS; row++){
        for(int col = 0; col < NCOLS; col++){
            m1 = _mm_loadu_si128(in_image_1_p + row * NCOLS + col);
            if(row < img2_rows && col < img2_cols){
                m2 = _mm_loadu_si128(in_image_2_p +(row * img2_cols + col));
                m2 = m2 >> 1;
                mask = _mm_cmpeq_epi8( m2, _mm_max_epu8(m2,temp));
                m2 = _mm_blendv_epi8(m2, _mm_subs_epu8(m2,temp), mask);  
                res = _mm_adds_epu8(m1,m2);
                _mm_storeu_si128(p2 + row * NCOLS + col, res);
            }
            else{
                _mm_storeu_si128(p2 + row * NCOLS + col, m1);
            }

        }
    }

    gettimeofday(&end1,NULL);

    long seconds1 = (end1.tv_sec - start1.tv_sec);
    long micros1 = ((seconds1 * 1000000) + end1.tv_usec) - (start1.tv_usec);

	printf("Execution time is %ld s and %ld micros\n", seconds1, micros1);

	printf("improvement : %f\n", (float) (seconds + micros) / (float) (seconds1 + micros1));

	cv::namedWindow( "image1", cv::WINDOW_AUTOSIZE ); 	
	cv::imshow( "image1", img_1 ); 
    cv::namedWindow( "logo", cv::WINDOW_AUTOSIZE ); 	
	cv::imshow( "logo", img_2 );
	cv::namedWindow( "serial output", cv::WINDOW_AUTOSIZE ); 	
	cv::imshow( "serial output", out_img_serial ); 
    cv::namedWindow( "parallel output", cv::WINDOW_AUTOSIZE ); 	
	cv::imshow( "parallel output", out_img_parallel ); 

						
	waitKey(0);                       					
    return 0;
}
