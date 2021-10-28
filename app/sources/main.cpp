#include <iostream>
#include <memory>
#include <thread>

#include "TimeManager.hpp"
#include "IGetTimeEndpoint.hpp"
#include "INetworkEndpoint.hpp"
#include "NetworkEndpoint.hpp"
#include "ChronoTimeEndpoing.hpp"

struct Args
{
    uint16_t port;
};

Args parseArgs(int argc,char ** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage:" << std::endl;
        std::cout << "tcp-datetime-server [PORT]" << std::endl;
        exit(1);
    }
    std::string port(argv[1]);
    Args result;
    try
    {
        result.port = std::stoi(port);
    }
    catch(const std::exception& e)
    {
        std::cout << "Usage:" << std::endl;
        std::cout << "tcp-datetime-server [PORT]" << std::endl;
        exit(1);
    }
    return result;
}

int main(int argc, char ** argv)
{
    auto args = parseArgs(argc,argv);
    std::shared_ptr<INetworkEndpoint> network = std::make_shared<NetworkEndpoint>(args.port);
    std::shared_ptr<IGetTimeEndpoint> clock = std::make_shared<ChronoTimeEndpoint>();
    
    TimeManager timeManager;
    timeManager.start(clock,network);
    network->start();

    while(true)
    {
        std::cout << "I'm alive!" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}
