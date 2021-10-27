#pragma once
#include "INetworkEndpoint.hpp"

class NetworkEndpoint : public INetworkEndpoint
{
public:
    NetworkEndpoint(const uint16_t port);
    void start() override;
    void stop() override;
    void subscribeToRead(const std::function<void(const uint16_t id,const std::string &)> handler) override;
    void scheduleWrite(const uint16_t id,const std::string & message) override;
private:
    class pImpl;
    pImpl * m_pImpl = nullptr;
};
