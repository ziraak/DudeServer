#ifndef DUDESERVER_TEST_PERFORMANCE_METING_H
#define DUDESERVER_TEST_PERFORMANCE_METING_H

#include <check.h>
#include "../../source/main.h"
#include <sys/timeb.h>

int testCommandsPerformanceMeting(int amountOfCommandTestRuns);
int getDurationTest(struct timeb start_time);

#endif //DUDESERVER_TEST_PERFORMANCE_METING_H
