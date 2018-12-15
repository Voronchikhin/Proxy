cmake_minimum_required(VERSION 3.12)
project(Proxy)

set(CMAKE_CXX_STANDARD 98)

add_executable(Proxy main.cpp Server.cpp Server.h Logger.cpp Logger.h Util.cpp Util.h LogLevel.h)