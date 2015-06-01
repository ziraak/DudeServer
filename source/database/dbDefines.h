#ifndef DUDESERVER_DBDEFINES_H
#define DUDESERVER_DBDEFINES_H

#define DB_MAXMESSAGES 101
#define DB_DOCNAMEMEMORYSPACE 500

//database propertie values
#define DB_VISIBLE_TRUE "y"
#define DB_VISIBLE_FALSE "n"

#define USER_ROLE_OPERATOR "-o"
#define USER_ROLE_USER "-u"

//returns:
#define DB_RETURN_FILENOTFOUND -2
#define DB_RETURN_DOESNOTEXIST -1
#define DB_RETURN_CORRUPTFILE -3
#define DB_RETURN_NULLPOINTER -666
#define DB_RETURN_SUCCES 0
#define DB_RETURN_ALREADYEXISTS -4
#define DB_RETURN_ISALREADYINCHANNEL -5


//file locations
#define DB_DBLOCATION "xml/"
#define DB_CHANNELLOCATION "xml/channels/"
#define DB_CHANNELLISTLOCATION "xml/channellist.xml"
#define DB_USERLISTLOCATION "xml/userlist.xml"
#define DB_USERLOCATION "xml/users/"

//sql defines by sjuul

#define ALL_COLUMNS "*"

#define SETUP_ERROR_RETURN(val, err) printf("ERROR: %s\n", err); sqlite3_close(db); return val

#define STMT_RETURN(val, stmt) sqlite3_finalize(stmt); return val



#endif //DUDESERVER_DBDEFINES_H
