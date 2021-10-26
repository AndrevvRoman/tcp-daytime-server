#pragma once
#include <memory>

#include "IGetTimeEndpoint.hpp"
#include "INetworkEndpoint.hpp"

class TimeManager
{
public:
    TimeManager() = default;
    void start(std::shared_ptr<IGetTimeEndpoint>,std::shared_ptr<INetworkEndpoint>);
private:
    std::shared_ptr<IGetTimeEndpoint> m_getTimeEndpoint;
    std::shared_ptr<INetworkEndpoint> m_networkEndpoint;
};
