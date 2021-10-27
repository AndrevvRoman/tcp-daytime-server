#pragma once
#include <string>
#include <map>
#include <functional>

class IGetTimeEndpoint
{
public:
    ~IGetTimeEndpoint() = default;
    virtual std::pair<std::string,bool> getNowInTimezone(std::string timeZone) = 0;
    virtual std::map<std::string,int> getKnownTimeZones() = 0;
    virtual void setFormat(const std::string & format) = 0;
};
