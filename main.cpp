#include <iostream>
#include "Logger.h"
#include "Server.h"

int main() {
    Server server = Server(16000);
    server.start();
}