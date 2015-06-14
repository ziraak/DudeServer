/*
 * Command: TOPIC
 * TOPIC <channel> :<#topic>
 *
 * Geeft het huidige topic van <channel> terug.
 * Indien <#topic> meegegeven is, wordt het topic gezet van <channel>.
 */
#include "topic.h"

int sendSuccessMessage(char* channelName, char* topic)
{
    size_t len = strlen(topic);
    if(len > 0)
    {
        len += strlen(channelName) + 7;
        char* snd = MALLOC(len);
        bzero(snd, len);
        sprintf(snd, "%i %s :%s", RPL_TOPIC, channelName, topic);
        sslSend(snd);
        FREE(snd);

        return RPL_NOREPLY;
    }

    return RPL_NOTOPIC;
}

int handleTopicCommand(commandStruct cmd)
{
    //TODO: operator rechten checken (channel +t flag)
    if(cmd.parameterCount < 1)
    {
        ERROR_NEED_MORE_PARAMETERS(cmd.message, 1, cmd.sender);
    }

    char* channelName = cmd.parameters[0], *topic = cmd.trailing;
    userInfo user = getClient(cmd.sender)->user;

    if(checkChannel(channelName) != BOOL_TRUE)
    {
        ERROR_NO_SUCH_CHANNEL(channelName, cmd.sender);
    }

    if(isUserInChannel(channelName, user.username) == BOOL_FALSE)
    {
        ERROR_NOT_ON_CHANNEL(channelName, user.username, cmd.sender);
    }

    if(topic != NULL)
    {
        if(checkIfChannelTopicOperatorOnly(channelName) == BOOL_TRUE && userIsOperatorInChannel(channelName, user.username) == BOOL_FALSE)
        {
            ERROR_CHANNEL_PRIVILEGES_NEEDED(channelName, cmd.sender);
            return ERR_CHANOPPRIVSNEEDED;
        }

        //set
        updateChannelTopic(channelName, topic);

        char *msg = "Channel topic has changed to: ";
        char *stringToSend = MALLOC(strlen(topic) + strlen(msg) + 1);
        sprintf(stringToSend, "%s%s", msg, topic);
        sendSystemMessageToChannel(stringToSend, channelName);
        FREE(stringToSend);

        return sendSuccessMessage(channelName, topic);
    }
    else
    {
        //get
        channelInfo channel;
        if(getChannelByName(channelName, &channel) == BOOL_TRUE && channel.topic != NULL)
        {
            int result = sendSuccessMessage(channel.name, channel.topic);
            channelInfo_free(&channel);
            return result;
        }
        return RPL_NOTOPIC;
    }
}