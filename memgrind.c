#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include "mymalloc.h"

double workload1_times[50];
double workload2_times[50];
double workload3_times[50];
double workload4_times[50];
double workload5_times[50];

struct timeval* timeval_subtract (struct timeval* result,struct timeval* x, struct timeval* y){
    if (x->tv_usec < y->tv_usec) {
        result->tv_usec += 1000000;
        x->tv_sec--;
    }
    result->tv_sec = x->tv_sec - y->tv_sec;
    result->tv_usec = x->tv_usec - y->tv_usec;
    return result; 
}


double workload1(){
    struct timeval result, x, y;
    gettimeofday(&x,NULL);
    int i;
    for(i = 0; i<120;i++){
	char* temp = (char*)malloc(1); 	
        free(temp);
    }
    gettimeofday(&y, NULL);
    timeval_subtract(&result,&y,&x);
    return (double)(result.tv_sec+((double)result.tv_usec/1000000.0));

}

double workload2(){
    struct timeval result, x, y;
    gettimeofday(&x,NULL);
    int i;
    char* arr[120];
    for(i = 0; i<120;i++){
	arr[i] = (char*)malloc(1);   
    }
    for(i = 0; i<120;i++){   
        free(arr[i]);
    }
    gettimeofday(&y, NULL);
    timeval_subtract(&result,&y,&x);
    return (double)(result.tv_sec+((double)result.tv_usec/1000000.0));
}

double workload3(){
    struct timeval result, x, y;
    gettimeofday(&x,NULL);
    int i, j=0,total_mallocs =0,total_frees = 0;
    char* arr[120];
    srand(time(0));
    for(i = 0; i < 240;i++){
        int num = (rand() %(2));
	if(((j == 0 || num ==0) && total_mallocs <120)|| total_frees == 120){
            arr[j] = (char*)malloc(1);
	    total_mallocs++;
	    j++;
	}else{
            j--;
            free(arr[j]);
	    total_frees++;
	}
    }
    gettimeofday(&y, NULL);
    timeval_subtract(&result,&y,&x);
    return (double)(result.tv_sec+((double)result.tv_usec/1000000.0));


}

double workload4(){
    struct timeval result, x, y;
    gettimeofday(&x,NULL);
    int i;
    char* arr[87];
    for(i = 1; i < 86;i++){ 
            arr[i] = (char*)malloc(i); //i starts at 1 because allocating blocks of 0 bytes or less is an error.
    }
    for(i = 1; i < 86;i++){
	    free(arr[i]);
    }
    
    gettimeofday(&y, NULL);
    timeval_subtract(&result,&y,&x);
    return (double)(result.tv_sec+((double)result.tv_usec/1000000.0));


}


double workload5(){
    struct timeval result, x, y;
    gettimeofday(&x,NULL);
    int i;
    char* arr[120];
    for(i = 0; i < 120;i++){
            arr[i] = (char*)malloc(30);
	    if(i%2 == 0){
                free(arr[i]);
            }
    }
    for(i = 0; i < 60;i++){
	arr[i*2] = (char*)malloc(27);
    }
    for(i = 0; i < 120;i++){
	free(arr[i]);
    }
    gettimeofday(&y, NULL);
    timeval_subtract(&result,&y,&x);
    return (double)(result.tv_sec+((double)result.tv_usec/1000000.0));


}



int main() {
    int i;
    double workload1_sum = 0, workload2_sum = 0, workload3_sum = 0, workload4_sum=0, workload5_sum=0;
    for(i = 0; i < 50; i++){
        workload1_times[i] = workload1();
	workload1_sum += workload1_times[i];
	workload2_times[i] = workload2();
	workload2_sum += workload2_times[i];
	workload3_times[i] = workload3();
	workload3_sum += workload3_times[i];
	workload4_times[i] = workload4();
	workload4_sum += workload4_times[i];
	workload5_times[i] = workload5();
	workload5_sum += workload5_times[i];
    }
    printf("Workload1 average time: %lf\n", workload1_sum/50.0); 
    printf("Workload2 average time: %lf\n", workload2_sum/50.0);
    printf("Workload3 average time: %lf\n", workload3_sum/50.0);
    printf("Workload4 average time: %lf\n", workload4_sum/50.0);
    printf("Workload5 average time: %lf\n", workload5_sum/50.0);
    return 0;
}
