#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "computepi.h"

#define CLOCK_ID CLOCK_MONOTONIC_RAW
#define ONE_SEC 1000000000.0


double compute_error(double Pi);
double compute_ci(double data[]);

int loop = 25;

int main(int argc, char const *argv[])
{
    struct timespec start = {0, 0};
    struct timespec end = {0, 0};

    if (argc < 2) return -1;

    int N = atoi(argv[1]);
    int i ;
    double compute_time[loop];
    // Baseline
    for(i = 0; i < loop; i++) {
        clock_gettime(CLOCK_ID, &start);
        compute_pi_baseline(N);
        clock_gettime(CLOCK_ID, &end);
        compute_time[i] = (double) (end.tv_sec - start.tv_sec) + 
        (end.tv_nsec - start.tv_nsec)/ONE_SEC;
    }
    printf("%lf,",compute_ci(compute_time));
    printf("%.16lf,", compute_error(compute_pi_baseline(N)));


    // OpenMP with 2 threads
    for(i = 0; i < loop; i++) {
        clock_gettime(CLOCK_ID, &start);
        compute_pi_openmp(N, 2);
        clock_gettime(CLOCK_ID, &end);
        compute_time[i] = (double) (end.tv_sec - start.tv_sec) +
        (end.tv_nsec - start.tv_nsec)/ONE_SEC;

    }
    printf("%lf,",compute_ci(compute_time));
    printf("%.16lf,", compute_error(compute_pi_openmp(N, 2)));

    // OpenMP with 4 threads
    for(i = 0; i < loop; i++) {
        clock_gettime(CLOCK_ID, &start);
        compute_pi_openmp(N, 4);
        clock_gettime(CLOCK_ID, &end);
        compute_time[i] = (double) (end.tv_sec - start.tv_sec) +
        (end.tv_nsec - start.tv_nsec)/ONE_SEC;
    }
    printf("%lf,",compute_ci(compute_time));
    printf("%.16lf,", compute_error(compute_pi_openmp(N, 4)));


    // AVX SIMD
    for(i = 0; i < loop; i++) {
        clock_gettime(CLOCK_ID, &start);
        compute_pi_avx(N);
        clock_gettime(CLOCK_ID, &end);
        compute_time[i] = (double) (end.tv_sec - start.tv_sec) +
        (end.tv_nsec - start.tv_nsec)/ONE_SEC;
    }
    printf("%lf,",compute_ci(compute_time));
    printf("%.16lf,", compute_error(compute_pi_avx(N)));

    // AVX SIMD + Loop unrolling
    for(i = 0; i < loop; i++) {
        clock_gettime(CLOCK_ID, &start);
        compute_pi_avx_unroll(N);
        clock_gettime(CLOCK_ID, &end);
        compute_time[i] = (double) (end.tv_sec - start.tv_sec) +
        (end.tv_nsec - start.tv_nsec)/ONE_SEC;
    } 
    printf("%lf,",compute_ci(compute_time));
    printf("%.16lf\n", compute_error(compute_pi_avx_unroll(N)));
    return 0;
}




double compute_error(double Pi){
    Pi=fabs(Pi-M_PI)/M_PI;
    return Pi;
}


double compute_ci(double data[]) {
    double mean = 0;
    double SD = 0; // Standard Deviation
    double SDerror = 0; 
    double low,top;
    double result = 0;
    int count = 0;
  
    //compute mean
    for(int i = 0; i <loop; i++) {
        mean += data[i];
    }
    mean = mean / loop;

    //compute Standard Deviation
    for(int i = 0; i < loop; i++) {
		SD += pow((data[i] - mean),2);
    }
    //Calculate standard error
    SDerror = sqrt(SD / (double)loop);

    //range
    low = mean - 2 * SDerror;
    top = mean + 2 * SDerror;

    // in  95%
    for(int i = 0; i < loop; i++) {
		if(data[i] >= low && data[i] <= top) {
	    	count++;
                result+= data[i];
		}
    }


    result = result / count;

    return result;

}
