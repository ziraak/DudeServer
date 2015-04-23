//
// Created by osboxes on 21/04/15.
//

#include "join.h"
#include "../main.h"
#include "../utils/utils.h"

int handleJoinCommand(char *message)
{
    char *channelName, *optionalChannelKey = NULL;
    int offset = substringCharacter(message, &channelName);
    if (!(*(message + offset) == '\n' || *(message + offset) == '\0'))
    {
        substringCharacter(message += offset, &optionalChannelKey);
    }

    int channelExists = findChannelByName(channelName);
    if (channelExists && optionalChannelKey != NULL)
    {
        if (authenticateChannel(channelName, optionalChannelKey))
        {
            // TODO: Join existing channel
            joinChannel(channelName);
        }
        else
        {
            // TODO: Not authenticated
            return ERR_BADCHANNELKEY;
        }
    }
    else
    {
        // TODO: Else create channel
        createChannel(channelName, optionalChannelKey);
    }

    return RPL_TOPIC;
}

int findChannelByName(char *channelName)
{
    // TODO: Find channel by name
    return 0;
}

int authenticateChannel(char *channelName, char *optionalChannelKey)
{
    // TODO: Authenticate on the given channel
    return 0;
}

int joinChannel(char *channelName)
{
    // TODO: Add current user to the channel
    return 0;
}

int createChannel(char *channelName, char *optionalChannelKey)
{
    // TODO: create channel
    return 0;
}
