//
// Created by osboxes on 20/04/15.
//


xmlDocPtr openDoc(char *docname);

xmlNodePtr checkDoc(xmlDocPtr doc, char *docType);

char *getValue(xmlDocPtr doc, xmlNodePtr node, char *fieldname);

char **getListOfValues(xmlDocPtr doc, xmlNodePtr node, char *listname, char *fieldname);

void mainMedium()
{
    int index;

    channelInfo batcave;
    getChannel("eigendunk");
    getChannel("bier");
    batcave = getChannel("batcave");

    printf("channelname is:%s\n", batcave.name);
    index = 0;
    while (batcave.users[index] != NULL)
    {
        printf("user %i : %s\n", index + 1, batcave.users[index]);
        index++;
    }
    index = 0;
    while (batcave.messages[index].body != NULL)
    {
        printf("----------------------------------- \n message number %i\n", index + 1);
        printf("written by: %s\n", batcave.messages[index].writer);
        printf("written on: %s\n", batcave.messages[index].timestamp);
        printf("message:\n %s\n----------------------------------- \n", batcave.messages[index].body);
        index++;
    }


    userInfo fatih;

    getUser("joe");
    getUser("patrick");
    fatih = getUser("fatih");


    printf("username is:%s\n", fatih.username);
    printf("nickname is:%s\n", fatih.nickname);
    printf("password is:%s\n", fatih.password);

    index = 0;
    while (fatih.channels[index] != NULL)
    {
        printf("channel%i : %s\n", index, fatih.channels[index]);
        index++;
    }
/*
    checkUser("joe");
    checkUser("desmond");
    checkChannel("batcave");
    checkChannel("eigendunk");*/

}

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
        fprintf(stderr, "document of the wrong type, this is not a channel\n");
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
    key = malloc(50000);
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

channelInfo getChannel(char *channelName)
{
    char *docname;
    channelInfo channel;
    messageInfo message;
    xmlDocPtr doc;
    xmlNodePtr cur;

    if (checkChannel(channelName) == EXIT_FAILURE)
    {
        fprintf(stderr, "channel: %s does not exist\n", channelName);
        return channel;
    }

    docname = (char *) malloc(500);
    sprintf(docname, "database/channels/%s.xml", channelName);

    doc = openDoc(docname);
    if (doc == NULL)
    {
        return channel;
    }
    cur = checkDoc(doc, "channel");
    if (cur == NULL)
    {
        return channel;
    }

    channel.name = getValue(doc, cur, "name");
    channel.users = getListOfValues(doc, cur, "users", "user");

    int index;
    index = 0;
    while (cur != NULL)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "messages")))
        {
            xmlNodePtr curChild;
            curChild = cur->xmlChildrenNode;
            while (curChild != NULL)
            {
                if ((!xmlStrcmp(curChild->name, (const xmlChar *) "message")))
                {

                    message.writer = (char *) xmlGetProp(curChild, (xmlChar *) "user");
                    message.timestamp = getValue(doc, curChild->xmlChildrenNode, "timestamp");
                    message.body = getValue(doc, curChild->xmlChildrenNode, "body");
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

char **getUserList()
{
    char *docname;
    docname = "database/userlist.xml";

    xmlDocPtr doc;
    xmlNodePtr cur;
    char **list;

    doc = openDoc(docname);
    cur = checkDoc(doc, "users");
    cur = cur->parent;

    list = getListOfValues(doc, cur, "users", "user");
    return list;
}

char **getChannelList()
{
    char *docname;
    docname = "database/channelList.xml";

    xmlDocPtr doc;
    xmlNodePtr cur;
    char **list;

    doc = openDoc(docname);
    cur = checkDoc(doc, "channels");
    cur = cur->parent;

    list = getListOfValues(doc, cur, "channels", "channel");
    return list;
}


userInfo getUser(char *username)
{
    char *docname;
    userInfo user;
    xmlDocPtr doc;
    xmlNodePtr cur;


    if (checkUser(username) == EXIT_FAILURE)
    {
        fprintf(stderr, "user: %s does not exist\n", username);
        return user;
    }

    docname = (char *) malloc(500);

    sprintf(docname, "database/users/%s.xml", username);

    doc = openDoc(docname);
    if (doc == NULL)
    {
        return user;
    }
    cur = checkDoc(doc, "user");
    if (cur == NULL)
    {
        return user;
    }


    user.username = malloc(30);
    user.nickname = malloc(30);
    user.password = malloc(30);
    user.channels = malloc(1000);


    strcpy(user.username, username);
    strcpy(user.nickname, getValue(doc, cur, "nickname"));
    strcpy(user.password, getValue(doc, cur, "password"));
    user.channels = getListOfValues(doc, cur, "channels", "channel");
    xmlFreeDoc(doc);
    return user;
}

int checkUser(char *userName)
{
    char **userList;
    userList = getUserList();
    int lijstIndex;
    lijstIndex = 0;
    while (userList[lijstIndex])
    {
        if (!strcmp(userList[lijstIndex], userName))
        {
            //printf("user: %s found it was : %s \n",userName,userList[lijstIndex]);
            return EXIT_SUCCESS;
        }
        lijstIndex++;
    }
    //printf("%s not found\n",userName);
    return EXIT_FAILURE;
}

int checkChannel(char *channelName)
{
    char **channelList;
    channelList = getChannelList();
    int lijstIndex;
    lijstIndex = 0;
    while (channelList[lijstIndex])
    {
        if (!strcmp(channelList[lijstIndex], channelName))
        {
            // printf("channel: %s found it was : %s \n",channelName, channelList[lijstIndex]);
            return EXIT_SUCCESS;
        }
        lijstIndex++;
    }
    //printf("%s not found\n", channelName);
    return EXIT_FAILURE;
}
