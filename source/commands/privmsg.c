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
        ERROR_NEED_MORE_PARAMETERS(cmd.message, 2, cmd.sender);
    }

    userInfo user = getClient(cmd.sender)->user;
    char *channel = cmd.parameters[0],
            *msgToSend = cmd.trailing;

    if (isUserInChannel(channel, user.username) == BOOL_FALSE)
    {
        return ERR_NOTONCHANNEL;
    }

    writeMessageToDB(msgToSend, channel, user.username);

    return RPL_NOREPLY;
}

int writeMessageToDB(char *msgToSend, char *channel, char *username)
{
    messageInfo message;
    message.body = msgToSend;
    message.writer = username;
    insertMessage(message, channel);

    char* buffer = MALLOC(INNER_BUFFER_LENGTH);
    sprintf(buffer, "%i %s %s %i :%s", RPL_PRIV_MSG, channel, username, (int)time(NULL), msgToSend);
    sendToAllClients(buffer);
    FREE(buffer);

    return DB_RETURN_SUCCES;
}