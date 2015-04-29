//
// Created by osboxes on 28/04/15.
//

#include "database.h"


xmlDocPtr openDoc(char *docname)
{
    xmlDocPtr doc = xmlParseFile(docname);
    if (doc == NULL)
    {
        fprintf(stderr, "Document **%s**Was not parsed successfully. \n", docname);
    }
    return doc;
}

xmlNodePtr checkDoc(xmlDocPtr doc, char *docType)
{
    xmlNodePtr cur;

    if ((cur = xmlDocGetRootElement(doc)) == NULL)
    {
        fprintf(stderr, "empty document\n");
        xmlFreeDoc(doc);
        return NULL;
    }

    if (xmlStrcmp(cur->name, (const xmlChar *) docType))
    {
        fprintf(stderr, "document of the wrong type, this is not a %s\n", docType);
        xmlFreeDoc(doc);
        return NULL;
    }

    cur = cur->xmlChildrenNode;
    return cur;
}

char *getValue(xmlDocPtr doc, xmlNodePtr cur, char *fieldname)
{
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

char **getListOfValues(xmlDocPtr doc, xmlNodePtr cur, char *listname, char *fieldname)
{
    char **key;
    key = calloc(50, 1000);
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

void addFieldToFileInList(char *fileType, char *filename, char *listname, char *fieldname, char *content)
{
    xmlDocPtr doc;
    xmlNodePtr cur;
    char *docname = (char *) malloc(500);

    sprintf(docname, "xml/%ss/%s.xml", fileType, filename);

    printf("opening document %s\n", docname);

    if ((doc = openDoc(docname)) == NULL)
    {
        printf("error\n");
        return;
    }

    if ((cur = checkDoc(doc, fileType)) == NULL)
    {
        printf("error\n");
        return;
    }

    addChild(cur, listname, fieldname, content);

    xmlSaveFormatFile(docname, doc, 0);
    xmlFreeDoc(doc);
    free(docname);
}


void addChild(xmlNodePtr cur, char *parent, char *child, char *childContent)
{
    while (cur != NULL)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar *) parent)))
        {
            xmlNewTextChild(cur, NULL, (xmlChar *) child, (xmlChar *) childContent);
            break;
        }
        cur = cur->next;
    }

}


void deleteField(xmlDocPtr doc, xmlNodePtr cur, char *fieldText)
{
    while (cur != NULL)
    {
        if ((!xmlStrcmp(xmlNodeListGetString(doc, cur->xmlChildrenNode, 1), (xmlChar *) fieldText)))
        {
            xmlUnlinkNode(cur);
        }
        cur = cur->next;
    }
}

void changeField(xmlNodePtr cur, char *nodeName, char *newContent)
{
    while (cur != NULL)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar *) nodeName)))
        {
            xmlNodeSetContent(cur, newContent);
        }
        cur = cur->next;
    }
}

void changeFieldInFile(char *fileType, char *filename , char *fieldname, char *newContent)
{
    xmlDocPtr doc;
    xmlNodePtr cur;
    char *docname = (char *) malloc(500);

    sprintf(docname, "xml/%ss/%s.xml", fileType, filename);

    printf("opening document %s\n", docname);

    if ((doc = openDoc(docname)) == NULL)
    {
        printf("error\n");
        return;
    }

    if ((cur = checkDoc(doc, fileType)) == NULL)
    {
        printf("error\n");
        return;
    }

    changeField(cur,fieldname, newContent);

    xmlSaveFormatFile(docname, doc, 0);
    xmlFreeDoc(doc);
    free(docname);
}