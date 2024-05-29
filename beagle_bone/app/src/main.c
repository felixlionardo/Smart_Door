// Main program to build the application
// Has main(); does initialization and cleanup of threads
#include "shutdown.h"

#include <stdio.h>
#include <stdbool.h>
#include <errno.h>

int main()
{
    createThreads();
    waitShutdown();
    joinThreads();
    
    return 0;
}


