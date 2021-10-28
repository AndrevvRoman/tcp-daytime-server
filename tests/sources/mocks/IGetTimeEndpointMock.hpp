#include "IGetTimeEndpoint.hpp"

class IGetTimeEndpointMock : public IGetTimeEndpoint
{
public:
    std::pair<std::string,bool> getNowInTimezone(std::string timeZone) override
    {
        getTimeExecuted = true;
        return getNowResult;
    }
    std::map<std::string,int> getKnownTimeZones() override
    {
        return getKnownTimeZonesResult;
    }
    void setFormat(const std::string & format) override
    {
        return;
    }
    std::pair<std::string,bool> getNowResult;
    std::map<std::string,int> getKnownTimeZonesResult;
    bool getTimeExecuted = false;
};
