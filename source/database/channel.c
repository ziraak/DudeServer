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
        return -1;
    }

    sprintf(docname, "xml/channels/%s.xml", channelName);

    if ((doc = openDoc(docname)) == NULL)
    {
        return -2;
    }

    if ((cur = checkDoc(doc, "channel")) == NULL)
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
    free(docname);
    return 0;
}


char **getChannelList()
{
    char *docname;
    docname = "xml/channelList.xml";

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
    if (channelName == NULL)
    {
        fprintf(stderr, "channel can not be NULL ");
        return EXIT_FAILURE;
    }

    char **channelList;
    channelList = getChannelList();
    int listIndex;
    listIndex = 0;

    while (channelList[listIndex] != NULL)
    {
        if (!strcmp(channelList[listIndex], channelName))
        {
            // printf("channel: %s found it was : %s \n",channelName, channelList[listIndex]);
            return EXIT_SUCCESS;
        }
        listIndex++;
    }
    printf("%s not found\n", channelName);
    return EXIT_FAILURE;
}


void deleteChannel(char *channelName)
{
    channelInfo channel;
    char *docname = (char *) malloc(500);

    if (getChannel(channelName, &channel) < 0)
    {
        return;
    }

    int userindex;
    userindex = 0;
    while (channel.users[userindex] != NULL)
    {
        deleteChannelFromUser(channel.users[userindex], channelName);
        userindex++;
    }

    sprintf(docname, "xml/channels/%s.xml", channelName);

    remove(docname);

    deleteChannelFromList(channelName);
    free(docname);
}

void deleteChannelFromList(char *channelName)
{
    xmlDocPtr doc;
    xmlNodePtr cur;
    char* docname = "xml/channelList.xml";

    printf("opening document %s\n", docname);


    if ((doc = openDoc(docname)) == NULL)
    {
        printf("error\n");
        return;
    }

    if ((cur = checkDoc(doc, "channels")) == NULL)
    {
        printf("error\n");
        return;
    }
    deleteField(doc, cur, channelName);

    xmlSaveFormatFile(docname, doc, 0);
    xmlFreeDoc(doc);
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

