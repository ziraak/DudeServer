#ifndef DudeServer_PROTOCOLDEFINES_H
#define DudeServer_PROTOCOLDEFINES_H

#define RPL_SUCCESS 50 // Default success message
#define RPL_CONNECTED 100 // CONNECT
#define RPL_LOGIN 300 // LOGIN
#define RPL_AWAY 301 // PRIVMSG
#define RPL_TOPIC 303 // JOIN

#define ERR_NOSUCHNICK 401
#define ERR_NOSUCHCHANNEL 403
#define ERR_CANNOTSENDTOCHAN 404
#define ERR_NORECIPIENT 411
#define ERR_NOTEXTTOSEND 412
#define ERR_UNKNOWNCOMMAND 421
#define ERR_NICKNAMEINUSE 433
#define ERR_NOLOGIN 444
#define ERR_NEEDMOREPARAMS 461
#define ERR_CHANNELISFULL 471
#define ERR_BANNEDFROMCHAN 474
#define ERR_BADCHANNELKEY 475
#define ERR_BADCHANMASK 476
#define ERR_USERNAMEINUSE 580
#define ERR_PASSWORDTOOSHORT 581

#endif //DudeServer_PROTOCOLDEFINES_H
