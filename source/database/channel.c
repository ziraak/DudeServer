//
// Created by desmond on 4/24/15.
//

#include "channel.h"

int writeChannel()
{

    return 0;
}

xmlTextWriterPtr openChannelFile(char *username)
{
    char filename[250];
    sprintf(filename, "database/channels/%s.xml", username);
    return xmlNewTextWriterFilename(filename, 0);
}

void writeChannels(xmlTextWriterPtr xmlptr, char **channels)
{
    xmlTextWriterStartElement(xmlptr, "channels");
    while (channels != NULL && channels != 0)
    {
        if (*channels == NULL || *channels == 0)
        {
            break;
        }
        xmlTextWriterWriteElement(xmlptr, "channel", *channels);
        channels++;
    }
    xmlTextWriterEndElement(xmlptr);
}