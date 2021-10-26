#pragma once
#include <functional>

class INetworkEndpoint
{
public:
    class IConnection
    {
    public:
        virtual size_t subscribeToRead(const std::function<void(const std::string)> handler) = 0;
        virtual void scheduleWrite(const std::string message) = 0;
        ~IConnection() = default;
    };
    virtual size_t subscribeToNewConnection(std::function<void(const IConnection &)> handler);
    ~INetworkEndpoint() = default;
};
