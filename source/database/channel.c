//
// Created by desmond on 4/24/15.
//

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
    while (messages->writer != NULL)
    {
        writeMessageToXml(xmlptr, *messages);
        messages++;
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
    while (message->writer != NULL)
    {
        total++;
        message++;
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
    messageInfo message;
    xmlDocPtr doc;
    xmlNodePtr cur;

    if (checkChannel(channelName) == EXIT_FAILURE)
    {
        fprintf(stderr, "channel: %s does not exist\n", channelName);
        return DB_RETURN_DOESNOTEXIST;
    }


    sprintf(docname, "%s%s.xml", DB_CHANNELLOC, channelName);

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

    //TODO: per message alloceren!!!! niet in 1x
    channel->messages = malloc(1024);

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
    free(docname);
    return DB_RETURN_SUCCES;
}


char **getChannellist()
{
    xmlDocPtr doc;
    xmlNodePtr cur;
    char **list;

    doc = openDoc(DB_CHANNELLISTLOC);
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
        return EXIT_FAILURE;
    }

    char **channellist;
    channellist = getChannellist();
    int listIndex;
    listIndex = 0;

    while (channellist[listIndex] != NULL)
    {
        if (!strcmp(channellist[listIndex], channelName))
        {
            // printf("channel: %s found it was : %s \n",channelName, channellist[listIndex]);
            return DB_RETURN_SUCCES;
        }
        listIndex++;
    }
    printf("%s not found\n", channelName);
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

    sprintf(docname, "%s%s.xml", DB_CHANNELLOC, channelName);

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


    if ((doc = openDoc(DB_CHANNELLISTLOC)) == NULL)
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

    xmlSaveFormatFile(DB_CHANNELLISTLOC, doc, 0);
    xmlFreeDoc(doc);
    return DB_RETURN_SUCCES;
}

int deleteUserFromChannel(char *channelName, char *username)
{
    xmlDocPtr doc;
    xmlNodePtr cur;
    char *docname;
    docname = (char *) malloc(500);

    sprintf(docname, "%s%s.xml", DB_CHANNELLOC, channelName);
    printf("opening : %s\n", docname);

    if ((doc = openDoc(docname)) == NULL)
    {
        printf("error\n");
        return DB_RETURN_FILENOTFOUND;
    }

    if ((cur = checkDoc(doc, "channel")) == NULL)
    {
        printf("error\n");
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
        printf("%s is empty", info.name);
        return BOOL_TRUE;
    }
    else
    {
        printf("%s is not empty", info.name);
        return BOOL_FALSE;
    }
}
