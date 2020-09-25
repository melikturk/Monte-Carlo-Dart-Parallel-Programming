#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

double calculate_pi(long long int num_toss){
    long long int num_in_circle = 0;
    int toss;
    srand(time(NULL));
    int threadNumber = omp_get_num_threads();
# pragma omp parallel for num_threads(threadNumber) reduction(+:num_in_circle) schedule(guided)
    for(toss=0; toss<num_toss; toss++){
        double x = rand() / (double)RAND_MAX;
        double y = rand() / (double)RAND_MAX;
        double distance = x*x + y*y;
        if(distance <= 1)
            num_in_circle++;
    }
    return 4*num_in_circle/(double)num_toss;
}

int main (int argc, char *argv[]){
    if (argc != 3){
        printf("-> Must pass 2 arguments:  <# Cores> <# tosses>");
        exit(-1);
    }

    int num_core = atoi(argv[1]);
    long long int num_toss = atoll(argv[2]);
    double pi;

    printf("-> Number of Cores: %d\n", num_core);
    printf("-> Number of Toss: %lli\n", num_toss);

    sem_t wait_circle;
    pthread_t tid[15];


    //Time Start...//
	clock_t start, end;
	double cpu_time_used;

    // Running... //
	start = clock();
    pi = calculate_pi(num_toss);

    //Time End...//
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("-> pi: %.10lf\n", pi);
    printf("------------------------------------------------------\n");
    printf("Running time for first block is %f\n", cpu_time_used);

    return 0;
}
