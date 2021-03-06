//
// Created by maxim on 22.12.18.
//

#ifndef PROXY_OBSERVER_H
#define PROXY_OBSERVER_H

#include <string>

template <typename SuccessDataType = void* >
class Observer {
public:
    virtual void notifyOnSuccess( SuccessDataType data ) = 0;
    virtual void eraseCache(std::string s) = 0;
    virtual ~Observer() {}
};


#endif //PROXY_OBSERVER_H
