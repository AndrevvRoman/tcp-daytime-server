#include "TimeManager.hpp"

void TimeManager::start(std::shared_ptr<IGetTimeEndpoint> getTimeEndpoint,std::shared_ptr<INetworkEndpoint> networkEndpoint)
{
    m_getTimeEndpoint = getTimeEndpoint;
    m_networkEndpoint = networkEndpoint;

    m_networkEndpoint->subscribeToRead([this](const uint16_t id,const std::string & message)
    {
        auto timeShiftResult = m_getTimeEndpoint->getNowInTimezone(message);
        if (timeShiftResult.second)
        {
            m_networkEndpoint->scheduleWrite(id,timeShiftResult.first);
        }
        else
        {
            std::string answer = "I don't know inputed timezone\n";
            answer += "I know these timezones:\n";
            auto zones = m_getTimeEndpoint->getKnownTimeZones();
            for (const auto & zone : zones)
            {
                answer += zone.first + ": UTC " + std::to_string(zone.second) + '\n';
            }
            m_networkEndpoint->scheduleWrite(id,answer);
        }
    });
}
