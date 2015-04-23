//
// Created by osboxes on 20/04/15.
//

#include "medium.h"

channelInfo getChannel(char *channelName);



void mainMedium() {
    int index;

    channelInfo batcave;
    batcave = getChannel("batcave");
    printf("channelname is:%s\n",batcave.naam);
    index = 0;
    while(batcave.users[index] != NULL) {
        printf("user %i : %s\n",index+1,batcave.users[index]);
        index++;
    }
    index = 0;
    while(batcave.messages[index].body != NULL){
        printf("----------------------------------- \nmessage number %i\n",index+1);
        printf("written by: %s\n",batcave.messages[index].writer);
        printf("written on: %s\n",batcave.messages[index].timestamp);
        printf("message:\n %s\n----------------------------------- \n",batcave.messages[index].body);
        index++;
    }



    userInfo fatih;
    fatih = getUser("fatih");
    printf("username is:%s\n",fatih.username);
    printf("nickname is:%s\n",fatih.nickname);
    printf("password is:%s\n",fatih.password);

    index = 0;
    while(fatih.channels[index] != NULL) {
        printf("channel%i : %s\n",index,fatih.channels[index]);
        index++;
    }
}
xmlDocPtr openDoc(char* docname){
    xmlDocPtr doc;
    doc = xmlParseFile(docname);

    if (doc == NULL) {
        fprintf(stderr, "Document not parsed successfully. \n");
        return NULL;
    } else {
        return doc;
    }

}

xmlNodePtr checkDoc(xmlDocPtr doc,char * docType ){
    xmlNodePtr cur;
    cur = xmlDocGetRootElement(doc);

    if (cur == NULL) {
        fprintf(stderr, "empty document\n");
        xmlFreeDoc(doc);
        return NULL;
    }

    if (xmlStrcmp(cur->name, (const xmlChar *)docType)) {
        fprintf(stderr, "document of the wrong type, this is not a channel");
        xmlFreeDoc(doc);
        return NULL;
    }

    cur = cur->xmlChildrenNode;
    return cur;
}

char* getValue(xmlDocPtr doc, xmlNodePtr node, char* fieldname){
    xmlNodePtr cur;
    cur = node;
    xmlChar *key;

    while (cur != NULL) {
        if ((!xmlStrcmp(cur->name, (const xmlChar *) fieldname))) {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
        }
        cur = cur->next;
    }
    return (char *)key;
}

char** getListOfValues(xmlDocPtr doc,xmlNodePtr node,char* listname, char* fieldname){
    xmlNodePtr cur;
    cur = node;
    char** key;
    key = malloc(50000);
    int i;
    i=0;
    while (cur != NULL) {
        if ((!xmlStrcmp(cur->name, (const xmlChar *) listname))) {
            xmlNodePtr curChild;
            curChild = cur->xmlChildrenNode;
            while (curChild != NULL) {
                if ((!xmlStrcmp(curChild->name, (const xmlChar *) fieldname))) {
                    key[i] =(char *) xmlNodeListGetString(doc, curChild->xmlChildrenNode, 1);
                    i++;
                }
                curChild = curChild->next;
            }
        }
        cur = cur->next;
    }
    return key;
}

channelInfo getChannel(char *channelName){
    char *docname;
    channelInfo channel;
    messageInfo message;
    xmlDocPtr doc;
    xmlNodePtr cur;

    docname = (char *) malloc(500);
    sprintf(docname,"database/channels/%s.xml",channelName);

    doc = openDoc(docname);
    cur = checkDoc(doc,"channel");

    channel.naam = getValue(doc,cur,"name");
    channel.users = getListOfValues(doc,cur,"users","user");

    int index;
    index = 0;
    while (cur != NULL){
         if ((!xmlStrcmp(cur->name, (const xmlChar *) "messages"))) {
            xmlNodePtr curChild;
            curChild = cur->xmlChildrenNode;
            while (curChild != NULL) {
                if ((!xmlStrcmp(curChild->name, (const xmlChar *) "message"))) {

                    message.writer = (char *)xmlGetProp(curChild,(xmlChar *)"user");
                    message.timestamp = getValue(doc,curChild->xmlChildrenNode ,"timestamp");
                    message.body = getValue(doc,curChild->xmlChildrenNode,"body");
                    channel.messages[index] = message;
                    index++;

                }
                curChild = curChild->next;
            }
        }

        cur = cur->next;

    }

    xmlFreeDoc(doc);
    return channel;
}
void getUserList(){
    char *docname;
    docname = "database/userlist.xml";
    xmlDocPtr doc;
    xmlNodePtr cur;
    char** list;

    doc = openDoc(docname);
    cur = checkDoc(doc,"users");
    cur = cur->parent;

    list = getListOfValues(doc,cur,"users","user");

    int i;
    i = 0;
    while(list[i]!=NULL){
        printf("something: %s\n",list[i]);
        i++;
    }
}



userInfo getUser(char *username) {
    char *docname;
    userInfo user;

    user.username = malloc(30);
    user.nickname = malloc(30);
    user.password = malloc(30);
    user.channels = malloc(1000);
    docname = (char *) malloc(500);

    sprintf(docname,"database/users/%s.xml",username);


    xmlDocPtr doc;
    xmlNodePtr cur;
    doc = openDoc(docname);
    cur = checkDoc(doc,"user");

    strcpy(user.username,username);
    strcpy(user.nickname,getValue(doc,cur,"nickname"));
    strcpy(user.password,getValue(doc,cur,"password"));
    user.channels = getListOfValues(doc,cur,"channels","channel");
    xmlFreeDoc(doc);
    return user;
}