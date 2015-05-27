//
// Created by osboxes on 28/04/15.
//

#include "user.h"

char **getUserList()
{
    xmlDocPtr docPtr;
    xmlNodePtr currentNodePtr;
    char **list;

    docPtr = openDoc(DB_USERLISTLOCATION);
    currentNodePtr = checkDoc(docPtr, "users");
    currentNodePtr = currentNodePtr->parent;


    list = getListOfValues(docPtr, currentNodePtr, "users", "user");
    return list;
}


int getUser(char *username, userInfo *result)
{
    char *docname = (char *) malloc(DB_DOCNAMEMEMORYSPACE);
    xmlDocPtr docPtr;
    xmlNodePtr currentNodePtr;

    int userReturn = checkUser(username);

    if (userReturn == DB_RETURN_NULLPOINTER)
    {
        return userReturn;
    }
    else if (userReturn != BOOL_TRUE)
    {
        return DB_RETURN_DOESNOTEXIST;
    }


    sprintf(docname, "%s%s.xml", DB_USERLOCATION, username);

    if ((docPtr = openDoc(docname)) == NULL)
    {
        return DB_RETURN_FILENOTFOUND;
    }
    if ((currentNodePtr = checkDoc(docPtr, "user")) == NULL)
    {
        return DB_RETURN_CORRUPTFILE;
    }


    //TODO: malloc
    result->username = malloc(30);
    result->nickname = malloc(30);
    result->password = malloc(30);
//    result->channels = malloc(1000);

    strcpy(result->username, username);
    strcpy(result->nickname, getValue(docPtr, currentNodePtr, "nickname"));
    strcpy(result->password, getValue(docPtr, currentNodePtr, "password"));
    result->channels = getListOfValues(docPtr, currentNodePtr, "channels", "channel");
    result->loginToken = getValue(docPtr, currentNodePtr, "loginToken");

    xmlFreeDoc(docPtr);
    //TODO: frees
    return DB_RETURN_SUCCES;
}
int isUserInChannel(char* channelname, char* username)
{
    channelUser *users = getUsersFromChannel(channelname);
    int index = 0;
    while(users[index].username != NULL)
    {
        if(!strcmp(users[index].username,username))
        {
            return BOOL_TRUE;
        }
        index++;
    }

    userInfo user;
    getUser(username,&user);

    int channelIndex;
    channelIndex = 0;
    while (user.channels[channelIndex] != NULL)
    {
        if(!strcmp(user.channels[channelIndex],channelname))
        {
            return BOOL_TRUE;
        }
        channelIndex++;
    }


    return BOOL_FALSE;
}


char* getUserNickname(char* username)
{
    userInfo info;
    if(getUser(username,&info) == DB_RETURN_SUCCES)
    {
        return info.nickname;
    }
    return NULL;
}




int checkUser(char *userName)
{
    if (userName == NULL)
    {
        fprintf(stderr, "user can not be NULL \n");
        return DB_RETURN_NULLPOINTER;
    }
    char **userList;
    userList = getUserList();
    int listIndex;
    listIndex = 0;
    while (userList[listIndex])
    {
        if (!strcmp(userList[listIndex], userName))
        {
            //printf("user: %s found it was : %s \n",userName,userList[listIndex]);
            return BOOL_TRUE;
        }
        listIndex++;
    }
    //printf("%s not found\n", userName);
    return BOOL_FALSE;
}

int userJoinChannel(char *username, char *channelName, char *userRole)
{
    if (checkUser(username) != BOOL_TRUE)
    {
        fprintf(stderr, "user: %s  does not exist\n", username);
        return DB_RETURN_DOESNOTEXIST;
    }
    else if (checkChannel(channelName) != BOOL_TRUE)
    {
        fprintf(stderr, " channel %s does not exist\n", channelName);
        return DB_RETURN_DOESNOTEXIST;
    }
    if(userRole == NULL)
    {
        return DB_RETURN_NULLPOINTER;
    }
    if(isUserInChannel(channelName,username) == BOOL_TRUE)
    {
        return DB_RETURN_ALREADYEXISTS;
    }

    addFieldToFileInList("user", username, "channels", "channel", channelName, NULL, NULL);
    addFieldToFileInList("channel", channelName, "users", "user", username, "role", userRole);

    return DB_RETURN_SUCCES;
}

void deleteChannelFromUser(char *username, char *channelName)
{
    xmlDocPtr docPtr;
    xmlNodePtr currentNodePtr;
    char *docname = (char *) malloc(DB_DOCNAMEMEMORYSPACE);

    sprintf(docname, "%s%s.xml", DB_USERLOCATION, username);
    printf("opening : %s\n", docname);

    if ((docPtr = openDoc(docname)) == NULL)
    {
        printf("error\n");
        return;
    }

    if ((currentNodePtr = checkDoc(docPtr, "user")) == NULL)
    {
        printf("error\n");
        return;
    }

    while (currentNodePtr != NULL)
    {
        if ((!xmlStrcmp(currentNodePtr->name, (const xmlChar *) "channels")))
        {
            deleteField(docPtr, currentNodePtr->xmlChildrenNode, channelName);
        }
        currentNodePtr = currentNodePtr->next;
    }
    xmlSaveFormatFile(docname, docPtr, DB_XML_FORMAT);
    xmlFreeDoc(docPtr);
    free(docname);
}


int deleteUser(char *username)
{
    userInfo user;
    char *docname = (char *) malloc(DB_DOCNAMEMEMORYSPACE);
    int userReturn = getUser(username, &user);

    if (userReturn != DB_RETURN_SUCCES)
    {
        return userReturn;
    }
    int channelIndex;
    channelIndex = 0;
    while (user.channels[channelIndex] != NULL)
    {
        deleteUserFromChannel(user.channels[channelIndex], username);
        channelIndex++;
    }

    sprintf(docname, "%s%s.xml", DB_USERLOCATION, username);

    remove(docname);
    free(docname);

    return deleteUserFromList(username);
}

int deleteUserFromList(char *username)
{
    xmlDocPtr docPtr;
    xmlNodePtr currentNodePtr;


    if ((docPtr = openDoc(DB_USERLISTLOCATION)) == NULL)
    {
        printf("error\n");
        return DB_RETURN_FILENOTFOUND;
    }

    if ((currentNodePtr = checkDoc(docPtr, "users")) == NULL)
    {
        printf("error\n");
        return DB_RETURN_CORRUPTFILE;
    }
    deleteField(docPtr, currentNodePtr, username);

    xmlSaveFormatFile(DB_USERLISTLOCATION, docPtr, DB_XML_FORMAT);
    xmlFreeDoc(docPtr);
    return DB_RETURN_SUCCES;
}

int changeNickname(char *username, char *newNickname)
{
    return changeFieldInFile("user", username, "nickname", newNickname);
}

int changePassword(char *username, char *newPassword)
{
    return changeFieldInFile("user", username, "password", newPassword);
}

int assignLoginToken(char *username, char *loginToken)
{
    assignFieldInFile("user", username, "loginToken", loginToken);
}

int createNewUser(char *username, char *password)
{
    int userReturn = checkUser(username);

    if (userReturn == DB_RETURN_NULLPOINTER || password == NULL)
    {
        return DB_RETURN_NULLPOINTER;
    }
    else if (userReturn != BOOL_FALSE)
    {
        return DB_RETURN_ALREADYEXISTS;
    }

    char *docname = (char *) malloc(DB_DOCNAMEMEMORYSPACE);
    sprintf(docname, "%s%s.xml", DB_USERLOCATION, username);

    if (xmlParseFile(docname) != NULL)
    {
        free(docname);
        return DB_RETURN_ALREADYEXISTS;
    }


    //printf("creating new user.\n");
    xmlDocPtr docPtr = NULL;       /* document pointer */
    xmlNodePtr root_node = NULL;/* node pointers */

    docPtr = xmlNewDoc(BAD_CAST "1.0");
    root_node = xmlNewNode(NULL, BAD_CAST "user");
    xmlDocSetRootElement(docPtr, root_node);
    xmlNewChild(root_node, NULL, BAD_CAST "nickname", BAD_CAST username);
    xmlNewChild(root_node, NULL, BAD_CAST "password", BAD_CAST password);
    xmlNewChild(root_node, NULL, BAD_CAST "channels", NULL);


    xmlSaveFormatFileEnc(docname, docPtr, DB_XML_ENCODING, DB_XML_FORMAT);
    xmlFreeDoc(docPtr);

    xmlCleanupParser();
    addToListFile("user", username);
    free(docname);
    return DB_RETURN_SUCCES;
}