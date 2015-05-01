#include "privmsg.h"

int handlePrivateMessageCommand(char *message)
{
    char *recipient, *msgToSend;
    int offset = substringCharacter(message, &recipient);
    msgToSend = message += offset;
    writeMessageToDB(recipient, msgToSend);

    return RPL_AWAY;
}
