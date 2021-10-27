#pragma once
#include <functional>

class INetworkEndpoint
{
public:
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void subscribeToRead(const std::function<void(const uint16_t idConnection, const std::string & message)> handler) = 0;
    virtual void scheduleWrite(const uint16_t idConnection, const std::string & message) = 0;
    ~INetworkEndpoint() = default;
};
