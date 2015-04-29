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
    char*  docname = (char *) malloc(500);
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
