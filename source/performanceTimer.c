//
// Created by osboxes on 03/06/15.
//

#include "performanceTimer.h"



void printTime(struct timespec tstart,char* functionName)
{
//    struct timeb end_time;
//    ftime(&end_time);
//    int time = (int) ((end_time.time - start_time.time) * 1000) + (end_time.millitm - start_time.millitm);
//    printf("function %s took %i miliseconds to complete\n",functionName ,time);


    struct timespec tend={0,0};
    clock_gettime(CLOCK_MONOTONIC, &tend);

    double timeDif = (((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec)) * 1000;
    if(timeDif < minTime) return;
#ifdef functionFilter
    if(strcmp(functionName,functionFilter)) return;
#endif


    printf("it took   %.3f    miliseconds to complete function    %s \n", timeDif ,functionName);

    if(timeDif > longestTime)
    {
        longestTime = timeDif;
        slowestFunction = functionName;
    }
}

void finalTimer(struct timespec tstart)
{
    printf("the slowest function is %s with %.3f miliseconds\n",slowestFunction,longestTime);

    struct timespec tend={0,0};
    clock_gettime(CLOCK_MONOTONIC, &tend);

    double timeDif = (((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));

    printf("it took a total of %.6f seconds \n", timeDif);
}