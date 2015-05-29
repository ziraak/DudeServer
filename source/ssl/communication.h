//
// Created by osboxes on 19/05/15.
//

#ifndef DUDESERVER_COMMUNICATION_H
#define DUDESERVER_COMMUNICATION_H

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9090
#define SSL_CERTIFICATE_LOCATION "certificates/certificate.pem"
#define SSL_PRIVATE_KEY_LOCATION "certificates/private/private_key.pem"

#define SSL_ERROR_RETURN(val) ERR_print_errors_fp(stderr); return val

#define SSL_OK 0
#define SSL_NO_SOCKET_CREATED -1
#define SSL_NO_BIND -2
#define SSL_NO_TCP_ACCEPT -3
#define SSL_NO_SSL_CONTEXT -4
#define SSL_CERTIFICATE_ERROR -5
#define SSL_PRIVATE_KEY_LOAD -6
#define SSL_PRIVATE_KEY_CHECK -7
#define SSL_NO_SSL_HANDLE -8
#define SSL_NO_SSL_HANDLE_SOCKET_LINK -9
#define SSL_NO_SSL_ACCEPT -10

#define SSL_NO_CONNECTION -11
#define SSL_CONNECTION_CLOSED -12
#define SSL_CONNECTION_ERROR -13

#include "../main.h"
#include "communicationStructs.h"

#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

sslConnection connection;

void sslInitialize();
void sslDestroy();
void sslClose();

int getListeningSocket(char *ip, uint16_t port);

int sslLoadCertificate(SSL_CTX *ssl_context, char* location);
int sslAcceptConnection(int listenSocket);

int sslSendInteger(int msg);
int sslSend(char* snd);
int sslRead(char* buffer, int bufferLength);

#endif //DUDESERVER_COMMUNICATION_H
