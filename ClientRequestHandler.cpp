//
// Created by maxim on 22.12.18.
//

#include "ClientRequestHandler.h"
#include "util/Util.h"
#include "lib/http_request_parser.h"

void ClientRequestHandler::processRequest(Request request) {
    if (NULL == observer) {
        logger.warn("use client handler without observer");
        throw ClientHandlerException();
    }
    run();
}

void ClientRequestHandler::setObserver(Observer<RequestData> *observer) {
    this->observer = observer;
}

ClientRequestHandler::ClientRequestHandler(Request request) :
        observer(NULL),
        request(request),
        logger(Logger()),
        isRead(true), isRunning(true), cacheEntry(NULL) {

}

void ClientRequestHandler::run() {
    size_t read_bytes_of_request = 0;
    while (isRunning) {
        if (isRead) {
            char * huy = new char[4555];
            ssize_t count_of_received_bytes = recv(request.getSocketFd(), huy, //request.getData().data(),
                                                   MAX_SIZE_OF_REQUEST - read_bytes_of_request, 0);

            if (-1 == count_of_received_bytes || 0 == count_of_received_bytes) {
                perror("");
                logger.warn("Invalid read operation. The connection will be closed");
                break;
            }

            if (sockets_util::is_finished_request(count_of_received_bytes, read_bytes_of_request,
                                                  huy)) {
                std::string handled_request = huy;


                handled_request = sockets_util::change_type_of_connection_to_close(handled_request);
                requestData.request.reserve(handled_request.length());


                std::copy(handled_request.data(), handled_request.data() + handled_request.length(),
                          std::back_inserter(requestData.request));

                try {

                    http_request_parser http_parser1(requestData.request.data());


                    url = http_parser1.get_uri();
                    host = http_parser1.get_host();


                    int request_type = http_parser1.get_request_type();
                    switch (request_type) {
                        case http_request_parser::GET_REQUEST : {
                            isRead = false;
                            requestData.host=host;
                            requestData.url=url;
                            requestData.handler = this;
                            requestData.requestAsString = requestData.request.data();
                            observer->notifyOnSuccess(requestData);
                            break;
                        }
                        default:
                            std::cerr << "There is not get request. The connection will be closed" << std::endl;
                            return;
                    }

                } catch (ExceptionBase e) {
                    logger.warn("catch exception");
                    return;
                }
            }

        } else {
            int result = cacheEntry->read_to_browser(request.getSocketFd(), requestData.request.data());

            if (result != 0) {
                logger.warn("read_to_browser error");
            }

            if (CacheEntry::DELETE_CACHE_ENTRY == result) {
                delete cacheEntry;
            }

            break;
        }

    }


}

void ClientRequestHandler::setCacheEntry(CacheEntry *cacheEntry) {
    this->cacheEntry = cacheEntry;
}
