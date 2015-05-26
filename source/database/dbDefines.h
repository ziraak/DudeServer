#ifndef DUDESERVER_DBDEFINES_H
#define DUDESERVER_DBDEFINES_H

#define DB_MAXMESSAGES 100
#define DB_DOCNAMEMEMORYSPACE 500
#define DB_XML_FORMAT 0
//1 is indent, 0 is no indent
#define DB_XML_ENCODING "UTF-8"


#define DB_VISIBLE_TRUE "y"
#define DB_VISIBLE_FALSE "n"

//returns:
#define DB_RETURN_FILENOTFOUND -2
#define DB_RETURN_DOESNOTEXIST -1
#define DB_RETURN_CORRUPTFILE -3
#define DB_RETURN_NULLPOINTER -666
#define DB_RETURN_SUCCES 0
#define DB_RETURN_ALREADYEXISTS -4


//file locations
#define DB_DBLOCATION "xml/"
#define DB_CHANNELLOCATION "xml/channels/"
#define DB_CHANNELLISTLOCATION "xml/channellist.xml"
#define DB_USERLISTLOCATION "xml/userlist.xml"
#define DB_USERLOCATION "xml/users/"

#endif //DUDESERVER_DBDEFINES_H
