/*
 * Command: PRIVMSG
 * PRIVMSG <channel> :<message>
 *
 * Stuurt <message> naar <channel>.
 */
#include "privmsg.h"

int handlePrivateMessageCommand(commandStruct cmd)
{
    if(cmd.parameterCount < 1 || cmd.trailing == NULL)
    {
        return ERR_NEEDMOREPARAMS;
    }

    char *channel = cmd.parameters[0],
            *msgToSend = cmd.trailing;

    if (isUserInChannel(channel, currentUser.username) == BOOL_FALSE)
    {
        return ERR_NOTONCHANNEL;
    }

    writeMessageToDB(msgToSend, channel);

    return RPL_AWAY;
}

int writeMessageToDB(char *msgToSend, char *channel)
{
    messageInfo message;
    message.body = msgToSend;
    message.writer = currentUser.username;
    insertMessage(message, channel);
    return DB_RETURN_SUCCES;
}