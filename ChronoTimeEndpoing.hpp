#pragma once
#include "IGetTimeEndpoint.hpp"
#include <string>
#include <map>
#include <ctime>

class ChronoTimeEndpoint : public IGetTimeEndpoint
{
public:
    ChronoTimeEndpoint(
        const std::string &format = "%a %b %d %T %Z %Y", std::function<time_t()> timeGetter = [](){ return std::time(nullptr); });
    std::pair<std::string, bool> getNowInTimezone(const std::string &timeZone);
    void setFormat(const std::string &format);
    ~ChronoTimeEndpoint();

private:
    class pImpl;
    pImpl *m_pImpl = nullptr;
};