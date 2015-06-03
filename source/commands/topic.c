/*
 * Command: TOPIC
 * TOPIC <channel> :<#topic>
 *
 * Geeft het huidige topic van <channel> terug.
 * Indien <#topic> meegegeven is, wordt het topic gezet van <channel>.
 */
#include "topic.h"

int handleTopicCommand(commandStruct cmd)
{
    //TODO: operator rechten checken (channel +t flag)
    if(cmd.parameterCount < 1)
    {
        return ERR_NEEDMOREPARAMS;
    }

    char* channelName = cmd.parameters[0], *topic = cmd.trailing;

    if(checkChannel(channelName) != BOOL_TRUE)
    {
        return ERR_NOSUCHCHANNEL;
    }

    if(isUserInChannel(channelName, currentUser.username) == BOOL_FALSE)
    {
        return ERR_NOTONCHANNEL;
    }

    if(topic != NULL)
    {
        //set
        updateChannelTopic(channelName, topic);

        return RPL_TOPIC;
    }
    else
    {
        //get
        channelInfo channel;
        if(getChannelByName(channelName, &channel) == BOOL_TRUE && channel.topic != NULL)
        {
            size_t len = strlen(channel.topic);
            if(len > 0)
            {
                char* snd = malloc(3 + len);
                sprintf(snd, "%i %s", RPL_TOPIC, channel.topic);

                sslSend(snd);
                channelInfo_free(&channel);
                return RPL_NOREPLY;
            }
            channelInfo_free(&channel);
        }
        return RPL_NOTOPIC;
    }
}