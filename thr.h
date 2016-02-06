#ifndef THR_H
#define THR_H

#include <string>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <iostream>
#include <semaphore.h>
#include <csignal>
#include <sys/socket.h>
#include <netinet/in.h>
#include "server.h"

using namespace std;

void server(int port, int threadNum, string directory);

#endif
