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
    timeStart;
    size_t len = strlen(topic);
    if(len > 0)
    {
        len += strlen(channelName) + 7;
        char* snd = MALLOC(len);
        bzero(snd, len);
        sprintf(snd, "%i %s :%s", RPL_TOPIC, channelName, topic);
        sslSend(snd);
        FREE(snd);

        timeEnd("sendSuccesMessage");
        return RPL_NOREPLY;
    }

    timeEnd("sendSuccesMessage");
    return RPL_NOTOPIC;
}

int handleTopicCommand(commandStruct cmd)
{
    timeStart;
    //TODO: operator rechten checken (channel +t flag)
    if(cmd.parameterCount < 1)
    {
        timeEnd("handleTopicCommand");
        return ERR_NEEDMOREPARAMS;
    }

    char* channelName = cmd.parameters[0], *topic = cmd.trailing;

    if(checkChannel(channelName) != BOOL_TRUE)
    {
        timeEnd("handleTopicCommand");
        return ERR_NOSUCHCHANNEL;
    }

    if(isUserInChannel(channelName, currentUser.username) == BOOL_FALSE)
    {
        timeEnd("handleTopicCommand");
        return ERR_NOTONCHANNEL;
    }

    if(topic != NULL)
    {
        if(checkIfChannelTopicOperatorOnly(channelName) == BOOL_TRUE && userIsOperatorInChannel(channelName, currentUser.username) == BOOL_FALSE)
        {
            return ERR_CHANOPPRIVSNEEDED;
        }

        //set
        updateChannelTopic(channelName, topic);
        timeEnd("handleTopicCommand");
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
            timeEnd("handleTopicCommand");
            return result;
        }
        timeEnd("handleTopicCommand");
        return RPL_NOTOPIC;
    }
}