//
// Created by desmond on 4/22/15.
//

#include "xmlWriter.h"

int writeUser(userInfo user)
{
    printf("User %s wordt opgeslagen", user.username);
    xmlTextWriterPtr xmlptr = openFile(user.username);
    if(xmlptr == NULL)
        return 1;
    xmlTextWriterStartElement(xmlptr, "user");
    xmlTextWriterWriteElement(xmlptr, "nickname", user.nickname);
    xmlTextWriterWriteElement(xmlptr, "password", user.password);
    writeChannels(xmlptr, user.channels);
    xmlTextWriterEndElement(xmlptr);
    xmlTextWriterFlush(xmlptr);
    return 0;
}

xmlTextWriterPtr openFile(char *username) {
    char filename[250];
    sprintf(filename, "database/users/%s.xml", username);
    return xmlNewTextWriterFilename(filename,0);
}

void writeChannels(xmlTextWriterPtr xmlptr, char **channels) {
    xmlTextWriterStartElement(xmlptr, "channels");
    while(channels != NULL && channels != 0)
    {
        if(*channels == NULL || *channels == 0) {
            break;
        }
        xmlTextWriterWriteElement(xmlptr, "channel", *channels);
        channels++;
    }
    xmlTextWriterEndElement(xmlptr);
}