#include "stdio.h"
#include "x86intrin.h"
#include <iostream> 


using namespace std;

typedef union {
	__m128i 			int128;
	
	unsigned char		m128_u8[16];
	signed char			m128_i8[16];

	unsigned short		m128_u16[8];
	signed short		m128_i16[8];

	unsigned int		m128_u32[4];
	signed int		m128_i32[4];

	unsigned long		m128_u64[2];
	signed long		m128_i64[2];

} intVec;


typedef union {
	__m128				float128;
	
	float		m128_u32[4];
} floatVec;

void print_spfp_vector (__m128 a)
{
	floatVec tmp;
	tmp.float128 = a;
	printf ("[");
	for (int i=3; i>0; i--) {
		printf ("%+f, ", (float) tmp.m128_u32[i]);
	}
	printf ("%+f]\n\n", (float) tmp.m128_u32[0]);
	
}

void print_u16 (__m128i a)
{
	intVec tmp;
	tmp.int128 = a;
	printf ("[");
	for (int i=7; i>0; i--) {
		printf ("%X, ", tmp.m128_u16[i]);
	}
	printf ("%X]\n\n", tmp.m128_u16[0]);
}

void print_int_vector (__m128i var128, unsigned char type){

	intVec temp;
	temp.int128 = var128;
	switch (type)
	{
	case 1:
		
		printf ("[");
		for (int i=15; i>0; i--) {
			printf ("%X, ", temp.m128_u8[i]);
		}
		printf ("%X]\n\n", temp.m128_u8[0]);
		break;

	case 2:
		
		printf ("[");
		for (int i=15; i>0; i--) {
			//printf("%s%X, ", temp.m128_i8[i] < 0 ? "-" : "", temp.m128_i8[i] < 0 ? -(unsigned)temp.m128_i8[i] : temp.m128_i8[i]);
			printf("%i, ", temp.m128_i8[i]);
		}
		//printf("%s%X]\n\n", temp.m128_i8[0] < 0 ? "-" : "", temp.m128_i8[0] < 0 ? -(unsigned)temp.m128_i8[0] : temp.m128_i8[0]);
		printf("%i]\n\n", temp.m128_i8[0]);

		break;

	case 3:
		
		printf ("[");
		for (int i=7; i>0; i--) {
			printf ("%X, ", temp.m128_u16[i]);
		}
		printf ("%X]\n\n", temp.m128_u16[0]);
		break;

	case 4:
		
		printf ("[");
		for (int i=7; i>0; i--) {
			printf ("%i, ", temp.m128_i16[i]);
			//printf("%s%X, ", temp.m128_i16[i] < 0 ? "-" : "", temp.m128_i16[i] < 0 ? -(unsigned)temp.m128_i16[i] : temp.m128_i16[i]);
		}
		printf ("%i]\n\n", temp.m128_i16[0]);
		//printf("%s%X]\n\n", temp.m128_i16[0] < 0 ? "-" : "", temp.m128_i16[0] < 0 ? -(unsigned)temp.m128_i16[0] : temp.m128_i16[0]);
		break;

	case 5:
		
		printf ("[");
		for (int i=3; i>0; i--) {
			printf ("%X, ", temp.m128_u32[i]);
		}
		printf ("%X]\n\n", temp.m128_u32[0]);
		break;

	case 6:
		
		printf ("[");
		for (int i=3; i>0; i--) {
			printf ("%i, ", temp.m128_i32[i]);
			//printf("%s%X, ", temp.m128_i32[i] < 0 ? "-" : "", temp.m128_i32[i] < 0 ? -(unsigned)temp.m128_i32[i] : temp.m128_i32[i]);
		}
		printf ("%i]\n\n", temp.m128_i32[0]);
		//printf("%s%X]\n\n", temp.m128_i32[0] < 0 ? "-" : "", temp.m128_i32[0] < 0 ? -(unsigned)temp.m128_i32[0] : temp.m128_i32[0]);
		break;

	case 7:
		
		printf ("[");
		for (int i=1; i>0; i--) {
			printf ("%lX, ", temp.m128_u64[i]);
		}
		printf ("%lX]\n\n", temp.m128_u64[0]);
		break;

	case 8:
		
		printf ("[");
		for (int i=1; i>0; i--) {
			//printf("%s%lX, ", temp.m128_i64[i] < 0 ? "-" : "", temp.m128_i64[i] < 0 ? -(unsigned)temp.m128_i64[i] : temp.m128_i64[i]);
			printf ("%li, ", temp.m128_i64[i]);
		}
		//printf("%s%lX]\n\n", temp.m128_i64[0] < 0 ? "-" : "", temp.m128_i64[0] < 0 ? -(unsigned)temp.m128_i64[0] : temp.m128_i64[0]);
		printf ("%li]\n\n", temp.m128_i64[0]);
		break;

	default:
		break;
	}
	
}


unsigned char intArray [16] = {	0X01, 0X12, 0X23, 0X34, 0X45, 0X56, 0X67, 0X78,
								0X89, 0X9A, 0XAB, 0XBC, 0XCD, 0XDE, 0XEF, 0XF0};

float a[4] = { 1.0, 2.0, 3.5, -4.25 };


int main(void)
{
	__m128i var128;
	var128 = _mm_load_si128((const __m128i*)intArray);
	//printf("%lu\n", sizeof(char));

	printf("%s\n", "u8");
	print_int_vector(var128 , 1);
	printf("%s\n", "i8");
	print_int_vector(var128 , 2);
	printf("%s\n", "u16");
	print_int_vector(var128 , 3);
	printf("%s\n", "i16");
	print_int_vector(var128 , 4);
	printf("%s\n", "u32");
	print_int_vector(var128 , 5);
	printf("%s\n", "i32");
	print_int_vector(var128 , 6);
	printf("%s\n", "u64");
	print_int_vector(var128 , 7);
	printf("%s\n", "i64");
	print_int_vector(var128 , 8);
	
	
	
	__m128 floatVar = _mm_load_ps(a);
	printf("%s\n", "float-32");
	print_spfp_vector(floatVar);

	return 0;
}
