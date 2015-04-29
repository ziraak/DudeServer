//
// Created by osboxes on 28/04/15.
//

#include "user.h"

char **getUserList()
{
    char *docname;
    docname = "xml/userlist.xml";

    xmlDocPtr doc;
    xmlNodePtr cur;
    char **list;

    doc = openDoc(docname);
    cur = checkDoc(doc, "users");
    cur = cur->parent;

    list = getListOfValues(doc, cur, "users", "user");
    return list;
}


int getUser(char *username, userInfo *result)
{
    char *docname = (char *) malloc(500);
    xmlDocPtr doc;
    xmlNodePtr cur;


    if (checkUser(username) == EXIT_FAILURE)
    {
        fprintf(stderr, "user: %s does not exist\n", username);
        return -1;
    }


    sprintf(docname, "xml/users/%s.xml", username);

    if ((doc = openDoc(docname)) == NULL)
    {
        return -2;
    }
    if ((cur = checkDoc(doc, "user")) == NULL)
    {
        return -3;
    }

    result->username = malloc(30);
    result->nickname = malloc(30);
    result->password = malloc(30);
    result->channels = malloc(1000);

    strcpy(result->username, username);
    strcpy(result->nickname, getValue(doc, cur, "nickname"));
    strcpy(result->password, getValue(doc, cur, "password"));
    result->channels = getListOfValues(doc, cur, "channels", "channel");
    result->loginToken = getValue(doc, cur, "loginToken");

    xmlFreeDoc(doc);
    return 0;
}

int checkUser(char *userName)
{
    if (userName == NULL)
    {
        fprintf(stderr, "user can not be NULL ");
        return EXIT_FAILURE;
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
            return EXIT_SUCCESS;
        }
        listIndex++;
    }
    printf("%s not found\n", userName);
    return EXIT_FAILURE;
}

void userJoinChannel(char *username, char *channelName)
{
    if (checkUser(username) == EXIT_FAILURE || checkChannel(channelName) == EXIT_FAILURE)
    {
        fprintf(stderr, "user: %s or channel %s does not exist\n", username, channelName);
        return;
    }

    addFieldToFileInList("user", username, "channels", "channel", channelName);
    addFieldToFileInList("channel", channelName, "users", "user", username);
}

void deleteChannelFromUser(char *username, char *channelName)
{
    xmlDocPtr doc;
    xmlNodePtr cur;
    char *docname;
    docname = (char *) malloc(500);

    sprintf(docname, "xml/users/%s.xml", username);
    printf("opening : %s\n", docname);

    if ((doc = openDoc(docname)) == NULL)
    {
        printf("error\n");
        return;
    }

    if ((cur = checkDoc(doc, "user")) == NULL)
    {
        printf("error\n");
        return;
    }

    while (cur != NULL)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "channels")))
        {
            deleteField(doc, cur->xmlChildrenNode, channelName);
        }
        cur = cur->next;
    }
    xmlSaveFormatFile(docname, doc, 0);
    xmlFreeDoc(doc);
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

    sprintf(docname, "xml/users/%s.xml", username);

    remove(docname);

    deleteUserFromList(username);
    free(docname);
}

void deleteUserFromList(char *username)
{
    xmlDocPtr doc;
    xmlNodePtr cur;
    char *docname = "xml/userlist.xml";

    printf("opening document %s\n", docname);


    if ((doc = openDoc(docname)) == NULL)
    {
        printf("error\n");
        return;
    }

    if ((cur = checkDoc(doc, "users")) == NULL)
    {
        printf("error\n");
        return;
    }
    deleteField(doc, cur, username);

    xmlSaveFormatFile(docname, doc, 0);
    xmlFreeDoc(doc);
}

void changeNickname(char* username, char* newNickname)
{
    changeFieldInFile("user",username,"nickname",newNickname);
}

void changePassword(char* username, char* newPassword)
{
    changeFieldInFile("user",username,"password",newPassword);
}