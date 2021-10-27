#include <iostream>
#include <memory>
#include <thread>

#include "TimeManager.hpp"
#include "IGetTimeEndpoint.hpp"
#include "INetworkEndpoint.hpp"
#include "NetworkEndpoint.hpp"
#include "ChronoTimeEndpoing.hpp"

int main()
{
    std::shared_ptr<INetworkEndpoint> network = std::make_shared<NetworkEndpoint>(8000);
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
