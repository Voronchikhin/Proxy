//
// Created by maxim on 22.12.18.
//

#ifndef PROXY_CLIENTREQUESTHANDLER_H
#define PROXY_CLIENTREQUESTHANDLER_H


#include "RequestHandler.h"
#include "exception/ClientHandlerException.h"
#include <iostream>

class ClientRequestHandler : public RequestHandler{
public:
    explicit ClientRequestHandler(Request request) : observer(NULL){
        socketfd = request.getSocketFd();
    }

    virtual void processRequest(Request request) {
        if( NULL == observer ){
            throw ClientHandlerException();
        }
        observer->notifyOnSuccess(RequestData());
    }

    virtual void setObserver(Observer<RequestData> *observer) {
        this->observer = observer;
    }

private:
    int socketfd;
    Observer<RequestData>* observer;
};


#endif //PROXY_CLIENTREQUESTHANDLER_H
