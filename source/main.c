#include "main.h"

int main(int argc, char **argv)
{
//    mallopt(M_CHECK_ACTION, 1);
//
//    int fork = BOOL_TRUE;
//    if(argc > 1)
//    {
//        if(strcmp(argv[1], "FORK") == 0 || strcmp(argv[1], "fork") == 0 || strcmp(argv[1], "-f") == 0)
//        {
//            fork = BOOL_TRUE;
//        }
//    }
//
//    runServer(fork, SERVER_PORT);
    messageInfo info;
    info.writer = "hallo";
    info.body = "doei";
    info.timestamp = "23186";
    writeMessageToChannel("batcave",info);
//    writeMessageToChannel("batcave",info);
//    writeMessageToChannel("batcave",info);
//    writeMessageToChannel("batcave",info);

    return EXIT_SUCCESS;
}