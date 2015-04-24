//
// Created by desmond on 4/24/15.
//

#ifndef DUDESERVER_CHANNEL_H
#define DUDESERVER_CHANNEL_H

static char *const FILEFORMATSTRING = "database/channels/%s.xml";
#endif //DUDESERVER_CHANNEL_H

#include <libxml/xmlwriter.h>
#include <stdio.h>
#include "../medium.h"

int writeChannel(channelInfo channel);
