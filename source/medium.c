//
// Created by osboxes on 20/04/15.
//

#include "medium.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xmlreader.h>

typedef struct currentDoc {
    xmlDocPtr document;
    xmlNodePtr node;
    int succesfull;
} currentDoc;

void getUser(char *username);
void parseStory(xmlDocPtr doc, xmlNodePtr cur);
static void parseDoc(char *docname);

typedef struct user {
    char *username;
    char *nickname;
    char *password;
    char **channels;
} user;


void mainMedium() {
    getUser("fatih");

}


void getUser(char *username) {
    user user1;
    char *document;
    currentDoc userFile;


    document = (char *) malloc(500);
    //sprintf(document,"/home/osboxes/ClionProjects/Dude/database/users/%s.xml",username);
    document = "/home/osboxes/ClionProjects/Dude/source/test.xml";
    printf("opening :%s \n", document);

    /*userFile = getDoc(document,"story");
    if(userFile.succesfull == EXIT_SUCCESS){
        strcpy(user1.username,username);
        printf("username : %s\n",user1.username);


        getField(userFile.document,userFile.node,"storyinfo");
        user1.nickname = getField(userFile.document,userFile.node,"nickname");

        printf("Nickname : %s\n",user1.nickname);
    }
    xmlFreeDoc(userFile.document);
    free(document);*/
    parseDoc(document);


}

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
}

static void parseDoc(char *docname) {

    xmlDocPtr doc;
    xmlNodePtr cur;

    doc = xmlParseFile(docname);

    if (doc == NULL) {
        fprintf(stderr, "Document not parsed successfully. \n");
        return;
    }

    cur = xmlDocGetRootElement(doc);

    if (cur == NULL) {
        fprintf(stderr, "empty document\n");
        xmlFreeDoc(doc);
        return;
    }

    if (xmlStrcmp(cur->name, (const xmlChar *) "story")) {
        fprintf(stderr, "document of the wrong type, root node != story");
        xmlFreeDoc(doc);
        return;
    }

    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "storyinfo"))) {
            parseStory(doc, cur);
        }

        cur = cur->next;
    }

    xmlFreeDoc(doc);
    return;
}
