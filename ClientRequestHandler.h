//
// Created by maxim on 22.12.18.
//

#ifndef PROXY_CLIENTREQUESTHANDLER_H
#define PROXY_CLIENTREQUESTHANDLER_H


#include "RequestHandlerBase.h"
#include "exception/ClientHandlerException.h"
#include "util/logging/Logger.h"
#include "util/cache/CacheEntry.h"
#include <iostream>

class ClientRequestHandler : public RequestHandlerBase {
public:
    explicit ClientRequestHandler(Request request);

    virtual void processRequest(Request request);

    virtual void setObserver(Observer<RequestData> *observer);

    void setCacheEntry(CacheEntry* cacheEntry);

private:
    Request request;
    Logger logger;
    RequestData requestData;

    Observer<RequestData>* observer;
    bool isRead;
    bool isRunning;
    CacheEntry* cacheEntry;
    std::string host;
    std::string url;

    void run();

    const static size_t MAX_SIZE_OF_REQUEST = 2*1024;
};


#endif //PROXY_CLIENTREQUESTHANDLER_H
