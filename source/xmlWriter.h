//
// Created by desmond on 4/22/15.
//

#include <libxml/xmlwriter.h>
#include "medium.h"

#ifndef DUDESERVER_XMLWRITER_H
#define DUDESERVER_XMLWRITER_H

#endif //DUDESERVER_XMLWRITER_H

int writeUser(userInfo user);

xmlTextWriterPtr openFile(char *username);

void writeChannels(xmlTextWriterPtr xmlptr, char **channels);

void userJoinChannel(char *username, char *channelName);