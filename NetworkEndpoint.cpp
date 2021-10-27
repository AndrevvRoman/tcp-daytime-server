#pragma once
#include "NetworkEndpoint.hpp"
#include "BoostTcpEndpoint.hpp"



class NetworkEndpoint::pImpl
{
public:
    pImpl(uint16_t port) : m_server(port)
    {
    }
    void subscribeToRead(const std::function<void(const uint16_t id,const std::string &)> handler)
    {
        // тут по id привязывать handler для чтения и connection через мапу
    }
    void scheduleWrite(const uint16_t id, const std::string & message)
    {
        // тут по id привязывать handler для записи и connection через мапу
    }
    void subscribeToNewConnection(const std::function<void(const uint16_t idConnection)> newConnectionHanlder)
    {
        m_server.subscribeToNewConnection([this,newConnectionHanlder](std::shared_ptr<BoostTcpEndpoint::TcpConnection> newConnection) 
        {
            m_connections[m_lastHandlerId] = newConnection;
            newConnectionHanlder(m_lastHandlerId);
            m_lastHandlerId++;
        });
    }
private:
    BoostTcpEndpoint::TcpServer m_server;
    std::map<size_t,std::shared_ptr<BoostTcpEndpoint::TcpConnection>> m_connections;
    size_t m_lastHandlerId = 1;
};

void NetworkEndpoint::subscribeToRead(const std::function<void(const uint16_t id,const std::string &)> handler)
{
    m_pImpl->subscribeToRead(handler);
}

void NetworkEndpoint::scheduleWrite(const uint16_t id,const std::string & message)
{
    m_pImpl->scheduleWrite(id, message);
}

void NetworkEndpoint::subscribeToNewConnection(const std::function<void(const uint16_t idConnection)> handler)
{
    m_pImpl->subscribeToNewConnection(handler);
}