//
// Created by desmond on 4/24/15.
//

#include "channel.h"

xmlTextWriterPtr openChannelFile(char *channelName);
void writeUsersToChannel(xmlTextWriterPtr  xmlptr, char **users);
void writeMessagesToChannel(xmlTextWriterPtr  xmlptr, messageInfo messages[]);
void writeMessageToXml(xmlTextWriterPtr  xmlptr, messageInfo message);

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

void writeUsersToChannel(xmlTextWriterPtr  xmlptr, char **users)
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

void writeMessagesToChannel(xmlTextWriterPtr  xmlptr, messageInfo messages[]
)
{
    xmlTextWriterStartElement(xmlptr, messagesTagName);
    while( messages->writer != NULL)
    {
        writeMessageToXml(xmlptr, *messages);
        messages++;
    }
    xmlTextWriterEndElement(xmlptr);
}

void writeMessageToXml(xmlTextWriterPtr  xmlptr, messageInfo message)
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
    while(message->writer != NULL)
    {
        total++;
        message++;
    }
    return total;
}

int writeMessageToChannel(char *channelName, messageInfo message)
{
    if(checkChannel(channelName) == EXIT_FAILURE)
        return EXIT_FAILURE;
    channelInfo ci = getChannel(channelName);
    int messageCount = countMessages(ci.messages);
    ci.messages = realloc(ci.messages,(messageCount + 2) * sizeof(messageInfo));
    ci.messages[messageCount] = message;
    memset(&ci.messages[messageCount + 1], 0, sizeof(messageInfo));
    if(messageCount >= maxMessages)
    {
        ci.messages = &ci.messages[1];
    }

    return writeChannel(ci);
}