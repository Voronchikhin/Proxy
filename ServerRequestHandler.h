//
// Created by maxim on 22.12.18.
//

#ifndef PROXY_SERVERREQUESTHANDLER_H
#define PROXY_SERVERREQUESTHANDLER_H


#include <netdb.h>
#include "RequestHandlerBase.h"
#include "util/cache/CacheEntry.h"
#include "util/logging/Logger.h"
#include "util/Util.h"


class ServerRequestHandler : public RequestHandlerBase {
    int socketFd;
    CacheEntry *cache_entry1;
    Logger logger1;
    addrinfo *addrinfo_;
    Observer<RequestData> *observer;
    bool is_write;
    bool is_running;
    size_t pos_in_sending_data;
    std::string url;
    RequestData requestData;

public:
    ServerRequestHandler(addrinfo *addrinfo1, int socketFd, CacheEntry *cache_entry1,
                         RequestData requestData) :
            is_write(true),
            pos_in_sending_data(0),
            logger1(Logger()),
            cache_entry1(cache_entry1),
            url(requestData.request.data()),
            addrinfo_(addrinfo1),
            socketFd(socketFd),
            observer(NULL),
            is_running(true),
            requestData(requestData) {
    }

    void set_observer(Observer<RequestData> *observer2) {
        this->observer = observer2;
    }

    void exec() {
        int connect_result = connect(socketFd, addrinfo_->ai_addr, addrinfo_->ai_addrlen);

        logger1.debug("Connected to server successfully!");

        if (-1 == connect_result) {
            logger1.warn("Can not connect");
            observer->eraseCache("");
            cache_entry1->mark_invalid();
            return;
        }

        while (is_running) {
            if (is_write) {
                ssize_t count_of_send_data = send(socketFd,
                                                  requestData.request.data() + pos_in_sending_data,
                                                  requestData.requestAsString.length() - pos_in_sending_data, 0);

                if (-1 == count_of_send_data) {
                    logger1.warn("Error while sending data to server");
                    observer->eraseCache(url);

                    if (0 == cache_entry1->get_count_of_readers()) {
                        delete cache_entry1;
                        logger1.info(
                                "A cache record was deleted cause there have not been any clients and the record was invalid");
                        return;
                    } else {
                        cache_entry1->mark_invalid();
                    }
                    return;
                }

                pos_in_sending_data += count_of_send_data;


                if (pos_in_sending_data == requestData.requestAsString.length()) {
                    is_write = false;
                    logger1.debug("The request was successfully sent to server");
                }
            } else {
                int result = cache_entry1->read_from_server(socketFd);

                logger1.debug("The response was received from the server!");

                if (CacheEntry::DELETE_CACHE_ENTRY == result) {
                    delete cache_entry1;
                }

                if (CacheEntry::COMMON_ERROR == result) {
                    observer->eraseCache(url);
                }

                return;
            }
        }

        return;
    }

    static void* run(void* arg) {
        ServerRequestHandler *request_base1 = (ServerRequestHandler*) arg;
        request_base1->exec();
        delete request_base1;
        return NULL;
    }
    void start(){
        pthread_t thread;
        pthread_attr_t pthread_attr;
        pthread_attr_init(&pthread_attr);
        pthread_attr_setdetachstate(&pthread_attr, PTHREAD_CREATE_DETACHED);

        int result_of_thread_creating = pthread_create(&thread, &pthread_attr, run, this);

        pthread_attr_destroy(&pthread_attr);

        if (-1 == result_of_thread_creating) {
            throw ExceptionBase();
        }
    }
    virtual void processRequest(Request request){
        logger1.warn("unsoported op");
        throw ExceptionBase();
    }

    virtual void setObserver(Observer<RequestData> *observer){
        this->observer = observer;
    }
    virtual ~ServerRequestHandler() {}
};


#endif //PROXY_SERVERREQUESTHANDLER_H
