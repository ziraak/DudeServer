//
// Created by osboxes on 21/04/15.
//

#include "privmsg.h"
#include "../main.h"
#include "../utils/utils.h"

int handlePrivateMessageCommand(char *message) {
    char *recipient, *msgToSend;
    int offset = substringCharacter(message, &recipient);
    msgToSend = message += offset;
    writeMessageToDB(recipient, msgToSend);

    return RPL_AWAY;
}
