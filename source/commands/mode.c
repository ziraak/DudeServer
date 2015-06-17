/*
 * Command: MODE
 * MODE <channel> {[+|-]}|o|p|s|i|t|n|b|v} [<limit>] [<user>] [<ban mask>]
 *
 * o: verander channel operator rechter (geef/haal weg)
 * s: geheim channel flag (invisibility)
 * i: invite-only channel flag
 * t: onderwerp kan alleen door channel operator veranderd worden
 * k: set een channel key (password)
 */
#include "mode.h"

modeStruct getFlags(commandStruct cmd)
{
    char* flags = cmd.parameters[1], flag, *parameter = NULL;
    int set = (flags[0] == '+') ? BOOL_TRUE : BOOL_FALSE,
        getParameter, getParameterCount = 1,
        error = BOOL_FALSE;

    flagStruct *fs = MALLOC(sizeof(flagStruct));

    int i;
    for(i = 0; (flag = flags[i + 1]) != '\0'; i++)
    {
        REALLOC(fs, sizeof(flagStruct) * (i + 1));
        getParameter = (flag == 'o' || flag == 'b' || flag == 'l') ? BOOL_TRUE : BOOL_FALSE;

        if(getParameter == BOOL_TRUE)
        {
            getParameterCount++;

            if(getParameterCount >= cmd.parameterCount)
            {
                error = ERR_NEEDMOREPARAMS;
                break;
            }

            parameter = cmd.parameters[getParameterCount];
        }
        else if(flag == 'k')
        {
            if(set == BOOL_TRUE)
            {
                if(cmd.trailing == NULL)
                {
                    error = ERR_NEEDMOREPARAMS;
                    break;
                }

                parameter = cmd.trailing;
            }
        }
        else
        {
            parameter = NULL;
        }

        flagStruct f = {
                .parameter = parameter,
                .set = set,
                .flag = flag
        };

        fs[i] = f;
    }

    modeStruct ms = {
            .channelName = cmd.parameters[0],
            .error = error,
            .flagCount = i,
            .flags = fs,
            .client = cmd.client
    };

    return ms;
}

void handleSFlag(char *channelName, flagStruct flag)
{
    if(flag.flag == 's')
    {
        updateChannelVisibility(channelName, flag.set);
    }
}

void handleOFlag(char *channelName, flagStruct flag)
{
    if(flag.flag == 'o')
    {
        updateChannelUserRole(channelName, flag.parameter, (flag.set == BOOL_TRUE) ? USER_ROLE_OPERATOR : USER_ROLE_USER);
        char *msg;
        if (flag.set == BOOL_TRUE)
        {
            msg = " is now operator!!";
        }
        else
        {
            msg = " is no longer an operator!!";
        }
        char *stringToSend = MALLOC(strlen(flag.parameter) + strlen(msg) + 1);
        sprintf(stringToSend, "%s%s", flag.parameter, msg);
        sendSystemMessageToChannel(stringToSend, channelName);
        FREE(stringToSend);

        int amount;
        userInfo *users = getChannelUsers(channelName, &amount);

        int i, client;
        for(i = 0; i < amount; i++)
        {
            client = getClientIdByUsername(users[i].username);

            if(client >= 0)
            {
                handleChannelNames(channelName, client);
            }
        }

        userInfos_free(users, amount);
    }
}

void handleKFlag(char *channelName, flagStruct flag)
{
    if(flag.flag == 'k')
    {
        if(flag.set == BOOL_TRUE)
        {
            updateChannelPassword(channelName, flag.parameter);
        }
        else
        {
            updateChannelPassword(channelName, NULL);
        }
    }
}

void handleTFlag(char *channelName, flagStruct flag)
{
    if(flag.flag == 't')
    {
        updateChannelTopicOperatorOnly(channelName, flag.set);
    }
}

void handleIFlag(char *channelName, flagStruct flag)
{
    if(toupper(flag.flag) == 'i')
    {
        updateChannelInviteOnly(channelName, flag.set);
    }
}

void handleFlags(modeStruct ms)
{
    int i;
    for(i = 0; i < ms.flagCount; i++)
    {
        switch(ms.flags[i].flag)
        {
            case 's':
                handleSFlag(ms.channelName, ms.flags[i]);
                break;

            case 'o':
                handleOFlag(ms.channelName, ms.flags[i]);
                break;

            case 'k':
                handleKFlag(ms.channelName, ms.flags[i]);
                break;

            case 't':
                handleTFlag(ms.channelName, ms.flags[i]);
                break;

            case 'i':
                handleIFlag(ms.channelName, ms.flags[i]);
                break;

            default:
                break;
        }
    }
}

int checkFlags(modeStruct ms)
{
    channelInfo ci;
    if(getChannelByName(ms.channelName, &ci) != DB_RETURN_SUCCES)
    {
        return ERR_NOSUCHCHANNEL;
    }

    int userCount;
    userInfo *users = getChannelUsers(ms.channelName, &userCount);

    int i, j, error;
    for(i = 0; i < ms.flagCount; i++)
    {
        switch(ms.flags[i].flag)
        {
            case 'o':
                error = BOOL_TRUE;
                for(j = 0; j < userCount; j++)
                {
                    if(strcmp(users[j].username, ms.flags[i].parameter) == 0)
                    {
                        error = BOOL_FALSE;
                        break;
                    }
                }

                if(error == BOOL_TRUE)
                {
                    MODE_CHECK_FLAGS_RETURN(ERR_NOTONCHANNEL, ci, users, userCount);
                }
                break;

            case 'k':
                if (strlen(ms.flags[i].parameter) < MINIMUM_PASSWORD_LENGTH)
                {
                    MODE_CHECK_FLAGS_RETURN(ERR_PASSWORDTOOSHORT, ci, users, userCount);
                }
                break;

            case 's':
            case 'i':
            case 't':
                break;

            default:
                ERROR_MODE_UNKNOWN_FLAG(ms.flags[i].flag, ms.client);
        }
    }

    MODE_CHECK_FLAGS_RETURN(RPL_SUCCESS, ci, users, userCount);
}

int handleModeCommand(commandStruct cmd)
{
    if(cmd.parameterCount < 2)
    {
        ERROR_NEED_MORE_PARAMETERS(cmd.message, 2, cmd.client);
    }

    char* channelName = cmd.parameters[0];
    userInfo user = getClient(cmd.client)->user;

    if(strlen(cmd.parameters[1]) < 2 || (cmd.parameters[1][0] != '-' && cmd.parameters[1][0] != '+'))
    {
        ERROR_NEED_MORE_PARAMETERS(cmd.message, 2, cmd.client);
    }

    if(checkChannel(channelName) != BOOL_TRUE)
    {
        ERROR_NO_SUCH_CHANNEL(channelName, cmd.client);
    }

    if (checkIfUserExists(user.username) == BOOL_FALSE)
    {
        ERROR_USERNAME_NOT_KNOWN(user.username, cmd.client);
    }

    if(isUserInChannel(channelName, user.username) == BOOL_FALSE)
    {
        ERROR_NOT_ON_CHANNEL(channelName, user.username, cmd.client);
    }

    if(userIsOperatorInChannel(channelName, user.username) == BOOL_FALSE)
    {
        ERROR_CHANNEL_PRIVILEGES_NEEDED(channelName, cmd.client);
    }

    int result;
    modeStruct ms = getFlags(cmd);
    if(ms.error != BOOL_FALSE)
    {
        result = ms.error;
    }
    else
    {
        result = checkFlags(ms);

        if(result == RPL_SUCCESS)
        {
            handleFlags(ms);
        }
    }

    modeStruct_free(&ms);

    switch(result)
    {
        case ERR_NEEDMOREPARAMS:
            ERROR_NEED_MORE_PARAMETERS(cmd.message, 3, cmd.client);

        case ERR_PASSWORDTOOSHORT:
            ERROR_PASSWORD_TOO_SHORT(cmd.client);

        case ERR_NOSUCHCHANNEL:
            ERROR_NO_SUCH_CHANNEL(channelName, cmd.client);

        case ERR_NOTONCHANNEL:
            ERROR_NOT_ON_CHANNEL(channelName, user.username, cmd.client);

        default:
            break;
    }

    return result;
}

void modeStruct_free(modeStruct *ms)
{
    // ms->flags[#].parameter is uit de commandStruct gehaald en wordt van daaruit gefree()'d, zelfde geldt voor ms->channelName

    int j;
    for(j = 0; j < ms->flagCount; j++)
    {
        ms->flags[j].parameter = NULL;
    }

    FREE(ms->flags);
    ms->channelName = NULL;
}