//
// Created by NEOfr on 15.12.2018.
//

#include <unistd.h>
#include "Server.h"
#include "util/logging/Logger.h"
#include "ClientRequestHandler.h"

Server::Server(int port) :
        port(port),
        is_running(false),
        MAX_COUNT_OF_PENDING_REQUESTS(100),
        BIND_IP("127.0.0.1"){

    this->socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if( socketFd==-1 ){
        throw ExceptionBase();
    }
    int flag = 1;
    int is_socket_became_reusing = setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));
    if (-1 == is_socket_became_reusing) {
        throw ExceptionBase();
    }
    struct sockaddr_in sockaddr_in1;
    sockaddr_in1.sin_family = AF_INET;
    sockaddr_in1.sin_port = htons(port);
    sockaddr_in1.sin_addr.s_addr = inet_addr(BIND_IP);

    int result_of_binding = bind(socketFd, (struct sockaddr *) &sockaddr_in1, sizeof(sockaddr_in1));

    if (-1 == result_of_binding) {
        throw ExceptionBase();
    }

    int result_of_listening_socket = listen(socketFd, MAX_COUNT_OF_PENDING_REQUESTS);
    if (0 != result_of_listening_socket) {
        throw ExceptionBase();
    }
}

void Server::start() {
    is_running = true;

    while (is_running){
        logger.debug("running iteration");
        sockaddr_in sockaddr_in1;
        socklen_t socklen = sizeof(sockaddr_in1);

        int new_fd = accept(socketFd, (struct sockaddr *) &sockaddr_in1, &socklen);
        if (-1 == new_fd) {
            logger.warn("error while accepting");
            continue;
        }
        Request request(new_fd);
        try {
            ClientRequestHandler handler(request);

            handler.setObserver(this);
            handler.processRequest(request);
        } catch (ClientHandlerException &exception){
            logger.warn("some error");
            close(new_fd);
        }


    }
}

void Server::notifyOnError() {
    logger.info("get error from http-client");
}

void Server::notifyOnSuccess(RequestData data) {
    logger.info("success connection");
}
