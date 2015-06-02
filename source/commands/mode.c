/*
 * Command: MODE
 * MODE <channel> {[+|-]}|o|p|s|i|t|n|b|v} [<limit>] [<user>] [<ban mask>]
 *
 * o: verander channel operator rechter (geef/haal weg)
 * p: privé channel flag
 * s: geheim channel flag (invisibility)
 * i: invite-only channel flag
 * t: onderwerp kan alleen door channel operator veranderd worden
 * n: geen messages aan channel van clients buiten de channel
 * m: gemodereerde channel
 * l: set een limiet op het aan users in de channel
 * b: set een ban mask om users buiten te houden
 * v: verander of users in een moderated channel mogen praten (geef/ haal weg)
 * k: set een channel key (password)
 */
#include "mode.h"

modeStruct getFlags(commandStruct cmd)
{
    char* flags = cmd.parameters[1], flag, *parameter = NULL;
    int set = (flags[0] == '+') ? BOOL_TRUE : BOOL_FALSE,
        getParameter, getParameterCount = 1,
        error = BOOL_FALSE;

    flagStruct *fs = malloc(sizeof(flagStruct));

    int i;
    for(i = 0; (flag = flags[i + 1]) != '\0'; i++)
    {
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
            if(cmd.trailing == NULL)
            {
                error = ERR_NEEDMOREPARAMS;
                break;
            }

            parameter = cmd.trailing;
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
    if(flag.flag != 'o')
    {
        return;
    }

    updateChannelUserRole(channelName, flag.parameter, (flag.set == BOOL_TRUE) ? USER_ROLE_OPERATOR : USER_ROLE_USER);
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

    int i, j, error;
    for(i = 0; i < ms.flagCount; i++)
    {
        switch(ms.flags[i].flag)
        {
            case 'o':
                error = BOOL_TRUE;
                for(j = 0; ci.users[j] != NULL; j++)
                {
                    if(strcmp(ci.users[j], ms.flags[i].parameter) == 0)
                    {
                        error = BOOL_FALSE;
                        break;
                    }
                }

                if(error == BOOL_TRUE)
                {
                    MODE_CHECK_FLAGS_RETURN(ERR_NOTONCHANNEL, ci);
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
            case 'k':
                break;

            default:
                MODE_CHECK_FLAGS_RETURN(ERR_UMODEUNKNOWNFLAG, ci);
        }
    }

    MODE_CHECK_FLAGS_RETURN(RPL_SUCCESS, ci);
}

int handleModeCommand(commandStruct cmd)
{
    if(cmd.parameterCount < 2)
    {
        return ERR_NEEDMOREPARAMS;
    }

    char* channelName = cmd.parameters[0];

    if(strlen(cmd.parameters[1]) < 2 || (cmd.parameters[1][0] != '-' && cmd.parameters[1][0] != '+'))
    {
        return ERR_NEEDMOREPARAMS;
    }

    if(checkChannel(channelName) != BOOL_TRUE)
    {
        return ERR_NOSUCHCHANNEL;
    }

    if(userHasChannel(channelName) == BOOL_FALSE)
    {
        return ERR_NOTONCHANNEL;
    }

    char* role = getChannelUserRole(channelName, currentUser.username);
    if(role == NULL || strcmp(role, USER_ROLE_OPERATOR) != 0)
    {
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

    return result;
}

void modeStruct_free(modeStruct *ms)
{
    int i;
    for(i = 0; i < ms->flagCount; i++)
    {
        free(ms->flags[i].parameter);
    }

    free(ms->flags);
    free(ms->channelName);
}