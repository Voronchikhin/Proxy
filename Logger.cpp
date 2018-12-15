//
// Created by NEOfr on 15.12.2018.
//

#include <iostream>
#include "Logger.h"
#include "Util.h"

void Logger::info(std::string &mes) {
    this->log(mes, INFO);
}

void Logger::warn(std::string &mes) {
    this->log(mes, WARN);
}

void Logger::debug(std::string &mes) {
    this->log(mes, DEBUG);
}

void Logger::log(std::string mes, LogLevel level) {
    if( level <= this->level ){
        std::cout<<mes<<std::endl;
    }
}

Logger::Logger(LogLevel level) : level(level) {}

Logger::Logger() : level(Util::getRootLogLevel()) {}
