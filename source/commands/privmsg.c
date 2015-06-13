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

    userInfo user = getClient(cmd.sender)->user;
    char *channel = cmd.parameters[0],
            *msgToSend = cmd.trailing;

    if (isUserInChannel(channel, user.username) == BOOL_FALSE)
    {
        return ERR_NOTONCHANNEL;
    }

    writeMessageToDB(msgToSend, channel, user.username);

    char* buffer = MALLOC(INNER_BUFFER_LENGTH);
    sprintf(buffer, "UNREAD %s %s %i :%s", channel, user.username, (int)time(NULL), msgToSend);
    sendToAllClients(buffer);
    FREE(buffer);

    return RPL_NOREPLY;
}

int writeMessageToDB(char *msgToSend, char *channel, char *username)
{
    messageInfo message;
    message.body = msgToSend;
    message.writer = username;
    insertMessage(message, channel);
    return DB_RETURN_SUCCES;
}