//
// Created by desmond on 4/22/15.
//
#include "xmlWriter.h"
void addChild(xmlNodePtr cur, char* parent,char* child,char* childContent);

void addFieldToFileInList(char* fileType,char* filename, char* listname, char* fieldname, char* content);

//int writeUser(userInfo user)
//{
//    printf("User %s wordt opgeslagen", user.username);
//    xmlTextWriterPtr xmlptr = openFile(user.username);
//    if (xmlptr == NULL)
//    {
//        return 1;
//    }
//    xmlTextWriterStartElement(xmlptr, "user");
//    xmlTextWriterWriteElement(xmlptr, "nickname", user.nickname);
//    xmlTextWriterWriteElement(xmlptr, "password", user.password);
//    writeChannels(xmlptr, user.channels);
//    xmlTextWriterEndElement(xmlptr);
//    xmlTextWriterEndDocument(file);
//    return 0;
//}

xmlTextWriterPtr openFile(char *username)
{
    char filename[250];
    sprintf(filename, "database/users/%s.xml", username);
    return xmlNewTextWriterFilename(filename, 0);
}

//void writeChannels(xmlTextWriterPtr xmlptr, char **channels)
//{
//    xmlTextWriterStartElement(xmlptr, "channels");
//    while (channels != NULL && channels != 0)
//    {
//        if (*channels == NULL || *channels == 0)
//        {
//            break;
//        }
//        xmlTextWriterWriteElement(xmlptr, "channel", *channels);
//        channels++;
//    }
//    xmlTextWriterEndElement(xmlptr);
//}

void userJoinChannel(char *username, char *channelName){
    if(checkUser(username) == EXIT_FAILURE || checkChannel(channelName) == EXIT_FAILURE){
        fprintf(stderr, "user: %s or channel %s does not exist\n", username,channelName);
        return;
    }

    addFieldToFileInList("user",username,"channels","channel",channelName);
    addFieldToFileInList("channel",channelName,"users","user",username);
}



void addFieldToFileInList(char* fileType,char* filename, char* listname, char* fieldname, char* content){
    xmlDocPtr doc;
    xmlNodePtr cur;
    char* docname;
    docname = (char *) malloc(500);

    sprintf(docname, "database/%ss/%s.xml", fileType,filename);

    printf("opening document %s\n",docname);

    doc = openDoc(docname);
    if (doc == NULL)
    {
        printf("error\n");
        return;
    }

    cur = checkDoc(doc, fileType);
    if (cur == NULL)
    {
        printf("error\n");
        return;
    }
    addChild(cur,listname,fieldname,content);

    xmlSaveFormatFile (docname, doc, 0);
    xmlFreeDoc(doc);
    free(docname);
}



void addChild(xmlNodePtr cur, char* parent,char* child,char* childContent){
    while (cur != NULL) {
        if ((!xmlStrcmp(cur->name, (const xmlChar *)parent))){
            xmlNewTextChild (cur, NULL, (xmlChar* )child,(xmlChar* ) childContent);
            break;
        }
        cur = cur->next;
    }

}