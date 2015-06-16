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
        ERROR_NEED_MORE_PARAMETERS(cmd.message, 2, cmd.client);
    }

    userInfo user = getClient(cmd.client)->user;
    char *channel = cmd.parameters[0],
            *msgToSend = cmd.trailing;

    if (isUserInChannel(channel, user.username) == BOOL_FALSE)
    {
        ERROR_NOT_ON_CHANNEL(channel, user.username, cmd.client);
    }

    writeMessageToDB(msgToSend, channel, user.username);

    return RPL_NOREPLY;
}

int writeMessageToDB(char *msgToSend, char *channel, char *username)
{
    struct timeb timestamp;
    ftime(&timestamp);

    messageInfo message;
    message.message = msgToSend;
    message.user = username;
    message.channel = channel;
    message.timestamp = timestamp.time * 1000 + timestamp.millitm;
    insertMessage(message);
    lastTimestamp = message.timestamp;

    REPLY_PRIVATE_MESSAGE(message);

    return DB_RETURN_SUCCES;
}