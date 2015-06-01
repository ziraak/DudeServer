//
// Created by osboxes on 27/05/15.
//

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

    if(userHasChannel(channelName) == BOOL_FALSE)
    {
        return ERR_NOTONCHANNEL;
    }

    if(topic != NULL)
    {
        //set
        newChannelTopic(channelName, topic);

        return RPL_TOPIC;
    }
    else
    {
        //get
        channelInfo channel;
        if(getChannelByName(channelName, ALL_COLUMNS, &channel) == DB_RETURN_SUCCES && channel.topic != NULL)
        {
            size_t len = strlen(channel.topic);
            if(len > 0)
            {
                char* snd = malloc(3 + len);
                sprintf(snd, "%i %s", RPL_TOPIC, channel.topic);

                sslSend(snd);
                return RPL_TOPIC;
            }
        }
        return RPL_NOTOPIC;
    }
}