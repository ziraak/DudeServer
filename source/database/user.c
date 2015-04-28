//
// Created by osboxes on 28/04/15.
//

#include "user.h"
#include "database.h"

char **getUserList()
{
    char *docname;
    docname = "database/userlist.xml";

    xmlDocPtr doc;
    xmlNodePtr cur;
    char **list;

    doc = openDoc(docname);
    cur = checkDoc(doc, "users");
    cur = cur->parent;

    list = getListOfValues(doc, cur, "users", "user");
    return list;
}


int getUser(char *username, userInfo* result)
{
    char *docname;
    xmlDocPtr doc;
    xmlNodePtr cur;


    if (checkUser(username) == EXIT_FAILURE)
    {
        fprintf(stderr, "user: %s does not exist\n", username);
        return -1;
    }

    docname = (char *) malloc(500);

    sprintf(docname, "database/users/%s.xml", username);

    doc = openDoc(docname);
    if (doc == NULL)
    {
        return -2;
    }
    cur = checkDoc(doc, "user");
    if (cur == NULL)
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
    result->loginToken = getValue(doc,cur,"loginToken");

    xmlFreeDoc(doc);
    return 0;
}

int checkUser(char *userName)
{
    if(userName == NULL){
        fprintf(stderr, "user can not be NULL ");
        return EXIT_FAILURE;
    }
    char **userList;
    userList = getUserList();
    int lijstIndex;
    lijstIndex = 0;
    while (userList[lijstIndex])
    {
        if (!strcmp(userList[lijstIndex], userName))
        {
            //printf("user: %s found it was : %s \n",userName,userList[lijstIndex]);
            return EXIT_SUCCESS;
        }
        lijstIndex++;
    }
    printf("%s not found\n",userName);
    return EXIT_FAILURE;
}