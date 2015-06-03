/*
 * Command: PRIVMSG
 * PRIVMSG <channel> :<message>
 *
 * Stuurt <message> naar <channel>.
 */
#include "privmsg.h"

int handlePrivateMessageCommand(commandStruct cmd)
{
    timeStart;
    if(cmd.parameterCount < 1 || cmd.trailing == NULL)
    {
        timeEnd("privmsg.c");
        return ERR_NEEDMOREPARAMS;
    }

    char *channel = cmd.parameters[0],
            *msgToSend = cmd.trailing;
    writeMessageToDB(msgToSend, channel);
    timeEnd("privmsg.c");
    return RPL_AWAY;
}

int writeMessageToDB(char *msgToSend, char *channel)
{
    timeStart;
    messageInfo message;
    message.body = msgToSend;
    message.writer = currentUser.username;
    message.timestamp = malloc(11);
    bzero(message.timestamp, 11);
    sprintf(message.timestamp, "%i", (int)time(NULL));
    insertMessage(message, channel);
    free(message.timestamp);
    timeEnd("privmsg.c/writeMsgToDB");
    return DB_RETURN_SUCCES;
}