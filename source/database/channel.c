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
    char *docname = (char *) malloc(DB_DOCNAMEMEMORYSPACE);
    xmlDocPtr docPtr;
    xmlNodePtr nodePtr;

    int checkChannelResult = checkChannel(channelName);

    if (checkChannelResult == DB_RETURN_NULLPOINTER || checkChannelResult == DB_RETURN_DOESNOTEXIST)
    {
        fprintf(stderr, "channel: %s does not exist\n", channelName);

        free(docname);
        return checkChannelResult;
    }

    sprintf(docname, "%s%s.xml", DB_CHANNELLOCATION, channelName);

    if ((docPtr = openDoc(docname)) == NULL)
    {
        xmlFreeDoc(docPtr);
        free(docname);
        return DB_RETURN_FILENOTFOUND;
    }

    if ((nodePtr = checkDoc(docPtr, "channel")) == NULL)
    {
        xmlFreeNode(nodePtr);
        xmlFreeDoc(docPtr);
        free(docname);
        return DB_RETURN_CORRUPTFILE;
    }

    channel->name = getValue(docPtr, nodePtr, "name");
    channel->users = getListOfValues(docPtr, nodePtr, "users", "user");
    channel->messages = getMessages(channelName);

    xmlFreeNode(nodePtr);
    xmlFreeDoc(docPtr);
    free(docname);
    return DB_RETURN_SUCCES;
}


char **getChannellist()
{
    xmlDocPtr docPtr;
    xmlNodePtr nodePtr;
    char **list;

    docPtr = openDoc(DB_CHANNELLISTLOCATION);
    nodePtr = checkDoc(docPtr, "channels");
    nodePtr = nodePtr->parent;

    list = getListOfValues(docPtr, nodePtr, "channels", "channel");

    xmlFreeNode(nodePtr);
    xmlFreeDoc(docPtr);
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
    char *docname = (char *) malloc(DB_DOCNAMEMEMORYSPACE);
    int returnvalue = getChannel(channelName, &channel);

    if (returnvalue != DB_RETURN_SUCCES)
    {
        free(docname);
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
    free(docname);
    if (deleteChannelFromList(channelName) == DB_RETURN_SUCCES)
    {
        return DB_RETURN_SUCCES;
    }

    return EXIT_FAILURE;
}

int deleteChannelFromList(char *channelName)
{
    xmlDocPtr docPtr;
    xmlNodePtr nodePtr;


    if ((docPtr = openDoc(DB_CHANNELLISTLOCATION)) == NULL)
    {
        xmlFreeDoc(docPtr);
        printf("error\n");
        return DB_RETURN_FILENOTFOUND;
    }

    if ((nodePtr = checkDoc(docPtr, "channels")) == NULL)
    {
        xmlFreeNode(nodePtr);
        xmlFreeDoc(docPtr);
        printf("error\n");
        return DB_RETURN_CORRUPTFILE;
    }
    deleteField(docPtr, nodePtr, channelName);

    xmlSaveFormatFile(DB_CHANNELLISTLOCATION, docPtr, DB_FORMAT);
    xmlFreeNode(nodePtr);
    xmlFreeDoc(docPtr);
    return DB_RETURN_SUCCES;
}

int deleteUserFromChannel(char *channelName, char *username)
{
    xmlDocPtr docPtr;
    xmlNodePtr nodePtr;
    char *docname;
    docname = (char *) malloc(DB_DOCNAMEMEMORYSPACE);

    sprintf(docname, "%s%s.xml", DB_CHANNELLOCATION, channelName);

    if ((docPtr = openDoc(docname)) == NULL)
    {
        free(docname);
        xmlFreeDoc(docPtr);
        return DB_RETURN_FILENOTFOUND;
    }

    if ((nodePtr = checkDoc(docPtr, "channel")) == NULL)
    {
        free(docname);
        xmlFreeDoc(docPtr);
        xmlFreeNode(nodePtr);
        return DB_RETURN_CORRUPTFILE;
    }

    while (nodePtr != NULL)
    {
        if ((!xmlStrcmp(nodePtr->name, (const xmlChar *) "users")))
        {
            deleteField(docPtr, nodePtr->xmlChildrenNode, username);
        }
        nodePtr = nodePtr->next;
    }
    xmlSaveFormatFile(docname, docPtr, DB_FORMAT);
    xmlFreeNode(nodePtr);
    xmlFreeDoc(docPtr);
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
    char *docname = (char *) malloc(DB_DOCNAMEMEMORYSPACE);
    xmlDocPtr docPtr;
    xmlNodePtr nodePtr;
    messageInfo *messages = malloc(100);

    sprintf(docname, "%s%s.xml", DB_CHANNELLOCATION, channelName);

    if ((docPtr = openDoc(docname)) == NULL)
    {
        free(docname);
        free(messages);
        xmlFreeDoc(docPtr);
        return messages;
    }

    if ((nodePtr = checkDoc(docPtr, "channel")) == NULL)
    {
        free(docname);
        free(messages);
        xmlFreeNode(nodePtr);
        xmlFreeDoc(docPtr);
        return messages;
    }

    int index = 0;
    while (nodePtr != NULL)
    {
        if ((!xmlStrcmp(nodePtr->name, (const xmlChar *) "messages")))
        {
            xmlNodePtr nodePtrChild;
            nodePtrChild = nodePtr->xmlChildrenNode;
            while (nodePtrChild != NULL)
            {
                if ((!xmlStrcmp(nodePtrChild->name, (const xmlChar *) "message")) &&
                    atoi(getValue(docPtr, nodePtrChild->xmlChildrenNode, "timestamp")) > timestamp)
                {
                    messages[index].writer = (char *) xmlGetProp(nodePtrChild, (xmlChar *) "user");
                    messages[index].timestamp = getValue(docPtr, nodePtrChild->xmlChildrenNode, "timestamp");
                    messages[index].body = getValue(docPtr, nodePtrChild->xmlChildrenNode, "body");
                    index++;
                }
                nodePtrChild = nodePtrChild->next;
            }
            xmlFreeNode(nodePtrChild);
        }
        nodePtr = nodePtr->next;
    }
    xmlFreeNode(nodePtr);
    xmlFreeDoc(docPtr);
    free(docname);
    return messages;
}



void createNewChannel(char *channelName)
{
    xmlDocPtr docPtr = NULL;
    xmlNodePtr root_node = NULL;
    char* docname = malloc(DB_DOCNAMEMEMORYSPACE);

    docPtr = xmlNewDoc(BAD_CAST "1.0");
    root_node = xmlNewNode(NULL, BAD_CAST "channel");
    xmlDocSetRootElement(docPtr, root_node);
    xmlNewChild(root_node, NULL, BAD_CAST "name",(xmlChar* )channelName);
    xmlNewChild(root_node, NULL, BAD_CAST "users",NULL);
    xmlNewChild(root_node, NULL, BAD_CAST "messages",NULL);

    sprintf(docname, "%s%s.xml", DB_CHANNELLOCATION, channelName);
    xmlSaveFormatFileEnc(docname, docPtr, DB_XML_ENCODING , DB_FORMAT);
    xmlFreeNode(root_node);
    xmlFreeDoc(docPtr);
    xmlCleanupParser();

    addToListFile("channel", channelName);
    free(docname);
}