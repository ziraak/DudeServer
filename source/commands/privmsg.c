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

    if (isUserInChannel(channel, currentUser.username) == BOOL_FALSE)
    {
        return ERR_NOTONCHANNEL;
    }

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
    insertMessage(message, channel);
    timeEnd("privmsg.c/writeMsgToDB");
    return DB_RETURN_SUCCES;
}