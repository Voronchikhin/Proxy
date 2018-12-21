//
// Created by NEOfr on 15.12.2018.
//

#ifndef PROXY_LOGGER_H
#define PROXY_LOGGER_H


#include <string>
#include "LogLevel.h"
#include <iostream>

class Util {
public:
    static LogLevel getRootLogLevel() {
        return Util::logLevel;
    }

private:
    const static LogLevel logLevel = DEBUG;
};

class Logger {
public:
    explicit Logger(LogLevel level) : level(level) {}

    Logger() : level(Util::getRootLogLevel()) {}

    template <typename T>
    void info(T mes);

    template <typename T>
    void warn(T mes);

    template <typename T>
    void debug(T mes);

private:
    template <typename T>
    void log(T, LogLevel level);

    LogLevel level;
};

template <typename T>
void Logger::info(T mes) {
    this->log(mes, INFO);
}

template <typename T>
void Logger::warn(T mes) {
    this->log(mes, WARN);
}

template <typename T>
void Logger::debug(T mes) {
    this->log(mes, DEBUG);
}

template <typename T>
void Logger::log(T mes, LogLevel level) {
    if( level <= this->level ){
        std::cout << mes << std::endl;
    }
}


#endif //PROXY_LOGGER_H
