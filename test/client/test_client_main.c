#include "test_client_main.h"

int main(int argc, char **argv)
{
    int success = getServerSocket(SERVER_PORT, SERVER_IP);

    if(success != SSL_OK)
    {
        perror("No connection!\n");
        return success;
    }

    printf("VERBONDEN MET %s:%i\n", SERVER_IP, SERVER_PORT);

    int childpid = fork();
    if (childpid == 0)
    {
        clientBusinessSend();
    }
    else
    {
        clientBusinessReceive();
    }

    return EXIT_SUCCESS;
}