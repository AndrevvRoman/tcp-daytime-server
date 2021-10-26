#include "TimeManager.hpp"

void TimeManager::start(std::shared_ptr<IGetTimeEndpoint> getTimeEndpoint,std::shared_ptr<INetworkEndpoint> networkEndpoint)
{
    m_getTimeEndpoint = getTimeEndpoint;
    m_networkEndpoint = networkEndpoint;
}
