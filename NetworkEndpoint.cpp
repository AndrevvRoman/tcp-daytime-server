#pragma once
#include "NetworkEndpoint.hpp"
#include "BoostTcpEndpoint.hpp"

class NetworkEndpoint::pImpl
{
public:
    pImpl(uint16_t port) : m_server(port)
    {
    }
    size_t subscribeToNewConnection(std::function<void(const INetworkEndpoint::IConnection &)> handler)
    {
    }

private:
    BoostTcpEndpoint::TcpServer m_server;
};
