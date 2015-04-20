//
// Created by osboxes on 20/04/15.
//

#include "medium.h"
#include <libxml/parser.h>
#include <libxml/xmlmemory.h>
#include <libxml/xmlreader.h>

void mainMedium()
{
    printf("something");

    xmlDocPtr doc;
    xmlNodePtr cur;
    doc = xmlParseFile("test.xml");

    if (doc ==NULL) {
        fprintf(stderr,"Document not parsed successfully. \n");
        return;
    }

    cur = xmlDocGetRootElement(doc);

    if (cur == NULL) {
        fprintf(stderr,"empty document\n");
        xmlFreeDoc(doc);
        return;
    }

    if (xmlStrcmp(cur->name, (const xmlChar *) "story")) {
        fprintf(stderr,"document of the wrong type, root node != story");
        xmlFreeDoc(doc);
        return;
    }

    return;
}
