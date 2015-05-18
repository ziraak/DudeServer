//
// Created by osboxes on 28/04/15.
//

#include "user.h"

char **getUserList()
{
    xmlDocPtr docPtr;
    xmlNodePtr nodePtr;
    char **list;

    docPtr = openDoc(DB_USERLISTLOCATION);
    nodePtr = checkDoc(docPtr, "users");
    nodePtr = nodePtr->parent;

    list = getListOfValues(docPtr, nodePtr, "users", "user");
    return list;
}


int getUser(char *username, userInfo *result)
{
    char *docname = (char *) malloc(DB_DOCNAMEMEMORYSPACE);
    xmlDocPtr docPtr;
    xmlNodePtr nodePtr;

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
    if ((nodePtr = checkDoc(docPtr, "user")) == NULL)
    {
        return DB_RETURN_CORRUPTFILE;
    }

    result->username = malloc(30);
    result->nickname = malloc(30);
    result->password = malloc(30);
    result->channels = malloc(1000);

    strcpy(result->username, username);
    strcpy(result->nickname, getValue(docPtr, nodePtr, "nickname"));
    strcpy(result->password, getValue(docPtr, nodePtr, "password"));
    result->channels = getListOfValues(docPtr, nodePtr, "channels", "channel");
    result->loginToken = getValue(docPtr, nodePtr, "loginToken");

    xmlFreeDoc(docPtr);
    return DB_RETURN_SUCCES;
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

int userJoinChannel(char *username, char *channelName)
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

    addFieldToFileInList("user", username, "channels", "channel", channelName);
    addFieldToFileInList("channel", channelName, "users", "user", username);

    return DB_RETURN_SUCCES;
}

void deleteChannelFromUser(char *username, char *channelName)
{
    xmlDocPtr docPtr;
    xmlNodePtr nodePtr;
    char *docname = (char *) malloc(500);

    sprintf(docname, "%s%s.xml", DB_USERLOCATION, username);
    printf("opening : %s\n", docname);

    if ((docPtr = openDoc(docname)) == NULL)
    {
        printf("error\n");
        return;
    }

    if ((nodePtr = checkDoc(docPtr, "user")) == NULL)
    {
        printf("error\n");
        return;
    }

    while (nodePtr != NULL)
    {
        if ((!xmlStrcmp(nodePtr->name, (const xmlChar *) "channels")))
        {
            deleteField(docPtr, nodePtr->xmlChildrenNode, channelName);
        }
        nodePtr = nodePtr->next;
    }
    xmlSaveFormatFile(docname, docPtr, 0);
    xmlFreeDoc(docPtr);
    free(docname);
}


void deleteUser(char *username)
{
    userInfo user;
    char *docname = (char *) malloc(500);

    if (getUser(username, &user) < 0)
    {
        return;
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

    deleteUserFromList(username);
    free(docname);
}

void deleteUserFromList(char *username)
{
    xmlDocPtr docPtr;
    xmlNodePtr nodePtr;

    if ((docPtr = openDoc(DB_USERLISTLOCATION)) == NULL)
    {
        printf("error\n");
        return;
    }

    if ((nodePtr = checkDoc(docPtr, "users")) == NULL)
    {
        printf("error\n");
        return;
    }
    deleteField(docPtr, nodePtr, username);

    xmlSaveFormatFile(DB_USERLISTLOCATION, docPtr, 0);
    xmlFreeDoc(docPtr);
}

void changeNickname(char *username, char *newNickname)
{
    changeFieldInFile("user", username, "nickname", newNickname);
}

void changePassword(char *username, char *newPassword)
{
    changeFieldInFile("user", username, "password", newPassword);
}

void assignLoginToken(char *username, char *loginToken)
{
    if (changeFieldInFile("user", username, "loginToken", loginToken) == BOOL_FALSE)
    {
        printf("new login token\n");
        addFieldToFile("user", username, "loginToken", loginToken);
    }

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

    char *docname = (char *) malloc(500);
    sprintf(docname, "%s%s.xml", DB_USERLOCATION, username);

    if (openDoc(docname) != NULL)
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


    xmlSaveFormatFileEnc(docname, docPtr, "UTF-8", 1);
    xmlFreeDoc(docPtr);
    xmlCleanupParser();
    addToListFile("user", username);
    free(docname);
    return DB_RETURN_SUCCES;
}