//
// Created by NEOfr on 15.12.2018.
//

#include "Server.h"

Server::Server(int port) : port(port){
    
}

void Server::start() {
    std::string infoMessage = "server started";
    while (true){

    }
    logger.info( infoMessage );
}
