//
// Created by maxim on 22.12.18.
//

#ifndef PROXY_REQUEST_H
#define PROXY_REQUEST_H


class Request {
private:
    int socketFd;
public:
    Request (int socketFd) : socketFd(socketFd){

    }
    int getSocketFd() {
        socketFd;
    }
};


#endif //PROXY_REQUEST_H
