//
// Created by osboxes on 20/04/15.
//

#include "medium.h"


void mainMedium() {
    userinfo fatih;
    fatih = getUser("desmond");


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


userinfo getUser(char *username) {
    char *docname;
    userinfo user;

    user.username = malloc(30);
    user.nickname = malloc(30);
    user.password = malloc(30);
    user.channels = malloc(1000);
    docname = (char *) malloc(500);
    sprintf(docname,"database/users/%s.xml",username);
    printf("opening :%s \n", docname);

    xmlDocPtr doc;
    xmlNodePtr cur;

    doc = xmlParseFile(docname);

    if (doc == NULL) {
        fprintf(stderr, "Document not parsed successfully. \n");
        return user;
    }

    cur = xmlDocGetRootElement(doc);

    if (cur == NULL) {
        fprintf(stderr, "empty document\n");
        xmlFreeDoc(doc);
        return user;
    }

    if (xmlStrcmp(cur->name, (const xmlChar *) "user")) {
        fprintf(stderr, "document of the wrong type, root node != user");
        xmlFreeDoc(doc);
        return user;
    }

    cur = cur->xmlChildrenNode;

    xmlChar *key;
    strcpy(user.username,username);

    while (cur != NULL) {
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "nickname"))) {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
           // printf("gevonden: %s\n", key);
            strcpy(user.nickname,(char *)key);
            xmlFree(key);
        } else if ((!xmlStrcmp(cur->name, (const xmlChar *) "password"))) {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            //printf("gevonden: %s\n", key);
            strcpy(user.password,(char *)key);
            xmlFree(key);
        } else if ((!xmlStrcmp(cur->name, (const xmlChar *) "channels"))) {
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

/*
void parseStory(xmlDocPtr doc, xmlNodePtr cur) {

    xmlChar *key;
    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "keyword"))) {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            printf("keyword: %s\n", key);
            xmlFree(key);
        }
        cur = cur->next;
    }
    return;
}*/
