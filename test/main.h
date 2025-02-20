#ifndef DUDESERVER_MAIN_H
#define DUDESERVER_MAIN_H

#include "utils/check_utils.h"
#include "server/test_suite_commands.h"
#include "database/check_user.h"
#include "database/check_channel.h"
#include "performanceMeting/test_performance_meting.h"
#include "performanceMeting/test_performance_meting_server.h"
#include "testStructs.h"
#include "knownBugs/knownBugs.h"

#define PERFORMANCE_METING_AMOUNT_OF_COMMAND_LOOPS 10
#define PERFORMANCE_METING_AMOUNT_OF_TEST_SUITES 10

int unitTests();
int performanceMetingCommandsTests();
int performanceMetingServer();
int getDurationTest(struct timeb start_time);

#endif //DUDESERVER_MAIN_H
