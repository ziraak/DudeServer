//
// Created by osboxes on 28/04/15.
//

#include "user.h"
#include "../main.h"

char **getUserList()
{
    xmlDocPtr doc;
    xmlNodePtr cur;
    char **list;

    doc = openDoc(DB_USERLISTLOC);
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

    int userReturn = checkUser(username);

    if (userReturn == DB_RETURN_NULLPOINTER)
    {
        return userReturn;
    } else if(userReturn != BOOL_TRUE){
        return DB_RETURN_DOESNOTEXIST;
    }


    sprintf(docname, "%s%s.xml",DB_USERLOC ,username);

    if ((doc = openDoc(docname)) == NULL)
    {
        return DB_RETURN_FILENOTFOUND;
    }
    if ((cur = checkDoc(doc, "user")) == NULL)
    {
        return DB_RETURN_CORRUPTFILE;
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
    if (checkUser(username) == BOOL_FALSE || checkChannel(channelName) == BOOL_FALSE)
    {
        fprintf(stderr, "user: %s or channel %s does not exist\n", username, channelName);
        return BOOL_FALSE;
    }

    addFieldToFileInList("user", username, "channels", "channel", channelName);
    addFieldToFileInList("channel", channelName, "users", "user", username);

    return BOOL_TRUE;
}

void deleteChannelFromUser(char *username, char *channelName)
{
    xmlDocPtr doc;
    xmlNodePtr cur;
    char *docname = (char *) malloc(500);

    sprintf(docname, "%s%s.xml", DB_USERLOC,username);
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

    sprintf(docname, "%s%s.xml",DB_USERLOC ,username);

    remove(docname);

    deleteUserFromList(username);
    free(docname);
}

void deleteUserFromList(char *username)
{
    xmlDocPtr doc;
    xmlNodePtr cur;

    if ((doc = openDoc(DB_USERLISTLOC)) == NULL)
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

    xmlSaveFormatFile(DB_USERLISTLOC, doc, 0);
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

void assignLoginToken(char* username, char* loginToken)
{
    if(changeFieldInFile("user",username,"loginToken",loginToken) == BOOL_FALSE)
    {
        printf("new login token\n");
        addFieldToFile("user",username,"loginToken",loginToken);
    }

}

int createNewUser(char *username, char *password)
{

    int userReturn = checkUser(username);


    if (userReturn == DB_RETURN_NULLPOINTER || password == NULL)
    {
        return DB_RETURN_NULLPOINTER;
    } else if(userReturn != BOOL_FALSE){
        return DB_RETURN_ALREADYEXISTS;
    }

    char *docname = (char *) malloc(500);
    sprintf(docname, "%s%s.xml",DB_USERLOC ,username);

    if ( openDoc(docname) != NULL)
    {
        free(docname);
        return DB_RETURN_ALREADYEXISTS;
    }


    printf("creating new user.\n");
    xmlDocPtr doc = NULL;       /* document pointer */
    xmlNodePtr root_node = NULL;/* node pointers */

    doc = xmlNewDoc(BAD_CAST "1.0");
    root_node = xmlNewNode(NULL, BAD_CAST "user");
    xmlDocSetRootElement(doc, root_node);
    xmlNewChild(root_node, NULL, BAD_CAST "nickname",BAD_CAST username);
    xmlNewChild(root_node, NULL, BAD_CAST "password",BAD_CAST password);
    xmlNewChild(root_node, NULL, BAD_CAST "channels",NULL);


    xmlSaveFormatFileEnc(docname, doc, "UTF-8", 1);
    xmlFreeDoc(doc);
    xmlCleanupParser();
    addToListFile("user",username);
    free(docname);
    return DB_RETURN_SUCCES;
}