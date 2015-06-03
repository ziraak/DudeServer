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
    timeStart;
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
            .flags = fs
    };
    timeEnd("mode.c/getFlags");
    return ms;
}

void handleSFlag(char *channelName, flagStruct flag)
{
    timeStart;
    if(flag.flag == 's')
    {
        updateChannelVisibility(channelName, flag.set);
    }
    timeEnd("handleSFlag");
}

void handleOFlag(char *channelName, flagStruct flag)
{
    timeStart;
    if(flag.flag == 'o')
    {
        updateChannelUserRole(channelName, flag.parameter, (flag.set == BOOL_TRUE) ? USER_ROLE_OPERATOR : USER_ROLE_USER);

    }
    timeEnd("handleOFlag");
}


void handleKFlag(char *channelName, flagStruct flag)
{
    timeStart;
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
    timeEnd("handleKFlag");
}

void handleTFlag(char *channelName, flagStruct flag)
{
    timeStart;
    if(flag.flag == 't')
    {
        updateChannelTopicOperatorOnly(channelName, flag.set);
    }
    timeEnd("handleKTFlag");
}

void handleIFlag(char *channelName, flagStruct flag)
{
    timeStart;
    if(toupper(flag.flag) == 'i')
    {
        updateChannelInviteOnly(channelName, flag.set);
    }
    timeEnd("handleIFlag");
}

void handleFlags(modeStruct ms)
{
    timeStart;
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
    timeEnd("mode.c/handleFlags");
}

int checkFlags(modeStruct ms)
{
    timeStart;
    channelInfo ci;
    if(getChannelByName(ms.channelName, &ci) != DB_RETURN_SUCCES)
    {
        timeEnd("checkFlags");
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
                    timeEnd("checkFlags");
                    MODE_CHECK_FLAGS_RETURN(ERR_NOTONCHANNEL, ci, users, userCount);
                }
                break;

            case 'p':
            case 's':
            case 'i':
            case 't':
            case 'n':
            case 'm':
            case 'l':
            case 'b':
            case 'v':
                break;

            default:
                timeEnd("checkFlags");
                MODE_CHECK_FLAGS_RETURN(ERR_UMODEUNKNOWNFLAG, ci, users, userCount);
        }
    }
    timeEnd("checkFlags");
    MODE_CHECK_FLAGS_RETURN(RPL_SUCCESS, ci, users, userCount);
}

int handleModeCommand(commandStruct cmd)
{
    timeStart;
    if(cmd.parameterCount < 2)
    {
        timeEnd("handleModeCommand");
        return ERR_NEEDMOREPARAMS;
    }

    char* channelName = cmd.parameters[0];

    if(strlen(cmd.parameters[1]) < 2 || (cmd.parameters[1][0] != '-' && cmd.parameters[1][0] != '+'))
    {
        timeEnd("handleModeCommand");
        return ERR_NEEDMOREPARAMS;
    }

    if(checkChannel(channelName) != BOOL_TRUE)
    {
        timeEnd("handleModeCommand");
        return ERR_NOSUCHCHANNEL;
    }

    if(isUserInChannel(channelName, currentUser.username) == BOOL_FALSE)
    {
        timeEnd("handleModeCommand");
        return ERR_NOTONCHANNEL;
    }

    if(userIsOperatorInChannel(channelName, currentUser.username) == BOOL_FALSE)
    {
        timeEnd("handleModeCommand");
        return ERR_CHANOPPRIVSNEEDED;
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
    timeEnd("handleModeCommand");
    return result;
}

void modeStruct_free(modeStruct *ms)
{
    // ms->flags[#].parameter is uit de commandStruct gehaald en wordt van daaruit gefree()'d, zelfde geldt voor ms->channelName
    timeStart;
    int j;
    for(j = 0; j < ms->flagCount; j++)
    {
        ms->flags[j].parameter = NULL;
    }

    FREE(ms->flags);
    ms->channelName = NULL;
    timeEnd("modeStruct_free");
}