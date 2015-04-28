//
// Created by osboxes on 28/04/15.
//

#include "database.h"



xmlDocPtr openDoc(char *docname)
{
    xmlDocPtr doc;
    doc = xmlParseFile(docname);

    if (doc == NULL)
    {
        fprintf(stderr, "Document **%s**Was not parsed successfully. \n", docname);
        return NULL;
    }
    return doc;

}

xmlNodePtr checkDoc(xmlDocPtr doc, char *docType)
{
    xmlNodePtr cur;
    cur = xmlDocGetRootElement(doc);

    if (cur == NULL)
    {
        fprintf(stderr, "empty document\n");
        xmlFreeDoc(doc);
        return NULL;
    }

    if (xmlStrcmp(cur->name, (const xmlChar *) docType))
    {
        fprintf(stderr, "document of the wrong type, this is not a %s\n",docType);
        xmlFreeDoc(doc);
        return NULL;
    }

    cur = cur->xmlChildrenNode;
    return cur;
}

char *getValue(xmlDocPtr doc, xmlNodePtr node, char *fieldname)
{
    xmlNodePtr cur;
    cur = node;
    xmlChar *key;

    while (cur != NULL)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar *) fieldname)))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
        }
        cur = cur->next;
    }
    return (char *) key;
}

char **getListOfValues(xmlDocPtr doc, xmlNodePtr node, char *listname, char *fieldname)
{
    xmlNodePtr cur;
    cur = node;
    char **key;
    key = calloc(50,1000);
    int i;
    i = 0;
    while (cur != NULL)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar *) listname)))
        {
            xmlNodePtr curChild;
            curChild = cur->xmlChildrenNode;
            while (curChild != NULL)
            {
                if ((!xmlStrcmp(curChild->name, (const xmlChar *) fieldname)))
                {
                    key[i] = (char *) xmlNodeListGetString(doc, curChild->xmlChildrenNode, 1);
                    i++;
                }
                curChild = curChild->next;
            }
        }
        cur = cur->next;
    }
    return key;
}
