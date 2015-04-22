//
// Created by osboxes on 20/04/15.
//

#include "medium.h"

void getChannel(char *channelName);


void mainMedium() {
    getChannel("batcave");



    userinfo fatih;
    fatih = getUser("fatih");


    printf("username is:%s\n",fatih.username);
    printf("nickname is:%s\n",fatih.nickname);
    printf("password is:%s\n",fatih.password);
    int index;
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
    int i;
    i=0;
    while (cur != NULL) {
        if ((!xmlStrcmp(cur->name, (const xmlChar *) listname))) {
            key[i] = getValue(doc,  cur->xmlChildrenNode,fieldname);
            i++;
        }
        cur = cur->next;
    }
    return key;
}

void getChannel(char *channelName){
    char *docname;
    docname = (char *) malloc(500);
    sprintf(docname,"database/channels/%s.xml",channelName);
    printf("opening :%s \n", docname);

    xmlDocPtr doc;
    xmlNodePtr cur;

    doc = openDoc(docname);
    cur = checkDoc(doc,"channel");

    printf("channel name: %s\n", getValue(doc,cur,"name"));
  
    xmlChar *key;
    while (cur != NULL){
         if ((!xmlStrcmp(cur->name, (const xmlChar *) "users"))) {
            xmlNodePtr curChild;
            curChild = cur->xmlChildrenNode;
            printf("users:\n");
            while (curChild != NULL) {
                if ((!xmlStrcmp(curChild->name, (const xmlChar *) "user"))) {
                    key = xmlNodeListGetString(doc, curChild->xmlChildrenNode, 1);
                    printf("user: %s\n", key);
                }
                curChild = curChild->next;
            }

        }  else if ((!xmlStrcmp(cur->name, (const xmlChar *) "messages"))) {
            xmlNodePtr curChild;
            curChild = cur->xmlChildrenNode;
            printf("messages:\n");
            while (curChild != NULL) {
                if ((!xmlStrcmp(curChild->name, (const xmlChar *) "message"))) {
                    key = xmlGetProp(curChild,"user");
                    printf("message send by: %s\n", key);

                    xmlNodePtr curGrandChild;
                    curGrandChild = curChild->xmlChildrenNode;
                    while(curGrandChild != NULL){
                        if ((!xmlStrcmp(curGrandChild->name, (const xmlChar *) "timestamp"))){
                            key = xmlNodeListGetString(doc, curGrandChild->xmlChildrenNode, 1);
                            printf("timestamp: %s\n", key);
                        }
                        if ((!xmlStrcmp(curGrandChild->name, (const xmlChar *) "body"))){
                            key = xmlNodeListGetString(doc, curGrandChild->xmlChildrenNode, 1);
                            printf("message: %s\n", key);
                        }
                        curGrandChild= curGrandChild->next;
                    }
                    printf("------------------\n\n");

                }
                curChild = curChild->next;
            }
        }

        cur = cur->next;

    }

    xmlFreeDoc(doc);
    return;
}

userinfo getUser(char *username) {
    char *docname;
    userinfo user;

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


    xmlChar *key;
    while (cur != NULL) {
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "channels"))) {
            xmlNodePtr curChild;
            curChild = cur->xmlChildrenNode;
            int i;
            i = 0;
            while (curChild != NULL) {
                if ((!xmlStrcmp(curChild->name, (const xmlChar *) "channel"))) {
                    key = xmlNodeListGetString(doc, curChild->xmlChildrenNode, 1);
                    //printf("channel: %s\n", key);
                    //user.channels[i] = malloc(20);
                    user.channels[i] = (char *)key;

                    i++;
                }
                curChild = curChild->next;
            }

        }

        cur = cur->next;
    }

    xmlFreeDoc(doc);
    return user;
}