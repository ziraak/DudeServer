//
// Created by osboxes on 03/06/15.
//

#ifndef DUDESERVER_PERFORMANCETIMER_H
#define DUDESERVER_PERFORMANCETIMER_H

#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "time.h"
#include <sys/timeb.h>


double longestTime;
char* slowestFunction;

void printTime(struct timespec tstart,char* functionName);
void finalTimer(struct timespec tstart);


#define minTime 0
//#define functionFilter "aaa"
#define timeStart struct timespec tstart={0,0};clock_gettime(CLOCK_MONOTONIC, &tstart);
#define timeEnd(functionName) printTime(tstart,functionName)
#define timeStop finalTimer(tstart)


#endif //DUDESERVER_PERFORMANCETIMER_H
