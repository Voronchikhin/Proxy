cmake_minimum_required(VERSION 3.10.2)
project(Proxy)

set(CMAKE_CXX_STANDARD 98)

add_executable(Proxy main.cpp Server.cpp Server.h util/logging/Logger.h util/logging/LogLevel.h exception/ExceptionBase.cpp RequestData.h exception/ExceptionBase.h Observer.h RequestHandler.cpp RequestHandler.h Request.h)