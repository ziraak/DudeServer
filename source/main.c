#include "main.h"
#include "pthread.h"

int test = 0;

void testSlet()
{
    printf("I slutted. You slut! Don' t steal other peoples things. Fuck mother.");
    test = 5;
    pthread_exit(NULL);
}

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
    int maxLengthBuffer = 10;
    char buffer[maxLengthBuffer];
    bzero(buffer, maxLengthBuffer);

    int pipeFd[2];
    pipe(pipeFd);
    int i = 0;

    for (i = 0; i < 10; i++)
    {
        int childpid = fork();
        if (childpid == 0)
        {
            read(pipeFd[0], buffer, maxLengthBuffer);
            printf("Child read message: %s \n", buffer);
            write(pipeFd[1], buffer, maxLengthBuffer);
            bzero(buffer, maxLengthBuffer);
            exit(0);
        }
    }

    write(pipeFd[1], "Broadcast", 9);

    if ()
    {

    }
    //write(pipeFd[1], "test", 4);
    printf("Parent done.\n");


     /*
    size_t maxLengthBuffer = 10;
    char buffer[maxLengthBuffer];
    bzero(buffer, maxLengthBuffer);

    int aantalChilderen = 0;
    int *childPipeFd[2];
    int i = 0;

    for (i = 0; i < 3; i++)
    {
        childPipeFd[aantalChilderen] = malloc(20);
        pipe(childPipeFd[aantalChilderen]);
        int childpid = fork();
        if (childpid == 0)
        {
            close(childPipeFd[aantalChilderen][1]);
            read(childPipeFd[aantalChilderen][0], buffer, maxLengthBuffer);
            printf("Child read message: %s \n", buffer);
            bzero(buffer, maxLengthBuffer);

            free(childPipeFd[aantalChilderen]);
            exit(0);
        }

        char buffer1[maxLengthBuffer];
        bzero(buffer1, maxLengthBuffer);
        sprintf(buffer1, "test %i", i);

        close(childPipeFd[aantalChilderen][0]);
        write(childPipeFd[aantalChilderen][1], buffer1, 6);

        aantalChilderen++;
    }

    pthread_t tid;
    pthread_create(&tid, NULL, (void *) &testSlet, NULL);

    printf("%i \n", test);
     */

    return EXIT_SUCCESS;
}