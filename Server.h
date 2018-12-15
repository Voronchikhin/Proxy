//
// Created by NEOfr on 15.12.2018.
//

#ifndef PROXY_SERVER_H
#define PROXY_SERVER_H


#include "Logger.h"

class Server {
public:
    explicit Server(int port);
    void start();
private:
    int port;
    int socketFd;
    Logger logger;
};

#endif //PROXY_SERVER_H
