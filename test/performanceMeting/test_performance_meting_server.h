#ifndef DUDESERVER_TEST_PERFORMANCE_METING_SERVER_H
#define DUDESERVER_TEST_PERFORMANCE_METING_SERVER_H

#include <check.h>
#include "../../source/server.h"
#include <sys/timeb.h>
#include "../main.h"
#include "../client/test_client.h"

int testServerPerformanceMeting(int amountOfCommandLoops);

#endif //DUDESERVER_TEST_PERFORMANCE_METING_SERVER_H
