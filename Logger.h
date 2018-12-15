//
// Created by NEOfr on 15.12.2018.
//

#ifndef PROXY_LOGGER_H
#define PROXY_LOGGER_H


#include <string>
#include "LogLevel.h"


class Logger {
public:
    explicit Logger(LogLevel level);

    Logger();

    void info(std::string& mes);

    void warn(std::string& mes);

    void debug(std::string& mes);

private:
    void log(std::string mes, LogLevel level);
    LogLevel level;
};

#endif //PROXY_LOGGER_H
