#pragma once
#include "INetworkEndpoint.hpp"

class NetworkEndpoint : public INetworkEndpoint
{
public:
    virtual size_t subscribeToNewConnection(std::function<void(const INetworkEndpoint::IConnection &)> handler);
private:
    class pImpl;
    pImpl * m_pImpl = nullptr;
};
