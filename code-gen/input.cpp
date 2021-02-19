#include <iostream>
#include <fstream>
#include "node.h"
#include "codegen.h"
extern FILE *yyin;

int input(int argc, char **argv)
{
    if (argc > 1)
    {
        if (!(yyin = fopen(argv[1], "r")))
        {
            perror(argv[1]);
            return 1;
        }
        printf("[I]>>>Read data from file:%s\n", argv[1]);
    }
    else
    {
        printf("[I]>>>Read data from stdin.\n");
    }
}