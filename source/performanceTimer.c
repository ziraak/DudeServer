//
// Created by osboxes on 03/06/15.
//

#include "performanceTimer.h"


FILE* openFile(char* filename)
{
    FILE *fp;
    fp = fopen(filename, "a");
    if (!fp)
    {
        fp = stdout;
    }
    return fp;
}

void printTime(struct timespec tstart,char* functionName)
{
//    struct timeb end_time;
//    ftime(&end_time);
//    int time = (int) ((end_time.time - start_time.time) * 1000) + (end_time.millitm - start_time.millitm);
//    printf("function %s took %i miliseconds to complete\n",functionName ,time);


    struct timespec tend={0,0};
    clock_gettime(CLOCK_MONOTONIC, &tend);

    double timeDif = (((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec)) * 1000;

#ifdef minimumTime
    if(timeDif < minimumTime) return;
#endif //mintime

#ifdef functionFilter
    if(strcmp(functionName,functionFilter)) return;
#endif //functionFilter

#ifndef printOnlyFinal
    FILE *fp = openFile("functionTimes.txt");
    fprintf(fp,"%.3f miliseconds for %s \n", timeDif ,functionName);
    fclose(fp);

#endif //printOnlyFinal

    if(timeDif > longestTime)
    {
        longestTime = timeDif;
        slowestFunction = functionName;
    }
}




void finalTimer(struct timespec tstart, char *testedFunction)
{
    FILE *fp = openFile("finalTimes.txt");
    if(longestTime != 0)
    {
        fprintf(fp, "the slowest function is %s with %.3f miliseconds\n", slowestFunction, longestTime);
    }
    struct timespec tend={0,0};
    clock_gettime(CLOCK_MONOTONIC, &tend);

    double timeDif = (((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));

    fprintf(fp,"%.6f seconds needed for section: %s \n\n", timeDif,testedFunction);
    longestTime = 0;
    slowestFunction = "NoSlowFunctions";
    fclose(fp);
}
