cmake_minimum_required(VERSION 3.10.2)
project(Proxy)

set(CMAKE_CXX_STANDARD 98)
LINK_LIBRARIES(pthread)

add_executable(Proxy main.cpp Server.cpp Server.h util/logging/Logger.h util/logging/LogLevel.h RequestData.h exception/ExceptionBase.h Observer.h RequestHandler.h Request.h util/cache/Cache.h exception/CacheException.h ClientRequestHandler.cpp ClientRequestHandler.h exception/ClientHandlerException.h util/cache/CacheEntry.cpp util/cache/CacheEntry.h)