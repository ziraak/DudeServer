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
    xmlNodePtr currentNodePtr;
    if ((currentNodePtr = xmlDocGetRootElement(docPtr)) == NULL)
    {
        fprintf(stderr, "empty document\n");
        xmlFreeDoc(docPtr);
        return NULL;
    }

    if (xmlStrcmp(currentNodePtr->name, (const xmlChar *) docType))
    {
        fprintf(stderr, "document of the wrong type, this is not a %s\n", docType);
        xmlFreeDoc(docPtr);
        return NULL;
    }

    currentNodePtr = currentNodePtr->xmlChildrenNode;
    return currentNodePtr;
}

char *getValue(xmlDocPtr docPtr, xmlNodePtr currentNodePtr, char *fieldname)
{
    xmlChar *key = NULL;
    while (currentNodePtr != NULL)
    {
        if ((!xmlStrcmp(currentNodePtr->name, (const xmlChar *) fieldname)))
        {
            key = xmlNodeListGetString(docPtr, currentNodePtr->xmlChildrenNode, 1);
        }
        currentNodePtr = currentNodePtr->next;
    }
    return (char *) key;
}

char** getListOfValues(xmlDocPtr docPtr, xmlNodePtr currentNodePtr, char *listname, char *fieldname)
{
    char **key;
    key = calloc(50, 500);
    //TODO: malloc
    int i;
    i = 0;
    while (currentNodePtr != NULL)
    {
        if ((!xmlStrcmp(currentNodePtr->name, (const xmlChar *) listname)))
        {
            xmlNodePtr currentNodePtrChild;
            currentNodePtrChild = currentNodePtr->xmlChildrenNode;
            while (currentNodePtrChild != NULL)
            {
                if ((!xmlStrcmp(currentNodePtrChild->name, (const xmlChar *) fieldname)))
                {
                    key[i] = (char *) xmlNodeListGetString(docPtr, currentNodePtrChild->xmlChildrenNode, 1);
                    i++;
                }
                currentNodePtrChild = currentNodePtrChild->next;
            }
        }
        currentNodePtr = currentNodePtr->next;
    }
    return key;
}

void addFieldToFileInList(char *fileType, char *filename, char *listname, char *fieldname, char *content,
                          char *optPropertyName, char *optPropertyValue)
{
    xmlDocPtr docPtr;
    xmlNodePtr currentNodePtr;
    char *docname = (char *) malloc(DB_DOCNAMEMEMORYSPACE);

    sprintf(docname, "%s%ss/%s.xml",DB_DBLOCATION, fileType, filename);

    if ((docPtr = openDoc(docname)) == NULL)
    {
        return;
    }

    if ((currentNodePtr = checkDoc(docPtr, fileType)) == NULL)
    {
        return;
    }

    addChild(currentNodePtr, listname, fieldname, content, optPropertyName, optPropertyValue);

    xmlSaveFormatFile(docname, docPtr, DB_XML_FORMAT);
    xmlFreeDoc(docPtr);
    free(docname);
}

void addFieldToFile(char *fileType, char *filename , char *fieldname, char *content)
{
    xmlDocPtr docPtr;
    xmlNodePtr currentNodePtr;
    char *docname = (char *) malloc(DB_DOCNAMEMEMORYSPACE);

    sprintf(docname, "%s%ss/%s.xml", DB_DBLOCATION,fileType, filename);

    if ((docPtr = openDoc(docname)) == NULL)
    {
        printf("error\n");
        return;
    }

    if ((currentNodePtr = checkDoc(docPtr, fileType)) == NULL)
    {
        printf("error\n");
        return;
    }
    currentNodePtr = currentNodePtr->parent;
    addChild(currentNodePtr, fileType, fieldname, content, NULL, NULL);
    xmlSaveFormatFile(docname, docPtr, DB_XML_FORMAT);
    xmlFreeDoc(docPtr);
    free(docname);
}


void addChild(xmlNodePtr currentNodePtr, char *parent, char *child, char *childContent, char *optPropertyName,
              char *optPropertyValue)
{
    while (currentNodePtr != NULL)
    {
        if ((!xmlStrcmp(currentNodePtr->name, (const xmlChar *) parent)))
        {
            currentNodePtr = xmlNewTextChild(currentNodePtr, NULL, (xmlChar *) child, (xmlChar *) childContent);
            if(optPropertyName != NULL && optPropertyValue != NULL)
            {
                xmlNewProp(currentNodePtr,BAD_CAST optPropertyName,BAD_CAST optPropertyValue);
            }
            break;
        }
        currentNodePtr = currentNodePtr->next;
    }

}


void deleteField(xmlDocPtr docPtr, xmlNodePtr currentNodePtr, char *fieldText)
{
    while (currentNodePtr != NULL)
    {
        if ((!xmlStrcmp(xmlNodeListGetString(docPtr, currentNodePtr->xmlChildrenNode, 1), (xmlChar *) fieldText)))
        {
            xmlUnlinkNode(currentNodePtr);
        }
        currentNodePtr = currentNodePtr->next;
    }
}

int changeField(xmlNodePtr currentNodePtr, char *nodeName, char *newContent)
{
    while (currentNodePtr != NULL)
    {
        if ((!xmlStrcmp(currentNodePtr->name, (const xmlChar *) nodeName)))
        {
            xmlNodeSetContent(currentNodePtr,(xmlChar *) newContent);
            return DB_RETURN_SUCCES;
        }
        currentNodePtr = currentNodePtr->next;
    }
    return DB_RETURN_DOESNOTEXIST;
}

int changeFieldInFile(char *fileType, char *filename , char *fieldname, char *newContent)
{
    xmlDocPtr docPtr;
    xmlNodePtr currentNodePtr;
    int succes;
    char *docname = (char *) malloc(DB_DOCNAMEMEMORYSPACE);
    sprintf(docname, "%s%ss/%s.xml", DB_DBLOCATION,fileType, filename);

    if ((docPtr = openDoc(docname)) == NULL)
    {
        return DB_RETURN_FILENOTFOUND;
    }

    if ((currentNodePtr = checkDoc(docPtr, fileType)) == NULL)
    {
        return DB_RETURN_CORRUPTFILE;
    }

    succes = changeField(currentNodePtr,fieldname, newContent);
    xmlSaveFormatFile(docname, docPtr, DB_XML_FORMAT);
    xmlFreeDoc(docPtr);
    free(docname);
    return succes;
}
void addToListFile(char* itemType,char* newItem)
{
    xmlDocPtr docPtr;
    xmlNodePtr currentNodePtr;
    char *docname = (char *) malloc(DB_DOCNAMEMEMORYSPACE);
    char *doctype = (char *) malloc(50);//TODO: magic number ?make a define for this?
    sprintf(doctype,"%ss", itemType);
    sprintf(docname, "%s%slist.xml", DB_DBLOCATION,itemType);

    if ((docPtr = openDoc(docname)) == NULL)
    {
        return;
    }

    if ((currentNodePtr = checkDoc(docPtr, doctype)) == NULL)
    {
        return;
    }

    currentNodePtr = currentNodePtr->parent;
    addChild(currentNodePtr, doctype, itemType, newItem, NULL, NULL);
    xmlSaveFormatFile(docname, docPtr, DB_XML_FORMAT);
    xmlFreeDoc(docPtr);
    free(docname);
    free(doctype);
}

void assignFieldInFile(char *fileType, char *filename , char *fieldname, char *newContent)
{
    if (changeFieldInFile(fileType, filename, fieldname, newContent) == DB_RETURN_DOESNOTEXIST)
    {
        addFieldToFile(fileType, filename, fieldname, newContent);
    }
}