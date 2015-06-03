#ifndef MAIN_H
#define MAIN_H
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "performanceTimer.h"
#include "ssl/communication.h"
#include "utils/commandStruct.h"
#include "utils/utils.h"
#include "commands/poll.h"
#include "commands/login.h"
#include "commands/join.h"
#include "commands/privmsg.h"
#include "commands/create_user.h"
#include "commands/delete_user.h"
#include "commands/update_password.h"
#include "commands/update_nickname.h"
#include "commands/part.h"
#include "commands/topic.h"
#include "commands/mode.h"
#include "database/database.h"
#include "protocolDefines.h"

#include "server.h"
#include "commands/invite.h"
#include "commands/names.h"
#include "commands/kick.h"





#define BOOL_TRUE 1
#define BOOL_FALSE 0




#define MINIMUM_PASSWORD_LENGTH 6

#define MAXIMUM_MESSAGE_LENGTH 1024




#endif
