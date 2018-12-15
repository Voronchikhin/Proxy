//
// Created by NEOfr on 15.12.2018.
//

#ifndef PROXY_UTIL_H
#define PROXY_UTIL_H


#include "Logger.h"

class Util {
public:
    static LogLevel getRootLogLevel();
private:
    const static LogLevel logLevel = DEBUG;
};

#endif //PROXY_UTIL_H
