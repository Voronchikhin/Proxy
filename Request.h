//
// Created by maxim on 22.12.18.
//

#ifndef PROXY_REQUEST_H
#define PROXY_REQUEST_H


#include <netinet/in.h>
#include <vector>
#include <iostream>

class Request {
private:
    int socketFd;
    std::vector<char> data;
    sockaddr_in sockaddr;
public:
    Request (int socketFd, sockaddr_in sockaddr) : socketFd(socketFd), sockaddr(sockaddr){
        data.reserve(2*1024);
    }
    std::vector<char> getData(){
        return data;
    }
    int getSocketFd() {
        return socketFd;
    }
};


#endif //PROXY_REQUEST_H
