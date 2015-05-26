//
// Created by osboxes on 21/05/15.
//

#ifndef DUDESERVER_COMMUNICATIONSTRUCTS_H
#define DUDESERVER_COMMUNICATIONSTRUCTS_H

#include <openssl/ossl_typ.h>

typedef struct {
    int socket;
    struct sockaddr_in address;
    SSL_CTX *ssl_context;
    SSL *ssl_handle;
} sslConnection;

#endif //DUDESERVER_COMMUNICATIONSTRUCTS_H
