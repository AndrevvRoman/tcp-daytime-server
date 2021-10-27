#include "TimeManager.hpp"

void TimeManager::start(std::shared_ptr<IGetTimeEndpoint> getTimeEndpoint,std::shared_ptr<INetworkEndpoint> networkEndpoint)
{
    m_getTimeEndpoint = getTimeEndpoint;
    m_networkEndpoint = networkEndpoint;

    m_networkEndpoint->subscribeToRead([this](const uint16_t id,const std::string & message)
    {
        auto timeShiftResult = m_getTimeEndpoint->getNowInTimezone(message);
        if (timeShiftResult.second)
            m_networkEndpoint->scheduleWrite(id,timeShiftResult.first);
        else
            m_networkEndpoint->scheduleWrite(id,"I don't know inputed timezone");
    });
}
