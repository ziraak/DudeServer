//
// Created by osboxes on 19/05/15.
//

#include "communication.h"
#include "communicationStructs.h"

void sslInitialize()
{
    SSL_load_error_strings();
    SSL_library_init();
    OPENSSL_add_all_algorithms_conf();
}

void sslDestroy()
{
    ERR_free_strings();
    EVP_cleanup();
}

void sslClose()
{
    if(connection == NULL || connection->ssl_handle == NULL)
    {
        return;
    }

    SSL_shutdown(connection->ssl_handle);
    SSL_free(connection->ssl_handle);
    close(connection->socket);
}

struct sockaddr_in getSocketAddress(char* ip, uint16_t port)
{
    struct sockaddr_in address;
    bzero((char *)&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);
    address.sin_port = htons(port);

    return address;
}

int getListeningSocket(char *ip, uint16_t port)
{
    sslInitialize();
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if(sock < 0)
    {
        return SSL_NO_SOCKET_CREATED;
    }

    struct sockaddr_in server_address = getSocketAddress(ip, port);

    if(bind(sock, (struct sockaddr *) &server_address, sizeof(server_address)) < 0)
    {
        return SSL_NO_BIND;
    }

    listen(sock, 500);

    return sock;
}

int sslLoadPrivateKey(SSL_CTX *ssl_context, char* location)
{
    if(SSL_CTX_use_PrivateKey_file(ssl_context, location, SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        return SSL_PRIVATE_KEY_LOAD;
    }

    if(!SSL_CTX_check_private_key(ssl_context))
    {
        ERR_print_errors_fp(stderr);
        return SSL_PRIVATE_KEY_CHECK;
    }

    return SSL_OK;
}

int sslLoadCertificate(SSL_CTX *ssl_context, char* location)
{
    if(SSL_CTX_use_certificate_file(ssl_context, location, SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        return SSL_CERTIFICATE_ERROR;
    }

    return SSL_OK;
}

int sslAcceptConnection(int listenSocket)
{
    struct sockaddr_in clientAddress;
    socklen_t client_address_size = sizeof(clientAddress);
    int clientSocket = accept(listenSocket, (struct sockaddr *)&clientAddress, &client_address_size);

    if(clientSocket < 0)
    {
        return SSL_NO_TCP_ACCEPT;
    }

    SSL_CTX *sslContet = SSL_CTX_new(SSLv23_server_method());

    if(sslContet == NULL)
    {
        SSL_ERROR_RETURN(SSL_NO_SSL_CONTEXT);
    }

    int sslCertificateLoad = sslLoadCertificate(sslContet, SSL_CERTIFICATE_LOCATION);
    if(sslCertificateLoad != SSL_OK)
    {
        SSL_ERROR_RETURN(sslCertificateLoad);
    }

    int sslPrivateKeyLoad = sslLoadPrivateKey(sslContet, SSL_PRIVATE_KEY_LOCATION);
    if(sslPrivateKeyLoad != SSL_OK)
    {
        SSL_ERROR_RETURN(sslPrivateKeyLoad);
    }

    SSL *sslHandle = SSL_new(sslContet);
    if(sslHandle == NULL)
    {
        SSL_ERROR_RETURN(SSL_NO_SSL_HANDLE);
    }

    if(SSL_set_fd(sslHandle, clientSocket) == 0)
    {
        SSL_ERROR_RETURN(SSL_NO_SSL_HANDLE_SOCKET_LINK);
    }

    if(SSL_accept(sslHandle) < 0)
    {
        SSL_ERROR_RETURN(SSL_NO_SSL_ACCEPT);
    }

    ssl_connection conn = {
            .socket = clientSocket,
            .ssl_handle = sslHandle,
            .ssl_context = sslContet,
            .address = clientAddress
    };

    connection = &conn;

    return SSL_OK;
}

int sslSendInteger(int message)
{
    char *dest = malloc(3);
    sprintf(dest, "%i", message);
    int result = sslSend(dest);
    free(dest);

    return result;
}

int sslSend(char* snd)
{
    size_t bufferLength;
    if(connection == NULL || connection->ssl_handle == NULL || snd == NULL || (bufferLength = strlen(snd)) == 0)
    {
        return SSL_NO_CONNECTION;
    }

    char* buffer = malloc(sizeof(char) * bufferLength + 2);
    sprintf(buffer, "%s\r\n", snd);
    int written = SSL_write(connection->ssl_handle, buffer, (int)bufferLength + 2);

    if(written > 0)
    {
        return SSL_OK;
    }
    else if(written == 0)
    {
        return SSL_CONNECTION_CLOSED;
    }

    SSL_ERROR_RETURN(SSL_CONNECTION_ERROR);
}


int sslRead(char* buffer, int bufferLength)
{
    if(connection == NULL || connection->ssl_handle == NULL)
    {
        return SSL_NO_CONNECTION;
    }

    int read = SSL_read(connection->ssl_handle, buffer, bufferLength);

    if(read > 0)
    {
        return SSL_OK;
    }
    else if(read == 0)
    {
        return SSL_CONNECTION_CLOSED;
    }

    SSL_ERROR_RETURN(SSL_CONNECTION_ERROR);
}

//void sendMessage(int sockfd, char *buffer)
//{
//    size_t bufferLength;
//    if(connection == NULL || connection->ssl_handle == NULL || sockfd < 0 || buffer == NULL || (bufferLength = strlen(buffer)) == 0)
//    {
//        return;
//    }
//
//    char* snd = malloc(sizeof(char) * bufferLength + 2);
//    sprintf(snd, "%s\r\n", buffer);
//    ssize_t sendResult = send(sockfd, snd, bufferLength + 2, 0);
//
//    struct timespec remaining;
//    remaining.tv_nsec = sendWait.tv_nsec = 500;
//    remaining.tv_sec = sendWait.tv_sec;
//
//    nanosleep(&sendWait, &remaining);
//    free(snd);
//
//    if (sendResult < 0)
//    {
//        perror("Error while sending a message to the client.");
//        exit(EXIT_FAILURE);
//    }
//}
