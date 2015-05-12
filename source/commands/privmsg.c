#include "privmsg.h"

int handlePrivateMessageCommand(commandStruct cmd)
{
    if(cmd.parameterCount < 1 || cmd.trailing == NULL)
    {
        return ERR_NEEDMOREPARAMS;
    }

    char *channel = cmd.parameters[0],
            *msgToSend = cmd.trailing;
    writeMessageToDB(msgToSend, channel);

    return RPL_AWAY;
}

int writeMessageToDB(char *msgToSend, char *channel)
{
    messageInfo message;
    message.body = msgToSend;
    message.writer = currentUser.username;
    message.timestamp = malloc(11);
    sprintf(message.timestamp, "%i\0", (int)time(NULL));
    writeMessageToChannel(channel, message);
    return 0;
}