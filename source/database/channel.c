//
// Created by desmond on 4/24/15.
//

#include "channel.h"

xmlTextWriterPtr openChannelFile(char *channelName);
void writeUsersToChannel(xmlTextWriterPtr  xmlptr, char **users);
void writeMessagesToChannel(xmlTextWriterPtr  xmlptr, messageInfo messages[]);
void writeMessageToChannel(xmlTextWriterPtr  xmlptr, messageInfo message);

int writeChannel(channelInfo channel)
{
    xmlTextWriterPtr file = openChannelFile(channel.name);

    xmlTextWriterStartElement(file, "channel");
    xmlTextWriterWriteElement(file, "name", channel.name);
    writeUsersToChannel(file, channel.users);
    writeMessagesToChannel(file, channel.messages);
    xmlTextWriterEndElement(file);
    xmlTextWriterEndDocument(file);
    return 0;
}


xmlTextWriterPtr openChannelFile(char *channelName)
{
    char filename[250];
    sprintf(filename, FILEFORMATSTRING, channelName);
    return xmlNewTextWriterFilename(filename, 0);
}

void writeUsersToChannel(xmlTextWriterPtr  xmlptr, char **users)
{

    xmlTextWriterStartElement(xmlptr, "users");
    while (users != NULL && users != 0)
    {
        if (*users == NULL || *users == 0)
        {
            break;
        }
        xmlTextWriterWriteElement(xmlptr, "user", *users);
        users++;
    }
    xmlTextWriterEndElement(xmlptr);
}

void writeMessagesToChannel(xmlTextWriterPtr  xmlptr, messageInfo messages[]
)
{
    xmlTextWriterStartElement(xmlptr, "messages");
    while( messages->writer != NULL)
    {
        writeMessageToChannel(xmlptr, *messages);
        messages++;
    }
    xmlTextWriterEndElement(xmlptr);
}

void writeMessageToChannel(xmlTextWriterPtr  xmlptr, messageInfo message)
{
    xmlTextWriterStartElement(xmlptr, "message");
    xmlTextWriterWriteAttribute(xmlptr, "user", message.writer);
    xmlTextWriterWriteElement(xmlptr, "timestamp", message.timestamp);
    xmlTextWriterWriteElement(xmlptr, "body", message.body);
    xmlTextWriterEndElement(xmlptr);
}