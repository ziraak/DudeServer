#ifndef DudeServer_PROTOCOLDEFINES_H
#define DudeServer_PROTOCOLDEFINES_H

#define RPL_SUCCESS 50 // Default success message
#define RPL_NOREPLY 51 // No reply message
#define RPL_JOIN_CHANNEL 52
#define RPL_PART_CHANNEL 53
#define RPL_PRIV_MSG 54

#define RPL_CONNECTED 100 // CONNECT
#define RPL_LOGIN 300 // LOGIN
#define RPL_AWAY 301 // PRIVMSG
#define RPL_NOTOPIC 331
#define RPL_TOPIC 332 // JOIN
#define RPL_INVITING 341 // INVITE
#define RPL_NAMREPLY 353 // NAMES

#define ERR_NOSUCHNICK 401
#define ERR_NOSUCHCHANNEL 403
#define ERR_CANNOTSENDTOCHAN 404
#define ERR_NORECIPIENT 411
#define ERR_NOTEXTTOSEND 412
#define ERR_UNKNOWNCOMMAND 421
#define ERR_NICKNAMEINUSE 433
#define ERR_NOTONCHANNEL 442
#define ERR_NOLOGIN 444
#define ERR_NEEDMOREPARAMS 461
#define ERR_CHANNELISFULL 471
#define ERR_UMODEUNKNOWNFLAG 472
#define ERR_INVITEONLYCHAN 473
#define ERR_BANNEDFROMCHAN 474
#define ERR_BADCHANNELKEY 475
#define ERR_BADCHANMASK 476
#define ERR_CHANOPPRIVSNEEDED 482
#define ERR_USERNAMEINUSE 580
#define ERR_PASSWORDTOOSHORT 581
#define ERR_USERNAME_NOT_KNOWN 582
#define ERR_ALREADY_LOGGED_IN 583

#endif //DudeServer_PROTOCOLDEFINES_H
