#include "part.h"

extern userInfo currentUser;

int handlePartCommand(char *channelName)
{
    if (checkChannel(channelName))
    {
        deleteUserFromChannel(channelName, currentUser.username);
//        if () // TODO: check of channel leeg is
//        {
//            // TODO: DELETE channel
//            deleteChannel(channelName);
//        }
        return RPL_SUCCESS;
    }
    return ERR_NOSUCHCHANNEL;
}