#include "database.h"

xmlDocPtr openDoc(char *docname)
{
    xmlDocPtr docPtr = xmlParseFile(docname);
    if (docPtr == NULL)
    {
        fprintf(stderr, "Document **%s**Was not parsed successfully. \n", docname);
    }
    return docPtr;
}

xmlNodePtr checkDoc(xmlDocPtr docPtr, char *docType)
{
    xmlNodePtr nodePtr;

    if ((nodePtr = xmlDocGetRootElement(docPtr)) == NULL)
    {
        fprintf(stderr, "empty document\n");
        xmlFreeDoc(docPtr);
        return NULL;
    }

    if (xmlStrcmp(nodePtr->name, (const xmlChar *) docType))
    {
        fprintf(stderr, "document of the wrong type, this is not a %s\n", docType);
        xmlFreeDoc(docPtr);
        return NULL;
    }

    nodePtr = nodePtr->xmlChildrenNode;
    return nodePtr;
}

char *getValue(xmlDocPtr docPtr, xmlNodePtr nodePtr, char *fieldname)
{
    xmlChar *key;

    while (nodePtr != NULL)
    {
        if ((!xmlStrcmp(nodePtr->name, (const xmlChar *) fieldname)))
        {
            key = xmlNodeListGetString(docPtr, nodePtr->xmlChildrenNode, 1);
        }
        nodePtr = nodePtr->next;
    }
    return (char *) key;
}

char** getListOfValues(xmlDocPtr docPtr, xmlNodePtr nodePtr, char *listname, char *fieldname)
{
    char **key;
    key = calloc(50, 1000);
    int i;
    i = 0;
    while (nodePtr != NULL)
    {
        if ((!xmlStrcmp(nodePtr->name, (const xmlChar *) listname)))
        {
            xmlNodePtr nodePtrChild;
            nodePtrChild = nodePtr->xmlChildrenNode;
            while (nodePtrChild != NULL)
            {
                if ((!xmlStrcmp(nodePtrChild->name, (const xmlChar *) fieldname)))
                {
                    key[i] = (char *) xmlNodeListGetString(docPtr, nodePtrChild->xmlChildrenNode, 1);
                    i++;
                }
                nodePtrChild = nodePtrChild->next;
            }
        }
        nodePtr = nodePtr->next;
    }
    return key;
}

void addFieldToFileInList(char *fileType, char *filename, char *listname, char *fieldname, char *content)
{
    xmlDocPtr docPtr;
    xmlNodePtr nodePtr;
    char *docname = (char *) malloc(DB_DOCNAMEMEMORYSPACE);

    sprintf(docname, "%s%ss/%s.xml", DB_DBLOCATION, fileType, filename);

    if ((docPtr = openDoc(docname)) == NULL)
    {
        return;
    }

    if ((nodePtr = checkDoc(docPtr, fileType)) == NULL)
    {
        return;
    }

    addChild(nodePtr, listname, fieldname, content);

    xmlSaveFormatFile(docname, docPtr, DB_FORMAT);
    xmlFreeDoc(docPtr);
    xmlFreeNode(nodePtr);
    free(docname);
}

void addFieldToFile(char *fileType, char *filename , char *fieldname, char *content)
{
    xmlDocPtr docPtr;
    xmlNodePtr nodePtr;
    char *docname = (char *) malloc(DB_DOCNAMEMEMORYSPACE);

    sprintf(docname, "%s%ss/%s.xml", DB_DBLOCATION,fileType, filename);

    if ((docPtr = openDoc(docname)) == NULL)
    {
        printf("error\n");
        return;
    }

    if ((nodePtr = checkDoc(docPtr, fileType)) == NULL)
    {
        printf("error\n");
        return;
    }
    nodePtr = nodePtr->parent;
    addChild(nodePtr, fileType, fieldname, content);

    xmlSaveFormatFile(docname, docPtr, DB_FORMAT);
    xmlFreeDoc(docPtr);
    xmlFreeNode(nodePtr);
    free(docname);
}


void addChild(xmlNodePtr nodePtr, char *parent, char *child, char *childContent)
{
    while (nodePtr != NULL)
    {
        if ((!xmlStrcmp(nodePtr->name, (const xmlChar *) parent)))
        {
            xmlNewTextChild(nodePtr, NULL, (xmlChar *) child, (xmlChar *) childContent);
            break;
        }
        nodePtr = nodePtr->next;
    }

}


void deleteField(xmlDocPtr docPtr, xmlNodePtr nodePtr, char *fieldText)
{
    while (nodePtr != NULL)
    {
        if ((!xmlStrcmp(xmlNodeListGetString(docPtr, nodePtr->xmlChildrenNode, 1), (xmlChar *) fieldText)))
        {
            xmlUnlinkNode(nodePtr);
        }
        nodePtr = nodePtr->next;
    }
}

int changeField(xmlNodePtr nodePtr, char *nodeName, char *newContent)
{
    while (nodePtr != NULL)
    {
        if ((!xmlStrcmp(nodePtr->name, (const xmlChar *) nodeName)))
        {
            xmlNodeSetContent(nodePtr,(xmlChar *) newContent);
            return DB_RETURN_SUCCES;
        }
        nodePtr = nodePtr->next;
    }
    return DB_RETURN_DOESNOTEXIST;
}

int changeFieldInFile(char *fileType, char *filename , char *fieldname, char *newContent)
{
    xmlDocPtr docPtr;
    xmlNodePtr nodePtr;
    int succes;
    char *docname = (char *) malloc(DB_DOCNAMEMEMORYSPACE);

    sprintf(docname, "%s%ss/%s.xml", DB_DBLOCATION,fileType, filename);

    if ((docPtr = openDoc(docname)) == NULL)
    {
        return DB_RETURN_FILENOTFOUND;
    }

    if ((nodePtr = checkDoc(docPtr, fileType)) == NULL)
    {
        return DB_RETURN_CORRUPTFILE;
    }

    succes = changeField(nodePtr,fieldname, newContent);

    xmlSaveFormatFile(docname, docPtr, DB_FORMAT);
    xmlFreeDoc(docPtr);
    xmlFreeNode(nodePtr);
    free(docname);
    return succes;
}
void addToListFile(char* itemType,char* newItem)
{
    xmlDocPtr docPtr;
    xmlNodePtr nodePtr;
    char *docname = (char *) malloc(DB_DOCNAMEMEMORYSPACE);
    char *doctype = (char *) malloc(DB_DOCNAMEMEMORYSPACE);
    sprintf(doctype,"%ss", itemType);
    sprintf(docname, "%s%slist.xml", DB_DBLOCATION,itemType);

    if ((docPtr = openDoc(docname)) == NULL)
    {
        return;
    }

    if ((nodePtr = checkDoc(docPtr, doctype)) == NULL)
    {
        return;
    }

    nodePtr = nodePtr->parent;
    addChild(nodePtr, doctype, itemType, newItem);
    xmlSaveFormatFile(docname, docPtr, DB_FORMAT);
    xmlFreeDoc(docPtr);
    xmlFreeNode(nodePtr);
    free(docname);
    free(doctype);
}


