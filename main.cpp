#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "server.h"

int main(int argc, char* argv[])
{
    std::string ip = "127.0.0.1";
    if (argc < 2) {
        std::cout << "Please specify ip which will be used by server: " << std::endl;
        std::cin >> ip;
    }
    else{
        ip = argv[1];
    }
    try{
        std::cout << "IP is " << ip << std::endl;
        boost::asio::io_service io_service;
        Server server(io_service, ip);
        io_service.run();
    }
    catch (std::exception& e){
        std::cerr << e.what() << std::endl;
    }
    return 0;
}