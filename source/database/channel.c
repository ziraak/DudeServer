#include "channel.h"

int writeChannel(channelInfo channel)
{
    xmlTextWriterPtr file = openChannelFile(channel.name);
    xmlTextWriterStartElement(file, channelTagName);
    xmlTextWriterWriteElement(file, nameTagName, (xmlChar const *) channel.name);
    writeUsersToChannel(file, channel.users);
    writeMessagesToChannel(file, channel.messages);
    xmlTextWriterEndElement(file);
    int suc = xmlTextWriterEndDocument(file);
    xmlFreeTextWriter(file);
    return suc;
}

xmlTextWriterPtr openChannelFile(char *channelName)
{
    char filename[maxFilenameSize];
    sprintf(filename, FILEFORMATSTRING, channelName);
    return xmlNewTextWriterFilename(filename, compression);
}

void writeUsersToChannel(xmlTextWriterPtr xmlptr, char **users)
{
    xmlTextWriterStartElement(xmlptr, usersTagName);
    while (users != NULL)
    {
        if (*users == NULL)
        {
            break;
        }
        xmlTextWriterWriteElement(xmlptr, userTagName, (xmlChar const *) *users);
        users++;
    }
    xmlTextWriterEndElement(xmlptr);
}

void writeMessagesToChannel(xmlTextWriterPtr xmlptr, messageInfo messages[])
{
    xmlTextWriterStartElement(xmlptr, messagesTagName);
    int i = 0;
    while (messages[i].writer != NULL)
    {
        writeMessageToXml(xmlptr, messages[i]);
        i++;
    }
    xmlTextWriterEndElement(xmlptr);
}

void writeMessageToXml(xmlTextWriterPtr xmlptr, messageInfo message)
{
    xmlTextWriterStartElement(xmlptr, messageTagName);
    xmlTextWriterWriteAttribute(xmlptr, userTagName, (xmlChar const *) message.writer);
    xmlTextWriterWriteElement(xmlptr, timestampTagName, (xmlChar const *) message.timestamp);
    xmlTextWriterWriteElement(xmlptr, bodyTagName, (xmlChar const *) message.body);
    xmlTextWriterEndElement(xmlptr);
}

int countMessages(messageInfo *message)
{
    int total = 0;
    while (message[total].writer != NULL)
    {
        total++;
    }
    return total;
}

int writeMessageToChannel(char *channelName, messageInfo message)
{
    channelInfo ci;
    if (getChannel(channelName, &ci) < 0)
    {
        return EXIT_FAILURE;
    }

    int messageCount = countMessages(ci.messages);
    ci.messages = realloc(ci.messages, (messageCount + 2) * sizeof(messageInfo));
    ci.messages[messageCount] = message;
    memset(&ci.messages[messageCount + 1], 0, sizeof(messageInfo));
    if (messageCount >= maxMessages)
    {
        ci.messages = &ci.messages[1];
    }
    return writeChannel(ci);
}


int getChannel(char *channelName, channelInfo *channel)
{
    char *docname = (char *) malloc(500);
    xmlDocPtr doc;
    xmlNodePtr cur;

    int checkChannelResult = checkChannel(channelName);

    if (checkChannelResult == DB_RETURN_NULLPOINTER || checkChannelResult == DB_RETURN_DOESNOTEXIST)
    {
        fprintf(stderr, "channel: %s does not exist\n", channelName);
        return checkChannelResult;
    }

    sprintf(docname, "%s%s.xml", DB_CHANNELLOCATION, channelName);

    if ((doc = openDoc(docname)) == NULL)
    {
        return DB_RETURN_FILENOTFOUND;
    }

    if ((cur = checkDoc(doc, "channel")) == NULL)
    {
        return DB_RETURN_CORRUPTFILE;
    }

    channel->name = getValue(doc, cur, "name");
    channel->users = getListOfValues(doc, cur, "users", "user");
    channel->messages = getMessages(channelName);

    xmlFreeDoc(doc);
    free(docname);
    return DB_RETURN_SUCCES;
}


char **getChannellist()
{
    xmlDocPtr doc;
    xmlNodePtr cur;
    char **list;

    doc = openDoc(DB_CHANNELLISTLOCATION);
    cur = checkDoc(doc, "channels");
    cur = cur->parent;

    list = getListOfValues(doc, cur, "channels", "channel");
    return list;
}


int checkChannel(char *channelName)
{
    if (channelName == NULL)
    {
        fprintf(stderr, "channel can not be NULL ");
        return DB_RETURN_NULLPOINTER;
    }

    char **channellist;
    channellist = getChannellist();
    int listIndex;
    listIndex = 0;

    while (channellist[listIndex] != NULL)
    {
        if (!strcmp(channellist[listIndex], channelName))
        {
            return BOOL_TRUE;
        }
        listIndex++;
    }
    return DB_RETURN_DOESNOTEXIST;
}


int deleteChannelInDB(char *channelName)
{
    channelInfo channel;
    char *docname = (char *) malloc(500);
    int returnvalue = getChannel(channelName, &channel);

    if (returnvalue != DB_RETURN_SUCCES)
    {
        return returnvalue;
    }

    int userindex;
    userindex = 0;
    while (channel.users[userindex] != NULL)
    {
        deleteChannelFromUser(channel.users[userindex], channelName);
        userindex++;
    }

    sprintf(docname, "%s%s.xml", DB_CHANNELLOCATION, channelName);

    remove(docname);

    if (deleteChannelFromList(channelName) == DB_RETURN_SUCCES)
    {
        free(docname);
        return DB_RETURN_SUCCES;
    }
    return EXIT_FAILURE;
}

int deleteChannelFromList(char *channelName)
{
    xmlDocPtr doc;
    xmlNodePtr cur;


    if ((doc = openDoc(DB_CHANNELLISTLOCATION)) == NULL)
    {
        printf("error\n");
        return DB_RETURN_FILENOTFOUND;
    }

    if ((cur = checkDoc(doc, "channels")) == NULL)
    {
        printf("error\n");
        return DB_RETURN_CORRUPTFILE;
    }
    deleteField(doc, cur, channelName);

    xmlSaveFormatFile(DB_CHANNELLISTLOCATION, doc, 0);
    xmlFreeDoc(doc);
    return DB_RETURN_SUCCES;
}

int deleteUserFromChannel(char *channelName, char *username)
{
    xmlDocPtr doc;
    xmlNodePtr cur;
    char *docname;
    docname = (char *) malloc(500);

    sprintf(docname, "%s%s.xml", DB_CHANNELLOCATION, channelName);

    if ((doc = openDoc(docname)) == NULL)
    {
        return DB_RETURN_FILENOTFOUND;
    }

    if ((cur = checkDoc(doc, "channel")) == NULL)
    {
        return DB_RETURN_CORRUPTFILE;
    }

    while (cur != NULL)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "users")))
        {
            deleteField(doc, cur->xmlChildrenNode, username);
        }
        cur = cur->next;
    }
    xmlSaveFormatFile(docname, doc, 0);
    xmlFreeDoc(doc);
    free(docname);
    return DB_RETURN_SUCCES;
}

int checkIfChannelEmpty(char *channelName)
{
    channelInfo info;
    getChannel(channelName, &info);
    if (info.users[0] == NULL)
    {
        return BOOL_TRUE;
    }
    else
    {
        return BOOL_FALSE;
    }
}

messageInfo *getMessages(char *channelName)
{
    return getMessagesOnTime(channelName, 0);
}

messageInfo *getMessagesOnTime(char *channelName, int timestamp)
{
    // TODO: them mallocs lengte
    char *docname = (char *) malloc(500);
    xmlDocPtr docPtr;
    xmlNodePtr nodePtr;
    messageInfo *messages = malloc(100);

    sprintf(docname, "%s%s.xml", DB_CHANNELLOCATION, channelName);

    if ((docPtr = openDoc(docname)) == NULL)
    {
        return messages;
    }

    if ((nodePtr = checkDoc(docPtr, "channel")) == NULL)
    {
        return messages;
    }

    int index = 0;
    while (nodePtr != NULL)
    {
        if ((!xmlStrcmp(nodePtr->name, (const xmlChar *) "messages")))
        {
            xmlNodePtr curChild;
            curChild = nodePtr->xmlChildrenNode;
            while (curChild != NULL)
            {
                if ((!xmlStrcmp(curChild->name, (const xmlChar *) "message")) &&
                    atoi(getValue(docPtr, curChild->xmlChildrenNode, "timestamp")) >= timestamp)
                {
                    messages[index].writer = (char *) xmlGetProp(curChild, (xmlChar *) "user");
                    messages[index].timestamp = getValue(docPtr, curChild->xmlChildrenNode, "timestamp");
                    messages[index].body = getValue(docPtr, curChild->xmlChildrenNode, "body");
                    index++;
                }
                curChild = curChild->next;
            }
        }
        nodePtr = nodePtr->next;
    }

    messages[index].writer = NULL;
    messages[index].body = NULL;
    messages[index].timestamp = NULL;

//    xmlFreeDoc(docPtr); // TODO: free oplossen

    free(docname);
    return messages;
}

