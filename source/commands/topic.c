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
        return ERR_NEEDMOREPARAMS;
    }

    char* channelName = cmd.parameters[0], *topic = cmd.trailing;
    userInfo user = getClient(cmd.sender)->user;

    if(checkChannel(channelName) != BOOL_TRUE)
    {
        return ERR_NOSUCHCHANNEL;
    }

    if(isUserInChannel(channelName, user.username) == BOOL_FALSE)
    {
        return ERR_NOTONCHANNEL;
    }

    if(topic != NULL)
    {
        if(checkIfChannelTopicOperatorOnly(channelName) == BOOL_TRUE && userIsOperatorInChannel(channelName, user.username) == BOOL_FALSE)
        {
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