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

char** getListOfValues(xmlDocPtr doc, xmlNodePtr cur, char *listname, char *fieldname)
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

    sprintf(docname, "%s%ss/%s.xml",DB_DBLOC, fileType, filename);

   // printf("opening document %s\n", docname);

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

void addFieldToFile(char *fileType, char *filename , char *fieldname, char *content)
{
    xmlDocPtr doc;
    xmlNodePtr cur;
    char *docname = (char *) malloc(500);

    sprintf(docname, "%s%ss/%s.xml", DB_DBLOC,fileType, filename);

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
    cur = cur->parent;
    addChild(cur, fileType, fieldname, content);

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

int changeField(xmlNodePtr cur, char *nodeName, char *newContent)
{
    while (cur != NULL)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar *) nodeName)))
        {
            xmlNodeSetContent(cur,(xmlChar *) newContent);
            return BOOL_TRUE;
        }
        cur = cur->next;
    }
    return BOOL_FALSE;
}

int changeFieldInFile(char *fileType, char *filename , char *fieldname, char *newContent)
{
    xmlDocPtr doc;
    xmlNodePtr cur;
    int succes;
    char *docname = (char *) malloc(500);

    sprintf(docname, "%s%ss/%s.xml", DB_DBLOC,fileType, filename);

    printf("opening document %s\n", docname);

    if ((doc = openDoc(docname)) == NULL)
    {
        printf("error\n");
        return -1;
    }

    if ((cur = checkDoc(doc, fileType)) == NULL)
    {
        printf("error\n");
        return -1;
    }

    succes = changeField(cur,fieldname, newContent);

    xmlSaveFormatFile(docname, doc, 0);
    xmlFreeDoc(doc);
    free(docname);
    return succes;
}
void addToListFile(char* itemType,char* newItem)
{
    xmlDocPtr doc;
    xmlNodePtr cur;
    char *docname = (char *) malloc(500);
    char *doctype = (char *) malloc(50);
    sprintf(doctype,"%ss", itemType);
    sprintf(docname, "%s%slist.xml", DB_DBLOC,itemType);

    if ((doc = openDoc(docname)) == NULL)
    {
        printf("error\n");
        return;
    }

    if ((cur = checkDoc(doc, doctype)) == NULL)
    {
        printf("error\n");
        return;
    }

    cur = cur->parent;
    addChild(cur, doctype, itemType, newItem);
    xmlSaveFormatFile(docname, doc, 0);
    xmlFreeDoc(doc);
    free(docname);
    free(doctype);
}



void createNewChannel(char *channelName, char *creator)
{
    xmlDocPtr doc = NULL;       /* document pointer */
    xmlNodePtr root_node = NULL;
    char* docname = malloc(500);

    doc = xmlNewDoc(BAD_CAST "1.0");
    root_node = xmlNewNode(NULL, BAD_CAST "channel");
    xmlDocSetRootElement(doc, root_node);
    xmlNewChild(root_node, NULL, BAD_CAST "name",(xmlChar* )channelName);
    xmlNewChild(root_node, NULL, BAD_CAST "users",NULL);
    xmlNewChild(root_node, NULL, BAD_CAST "messages",NULL);

    sprintf(docname,"%s%s.xml",DB_CHANNELLOC,channelName);
    xmlSaveFormatFileEnc(docname, doc, "UTF-8", 1);
    xmlFreeDoc(doc);
    xmlCleanupParser();

    addToListFile("channel",channelName);
}