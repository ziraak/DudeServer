#include "channel.h"

int countMessages(messageInfo *message)
{
    int total = 0;
    while (message[total].writer != NULL)
    {
        total++;
    }
    return total;
}

void writeMessage(messageInfo message, char* channelName)
{
    xmlDocPtr docPtr;
    xmlNodePtr currentNodePtr;
    xmlNodePtr root_node;
    char *docname;
    docname = (char *) malloc(DB_DOCNAMEMEMORYSPACE);

    sprintf(docname, "%s%s.xml", DB_CHANNELLOCATION, channelName);

    if ((docPtr = openDoc(docname)) == NULL)
    {
        return;
    }

    if ((currentNodePtr = checkDoc(docPtr, "channel")) == NULL)
    {
        return;
    }

    while (currentNodePtr != NULL)
    {
        if (!xmlStrcmp(currentNodePtr->name,BAD_CAST "messages"))
        {
            xmlNodePtr newMsgPtr = xmlNewChild(currentNodePtr,NULL,BAD_CAST "message",NULL);
            xmlNewProp(newMsgPtr,BAD_CAST  "user",BAD_CAST message.writer);
            xmlNewTextChild(newMsgPtr,NULL,BAD_CAST "timestamp",BAD_CAST message.timestamp);
            xmlNewTextChild(newMsgPtr,NULL,BAD_CAST "body",BAD_CAST message.body);
            break;
        }
        currentNodePtr = currentNodePtr->next;
    }

    xmlSaveFormatFileEnc(docname, docPtr,DB_XML_ENCODING, DB_XML_FORMAT);
    xmlFreeDoc(docPtr);
    free(docname);
}
void deleteMessage(char* channelname)
{
    xmlDocPtr docPtr;
    xmlNodePtr currentNodePtr;
    char *docname;
    docname = (char *) malloc(DB_DOCNAMEMEMORYSPACE);

    sprintf(docname, "%s%s.xml", DB_CHANNELLOCATION, channelname);

    if ((docPtr = openDoc(docname)) == NULL)
    {
        return;
    }

    if ((currentNodePtr = checkDoc(docPtr, "channel")) == NULL)
    {
        return;
    }

    while (currentNodePtr != NULL)
    {
        if ((!xmlStrcmp(currentNodePtr->name, (const xmlChar *) "messages")))
        {
            currentNodePtr = currentNodePtr->children;
            while (currentNodePtr != NULL)
            {
                if ((!xmlStrcmp(currentNodePtr->name, (const xmlChar *) "message")))
                {
                    xmlUnlinkNode(currentNodePtr);
                    break;
                }
                currentNodePtr = currentNodePtr->next;
            }
            break;
        }
        currentNodePtr = currentNodePtr->next;
    }
    xmlSaveFormatFile(docname, docPtr, DB_XML_FORMAT);
    xmlFreeDoc(docPtr);
    free(docname);
}

int writeMessageToChannel(char *channelName, messageInfo message)
{
    messageInfo *ci;
    ci = getMessages(channelName);
    int messageCount = countMessages(ci);
    free(ci);
    if (messageCount >= maxMessages)
    {
        deleteMessage(channelName);
    }
    writeMessage(message,channelName);

    return DB_RETURN_SUCCES;
}





int getChannel(char *channelName, channelInfo *channel)
{
    char *docname = (char *) malloc(DB_DOCNAMEMEMORYSPACE);
    xmlDocPtr docPtr;
    xmlNodePtr currentNodePtr;

    int checkChannelResult = checkChannel(channelName);

    if (checkChannelResult == DB_RETURN_NULLPOINTER || checkChannelResult == DB_RETURN_DOESNOTEXIST)
    {
        fprintf(stderr, "channel: %s does not exist\n", channelName);
        return checkChannelResult;
    }

    sprintf(docname, "%s%s.xml", DB_CHANNELLOCATION, channelName);

    if ((docPtr = openDoc(docname)) == NULL)
    {
        return DB_RETURN_FILENOTFOUND;
    }

    if ((currentNodePtr = checkDoc(docPtr, "channel")) == NULL)
    {
        return DB_RETURN_CORRUPTFILE;
    }

    channel->name = getValue(docPtr, currentNodePtr, "name");
    channel->password = getValue(docPtr, currentNodePtr, "password");
    channel->topic = getValue(docPtr, currentNodePtr, "topic");
    channel->users = getListOfValues(docPtr, currentNodePtr, "users", "user");
    channel->messages = getMessages(channelName);

    xmlFreeDoc(docPtr);
    free(docname);
    return DB_RETURN_SUCCES;
}

channelUser *getUsersFromChannel(char *channelName)
{
    char *docname = (char *) malloc(DB_DOCNAMEMEMORYSPACE);
    xmlDocPtr docPtr;
    xmlNodePtr nodePtr;
    channelUser* users = NULL;

    sprintf(docname, "%s%s.xml", DB_CHANNELLOCATION, channelName);

    if ((docPtr = openDoc(docname)) == NULL)
    {
        return users;
    }

    if ((nodePtr = checkDoc(docPtr, "channel")) == NULL)
    {
        return users;
    }

    int i = 0;

    users = malloc(5000);
    while(nodePtr != NULL)
    {
        if(!xmlStrcmp(nodePtr->name, (const xmlChar *) BAD_CAST "users"))
        {
            xmlNodePtr currentNodePtrChild;
            currentNodePtrChild = nodePtr->xmlChildrenNode;
            while (currentNodePtrChild != NULL)
            {
                if ((!xmlStrcmp(currentNodePtrChild->name, BAD_CAST "user")))
                {
                    users[i].username = (char *) xmlNodeListGetString(docPtr, currentNodePtrChild->xmlChildrenNode, 1);
                    users[i].role = (char *) xmlGetProp(currentNodePtrChild,BAD_CAST "role");
                    users[i].nickname = getUserNickname(users[i].username);
                    i++;
                }
                currentNodePtrChild = currentNodePtrChild->next;
            }
        }
        nodePtr = nodePtr->next;
    }

    users[i].username = NULL;
    users[i].role = NULL;
    xmlFreeDoc(docPtr);

//    free(docname);
    return users;
}

char* getUserRole(char* channelName, char* username)
{
    channelUser *users = getUsersFromChannel(channelName);
    int index = 0;
    while(users[index].username != NULL)
    {
        if(!strcmp(users[index].username,username))
        {
            char *role = users[index].role;
            channelUser_free(users);
            return role;
        }
        index++;
    }
    return NULL;
}



char **getChannellist()
{
    xmlDocPtr docPtr;
    xmlNodePtr currentNodePtr;
    char **list;

    docPtr = openDoc(DB_CHANNELLISTLOCATION);
    currentNodePtr = checkDoc(docPtr, "channels");
    currentNodePtr = currentNodePtr->parent;

    list = getListOfValues(docPtr, currentNodePtr, "channels", "channel");
    xmlFreeDoc(docPtr);
    return list;
}

char **getVisibleChannels()
{
    xmlDocPtr docPtr;
    xmlNodePtr currentNodePtr;

    docPtr = openDoc(DB_CHANNELLISTLOCATION);
    currentNodePtr = checkDoc(docPtr, "channels");
    char **key;
    key = calloc(50, 500);
    //TODO: malloc
    int i;
    i = 0;
    while (currentNodePtr != NULL)
    {
        if ((!xmlStrcmp(currentNodePtr->name, (const xmlChar *) BAD_CAST "channel")) &&
                !xmlStrcmp(xmlGetProp(currentNodePtr, (xmlChar *) "visible"),BAD_CAST DB_VISIBLE_TRUE ))
        {
            key[i] = (char *) xmlNodeListGetString(docPtr, currentNodePtr->xmlChildrenNode, 1);
            i++;
        }
        currentNodePtr = currentNodePtr->next;
    }
    return key;
}


int checkIfChannelVisible(char* channelName)
{
    if (channelName == NULL)
    {
        fprintf(stderr, "channel can not be NULL ");
        return DB_RETURN_NULLPOINTER;
    }

    char **channellist;
    channellist = getVisibleChannels();
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
    return BOOL_FALSE;
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
    free(channellist);
    return DB_RETURN_DOESNOTEXIST;
}


int deleteChannelInDB(char *channelName)
{
    channelInfo channel;
    char *docname = (char *) malloc(DB_DOCNAMEMEMORYSPACE);
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
    xmlNodePtr currentNodePtr;


    if ((docPtr = openDoc(DB_CHANNELLISTLOCATION)) == NULL)
    {
        printf("error\n");
        return DB_RETURN_FILENOTFOUND;
    }

    if ((currentNodePtr = checkDoc(docPtr, "channels")) == NULL)
    {
        printf("error\n");
        return DB_RETURN_CORRUPTFILE;
    }
    deleteField(docPtr, currentNodePtr, channelName);

    xmlSaveFormatFile(DB_CHANNELLISTLOCATION, docPtr, DB_XML_FORMAT);
    xmlFreeDoc(docPtr);
    return DB_RETURN_SUCCES;
}

int deleteUserFromChannel(char *channelName, char *username)
{
    xmlDocPtr docPtr;
    xmlNodePtr currentNodePtr;
    char *docname;
    docname = (char *) malloc(DB_DOCNAMEMEMORYSPACE);

    sprintf(docname, "%s%s.xml", DB_CHANNELLOCATION, channelName);

    if ((docPtr = openDoc(docname)) == NULL)
    {
        return DB_RETURN_FILENOTFOUND;
    }

    if ((currentNodePtr = checkDoc(docPtr, "channel")) == NULL)
    {
        return DB_RETURN_CORRUPTFILE;
    }

    while (currentNodePtr != NULL)
    {
        if ((!xmlStrcmp(currentNodePtr->name, (const xmlChar *) "users")))
        {
            deleteField(docPtr, currentNodePtr->xmlChildrenNode, username);
        }
        currentNodePtr = currentNodePtr->next;
    }
    xmlSaveFormatFile(docname, docPtr, DB_XML_FORMAT);
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
    char *documentName = malloc(DB_DOCNAMEMEMORYSPACE);
    xmlDocPtr docPtr;
    xmlNodePtr nodePtr;
    messageInfo *messages = malloc(DB_MAXMESSAGES * sizeof(messageInfo));

    sprintf(documentName, "%s%s.xml", DB_CHANNELLOCATION, channelName);

    if ((docPtr = openDoc(documentName)) == NULL)
    {
        return messages;
    }

    free(documentName);

    if ((nodePtr = checkDoc(docPtr, "channel")) == NULL)
    {
        return messages;
    }

    int index = 0;
    while (nodePtr != NULL)
    {
        if ((!xmlStrcmp(nodePtr->name, (const xmlChar *) "messages")))
        {
            xmlNodePtr currentNodePtrChild;
            currentNodePtrChild = nodePtr->xmlChildrenNode;
            while (currentNodePtrChild != NULL)
            {
                if ((!xmlStrcmp(currentNodePtrChild->name, (const xmlChar *) "message")) &&
                    atoi(getValue(docPtr, currentNodePtrChild->xmlChildrenNode, "timestamp")) >= timestamp)
                {
                    messages[index].writer = (char *) xmlGetProp(currentNodePtrChild, (xmlChar *) "user");
                    messages[index].timestamp = getValue(docPtr, currentNodePtrChild->xmlChildrenNode, "timestamp");
                    messages[index].body = getValue(docPtr, currentNodePtrChild->xmlChildrenNode, "body");
                    index++;
                }
                currentNodePtrChild = currentNodePtrChild->next;
            }
        }
        nodePtr = nodePtr->next;
    }
    xmlFreeDoc(docPtr);

    messages[index].writer = NULL;
    messages[index].body = NULL;
    messages[index].timestamp = NULL;

    return messages;
}

void createNewChannel(char *channelName, char *password, char *topic, int visible)
{
    xmlDocPtr docPtr = NULL;
    xmlNodePtr root_node = NULL;
    char *docname = malloc(DB_DOCNAMEMEMORYSPACE);

    docPtr = xmlNewDoc(BAD_CAST "1.0");
    root_node = xmlNewNode(NULL, BAD_CAST "channel");
    xmlDocSetRootElement(docPtr, root_node);
    xmlNewChild(root_node, NULL, BAD_CAST "name", BAD_CAST channelName);
    if (topic != NULL)
    {
        xmlNewChild(root_node, NULL, BAD_CAST "topic", BAD_CAST topic);
    }
    if (password != NULL)
    {
        xmlNewChild(root_node, NULL, BAD_CAST "password", BAD_CAST password);
    }

    xmlNewChild(root_node, NULL, BAD_CAST "users", NULL);
    xmlNewChild(root_node, NULL, BAD_CAST "messages", NULL);

    sprintf(docname, "%s%s.xml", DB_CHANNELLOCATION, channelName);
    xmlSaveFormatFileEnc(docname, docPtr, DB_XML_ENCODING, DB_XML_FORMAT);
    xmlFreeDoc(docPtr);
//    xmlCleanupParser();
    free(docname);
    addToListFile("channel", channelName);
    addChannelToList(channelName, visible);
}

void addChannelToList(char *channelName, int visible)
{
    xmlDocPtr docPtr;
    xmlNodePtr currentNodePtr;
    char *docname = DB_CHANNELLISTLOCATION;
    char *doctype = "channels";
    char *visibility;
    if (visible == BOOL_TRUE)
    {
        visibility = DB_VISIBLE_TRUE;
    }
    else
    {
        visibility = DB_VISIBLE_FALSE;
    }

    if ((docPtr = openDoc(docname)) == NULL)
    {
        return;
    }

    if ((currentNodePtr = checkDoc(docPtr, doctype)) == NULL)
    {
        return;
    }

    currentNodePtr = currentNodePtr->parent;
    currentNodePtr = xmlNewTextChild(currentNodePtr, NULL, BAD_CAST "channel", BAD_CAST channelName);
    xmlNewProp(currentNodePtr, BAD_CAST "visible", BAD_CAST visibility);
    xmlSaveFormatFile(docname, docPtr, DB_XML_FORMAT);
    xmlFreeDoc(docPtr);
}


int checkIfChannelHasPassword(char *channelname)
{
    channelInfo channel;
    getChannel(channelname, &channel);
    if (channel.password == NULL)
    {
        return BOOL_FALSE;
    }
    return BOOL_TRUE;
}

int authenticateChannelPassword(char *channelname, char *password)
{
    if (checkIfChannelHasPassword(channelname) == BOOL_TRUE)
    {
        channelInfo channel;
        getChannel(channelname, &channel);
        if (!strcmp(channel.password, password))
        {
            return BOOL_TRUE;
        }
    }
    return BOOL_FALSE;
}

void newChannelPassword(char *channelName, char *newPass)
{
    assignFieldInFile("channel", channelName, "password", newPass);
}

void newChannelTopic(char *channelName, char *newTopic)
{
    assignFieldInFile("channel", channelName, "topic", newTopic);
}


void setChannelVisibility(char* channelname,int visible)
{
    xmlDocPtr docPtr;
    xmlNodePtr currentNodePtr;
    char *docname = DB_CHANNELLISTLOCATION;
    char *doctype = "channels";
    char *visibility;
    if (visible == BOOL_TRUE)
    {
        visibility = DB_VISIBLE_TRUE;
    }
    else
    {
        visibility = DB_VISIBLE_FALSE;
    }

    if ((docPtr = openDoc(docname)) == NULL)
    {
        return;
    }

    if ((currentNodePtr = checkDoc(docPtr, doctype)) == NULL)
    {
        return;
    }
    while(currentNodePtr != NULL)
    {
        if(!xmlStrcmp(xmlNodeListGetString(docPtr,currentNodePtr->xmlChildrenNode,1),BAD_CAST channelname))
        {
            xmlSetProp(currentNodePtr,BAD_CAST "visible", BAD_CAST visibility);
            break;
        }
        currentNodePtr = currentNodePtr->next;
    }

    xmlSaveFormatFile(docname, docPtr, DB_XML_FORMAT);
    xmlFreeDoc(docPtr);
}

// TODO: return waarde toevoegen? kan hier nu niet op controleren of het goed is gegaan in MODE (Sjuul)
void setChannelUserRole(char* channelname, char* username, char* newRole)
{
    char *docname = (char *) malloc(DB_DOCNAMEMEMORYSPACE);
    xmlDocPtr docPtr;
    xmlNodePtr nodePtr;

    sprintf(docname, "%s%s.xml", DB_CHANNELLOCATION, channelname);

    if ((docPtr = openDoc(docname)) == NULL)
    {
        return;
    }

    if ((nodePtr = checkDoc(docPtr, "channel")) == NULL)
    {
        return;
    }


    while(nodePtr != NULL)
    {
        if(!xmlStrcmp(nodePtr->name, (const xmlChar *) BAD_CAST "users"))
        {
            nodePtr = nodePtr->xmlChildrenNode;
            while (nodePtr != NULL)
            {
                if (!xmlStrcmp(xmlNodeListGetString(docPtr,nodePtr->xmlChildrenNode,1),BAD_CAST username))
                {
                    xmlSetProp(nodePtr,BAD_CAST "role",BAD_CAST newRole);
                    xmlSaveFormatFile(docname, docPtr, DB_XML_FORMAT);
                    xmlFreeDoc(docPtr);
                    free(docname);
                    return;
                }
                nodePtr = nodePtr->next;
            }
        }
        nodePtr = nodePtr->next;
    }
    xmlFreeDoc(docPtr);

    free(docname);

}
