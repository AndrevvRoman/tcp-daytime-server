#include "NetworkEndpoint.hpp"
#include "BoostTcpEndpoint.hpp"

class NetworkEndpoint::pImpl
{
public:
    pImpl(uint16_t port) : m_server(port)
    {
    }
    void start()
    {
        m_server.start();
    }
    void stop()
    {
        m_server.stop();
    }
    void subscribeToRead(const std::function<void(const uint16_t id,const std::string &)> handler)
    {
        m_server.subscribeToNewMessage(handler);
    }
    void scheduleWrite(const uint16_t id, const std::string & message)
    {
        m_server.writeMessageToClinet(id,message);
    }
private:
    BoostTcpEndpoint::TcpServer m_server;
};

NetworkEndpoint::NetworkEndpoint(const uint16_t port)
{
    m_pImpl = new pImpl(port);
}


void NetworkEndpoint::start()
{
    m_pImpl->start();
}

void NetworkEndpoint::stop()
{
    m_pImpl->stop();
}

void NetworkEndpoint::subscribeToRead(const std::function<void(const uint16_t id,const std::string &)> handler)
{
    m_pImpl->subscribeToRead(handler);
}

void NetworkEndpoint::scheduleWrite(const uint16_t id,const std::string & message)
{
    m_pImpl->scheduleWrite(id, message);
}