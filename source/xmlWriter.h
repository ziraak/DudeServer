//
// Created by desmond on 4/22/15.
//

#include <libxml/xmlwriter.h>

#ifndef DUDESERVER_XMLWRITER_H
#define DUDESERVER_XMLWRITER_H

#endif //DUDESERVER_XMLWRITER_H

struct user {
    char *nickname;
    char *username;
    char *password;
    char **channels;
};

int writeUser(struct user user);
xmlTextWriterPtr openFile(char *username);
void writeChannels(xmlTextWriterPtr xmlptr, char **channels);