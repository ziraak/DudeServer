#include "main.h"

#define PRINT_CHANNEL(c) printf("%s: %s | %s | %i\n", c.name, c.password, c.topic, c.visible)
#define PRINT_USER(u) printf("%s: %s | %s\n", u.username, u.password, u.nickname)

int main(int argc, char **argv)
{
    if(setupDatabaseConnection() != DB_RETURN_SUCCES)
    {
        return EXIT_FAILURE;
    }

    int fork = BOOL_FALSE;
    if(argc > 1)
    {
        if(strcmp(argv[1], "FORK") == 0 || strcmp(argv[1], "fork") == 0 || strcmp(argv[1], "-f") == 0)
        {
            fork = BOOL_TRUE;
        }
    }

    runServer(fork, SERVER_PORT);
    stopDatabase();

    //commandStruct cmd = commandStruct_initialize("NAMES batcave");
    //handleNamesCommand(cmd);

    return EXIT_SUCCESS;
}