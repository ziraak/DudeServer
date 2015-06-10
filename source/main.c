#include "main.h"

int main(int argc, char **argv)
{
    lastTimestamp = 0;
    if(setupDatabaseConnection() != DB_RETURN_SUCCES)
    {
        return EXIT_FAILURE;
    }

    int fork = BOOL_TRUE;
    int port = SERVER_PORT;
    int i;
    for(i = 1; i < argc; i++)
    {
        if(strcmp(argv[i], "FORK") == 0 || strcmp(argv[i], "fork") == 0 || strcmp(argv[i], "-f") == 0)
        {
            fork = BOOL_TRUE;
        }
        if(strcmp(argv[i], "PORT") == 0 || strcmp(argv[i], "port") == 0 || strcmp(argv[i], "-p") == 0)
        {
            if(argc >= i + 1)
            {
                port = atoi(argv[i + 1]);
                i++;
            }
        }
    }

    runServer(fork, port);
    stopDatabase();
    /*

    int pipeFd[2], pipeFd2[2];
    int maxLengthBuffer = 10;
    char buffer[maxLengthBuffer];

    bzero(buffer, 10);
    pipe(pipeFd);
    pipe(pipeFd2);

    int childpid = fork();
    if (childpid == 0)
    {
        write(pipeFd[1], "test", 4);
        read(pipeFd2[0], buffer, maxLengthBuffer);
        write(pipeFd[1], "whatsup", 7);
        write(pipeFd[1], "hey", 3);
        printf("Child1 read message: %s \n", buffer);
        exit(0);
    }

    write(pipeFd2[1], "Received", 8);


    while (read(pipeFd[0], buffer, maxLengthBuffer) > 0 && buffer[0] != '\0')
    {
        printf("Final boss read message: %s \n", buffer);
        bzero(buffer, maxLengthBuffer);
    }

    */

    return EXIT_SUCCESS;
}