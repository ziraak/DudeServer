//
// Created by desmond on 4/24/15.
//

#include <libxml/xmlstring.h>

#ifndef DUDESERVER_CHANNEL_H
#define DUDESERVER_CHANNEL_H

static char *const FILEFORMATSTRING = "database/channels/%s.xml";
static xmlChar *const channelTagName = "channel";
static xmlChar *const nameTagName = "name";
static const int maxFilenameSize = 250;
static const int compression = 0;
static xmlChar *const usersTagName = "users";
static xmlChar *const userTagName = "user";
static xmlChar *const messagesTagName = "messages";
static xmlChar *const messageTagName = "message";
static xmlChar *const timestampTagName = "timestamp";
static xmlChar *const bodyTagName = "body";
static const int maxMessages = 100;
#endif //DUDESERVER_CHANNEL_H

#include <libxml/xmlwriter.h>
#include <stdio.h>
#include "../medium.h"

int writeChannel(channelInfo channel);
int writeMessageToChannel(char *channelName, messageInfo message);
