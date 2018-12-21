//
// Created by NEOfr on 15.12.2018.
//

#ifndef PROXY_SERVER_H
#define PROXY_SERVER_H

#include "Server.h"
#include "exception/ExceptionBase.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "util/logging/Logger.h"
#include "Observer.h"
#include "RequestData.h"
#include "util/cache/Cache.h"
#include "util/cache/CacheEntry.h"

class Server : public Observer<RequestData> {
public:

    void notifyOnSuccess( RequestData data );
    void notifyOnError();

    explicit Server(int port);
    void start();
private:
    int port;
    int socketFd;
    Logger logger;
    const char *BIND_IP;
    const int MAX_COUNT_OF_PENDING_REQUESTS;

    Cache<std::string, CacheEntry > cache;

    bool is_running;
};

#endif //PROXY_SERVER_H
