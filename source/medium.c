//
// Created by osboxes on 20/04/15.
//
#include "medium.h"

xmlDocPtr openDoc(char *docname);

xmlNodePtr checkDoc(xmlDocPtr doc, char *docType);

char *getValue(xmlDocPtr doc, xmlNodePtr node, char *fieldname);

char **getListOfValues(xmlDocPtr doc, xmlNodePtr node, char *listname, char *fieldname);

void mainMedium()
{
    int index;
/*
    channelInfo batcave;
    getChannel("eigendunk");
    getChannel("bier");
    batcave = getChannel("batcave");

    printf("channelname is:%s\n", batcave.name);
    index = 0;
    while (batcave.users[index] != NULL)
    {
        printf("user %i : %s\n", index + 1, batcave.users[index]);
        index++;
    }
    index = 0;
    while (batcave.messages[index].body != NULL)
    {
        printf("----------------------------------- \n message number %i\n", index + 1);
        printf("written by: %s\n", batcave.messages[index].writer);
        printf("written on: %s\n", batcave.messages[index].timestamp);
        printf("message:\n %s\n----------------------------------- \n", batcave.messages[index].body);
        index++;
    }

*/
    userInfo fatih;

//    getUser("joe");
//    getUser("patrick");
//    fatih = getUser("fatih");


    printf("username is:%s\n", fatih.username);
    printf("nickname is:%s\n", fatih.nickname);
    printf("password is:%s\n", fatih.password);
    printf("token is:%s\n",fatih.loginToken);
    index = 0;
    while (fatih.channels[index] != NULL)
    {
        printf("channel%i : %s\n", index, fatih.channels[index]);
        index++;
    }
/*
    checkUser("joe");
    checkUser("desmond");
    checkChannel("batcave");
    checkChannel("eigendunk");*/

}

int getChannel(char *channelName, channelInfo* channel)
{
    char *docname;
    messageInfo message;
    xmlDocPtr doc;
    xmlNodePtr cur;

    if (checkChannel(channelName) == EXIT_FAILURE)
    {
        fprintf(stderr, "channel: %s does not exist\n", channelName);
        return -1;
    }

    docname = (char *) malloc(500);
    sprintf(docname, "database/channels/%s.xml", channelName);

    doc = openDoc(docname);
    if (doc == NULL)
    {
        return -2;
    }
    cur = checkDoc(doc, "channel");
    if (cur == NULL)
    {
        return -3;
    }

    channel->name = getValue(doc, cur, "name");
    channel->users = getListOfValues(doc, cur, "users", "user");

    int index;
    index = 0;
    while (cur != NULL)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "messages")))
        {
            xmlNodePtr curChild;
            curChild = cur->xmlChildrenNode;
            while (curChild != NULL)
            {
                if ((!xmlStrcmp(curChild->name, (const xmlChar *) "message")))
                {

                    message.writer = (char *) xmlGetProp(curChild, (xmlChar *) "user");
                    message.timestamp = getValue(doc, curChild->xmlChildrenNode, "timestamp");
                    message.body = getValue(doc, curChild->xmlChildrenNode, "body");
                    channel->messages[index] = message;
                    index++;

                }
                curChild = curChild->next;
            }
        }

        cur = cur->next;

    }

    xmlFreeDoc(doc);
    return 0;
}



char **getChannelList()
{
    char *docname;
    docname = "database/channelList.xml";

    xmlDocPtr doc;
    xmlNodePtr cur;
    char **list;

    doc = openDoc(docname);
    cur = checkDoc(doc, "channels");
    cur = cur->parent;

    list = getListOfValues(doc, cur, "channels", "channel");
    return list;
}


int checkChannel(char *channelName)
{
    if(channelName == NULL){
        fprintf(stderr, "channel can not be NULL ");
        return EXIT_FAILURE;
    }

    char **channelList;
    channelList = getChannelList();
    int lijstIndex;
    lijstIndex = 0;

    while (channelList[lijstIndex] != NULL)
    {
        if (!strcmp(channelList[lijstIndex], channelName))
        {
            // printf("channel: %s found it was : %s \n",channelName, channelList[lijstIndex]);
            return EXIT_SUCCESS;
        }
        lijstIndex++;
    }
    printf("%s not found\n", channelName);
    return EXIT_FAILURE;
}

