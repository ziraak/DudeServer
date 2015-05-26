#ifndef DUDESERVER_MAIN_H
#define DUDESERVER_MAIN_H

#include "utils/check_utils.h"
#include "server/test_suite_commands.h"
#include "database/check_user.h"
#include "database/check_channel.h"
#include "performanceMeting/test_performance_meting.h"

int unitTests();
int performanceMeting();

#endif //DUDESERVER_MAIN_H
