//
// Created by maxim on 22.12.18.
//

#ifndef PROXY_REQUESTDATA_H
#define PROXY_REQUESTDATA_H



#include <vector>

struct RequestData {
    std::string host;
    std::string url;
    void* handler;
    std::vector<char> request;
    std::string requestAsString;
};


#endif //PROXY_REQUESTDATA_H
