//
// Created by maxim on 22.12.18.
//

#ifndef PROXY_REQUESTHANDLER_H
#define PROXY_REQUESTHANDLER_H


#include "Observer.h"
#include "RequestData.h"
#include "Request.h"

class RequestHandler {
public:
    virtual void processRequest( Request request ) = 0;
    virtual void setObserver(Observer<RequestData> *observer ) = 0;
    virtual ~RequestHandler(){}
protected:
    RequestHandler() {}
};


#endif //PROXY_REQUESTHANDLER_H
