#include <time.h>
#include "privmsg.h"

int handlePrivateMessageCommand(char *message)
{
    char *channel, *msgToSend;
    int offset = substringCharacter(message, &channel);
    substringCharacter(message += offset, &msgToSend);
    writeMessageToDB(msgToSend, channel);

    return RPL_AWAY;
}

int writeMessageToDB(char *msgToSend, char *channel)
{
    messageInfo message;
    message.body = msgToSend;
    message.writer = currentUser.username;
    sprintf(message.timestamp, "%i", (int)time(NULL));
    writeMessageToChannel(channel, message);
    return 0;
}