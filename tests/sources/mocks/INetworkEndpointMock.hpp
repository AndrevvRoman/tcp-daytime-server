#include "INetworkEndpoint.hpp"
#include <string>
#include <functional>

class INetworkEndpointMock : public INetworkEndpoint
{
    public:
    virtual void start()
    {

    }
    virtual void stop()
    {

    }
    void subscribeToRead(const std::function<void(const uint16_t idConnection, const std::string & message)> handler) override
    {
        readHandler = handler;
    }
    void scheduleWrite(const uint16_t idConnection, const std::string & message) override
    {
        isWrited = true;
        writed = {idConnection,message};
    }
    void executeRead(const uint16_t idConnection, const std::string & message)
    {
        readHandler(idConnection,message);
    }
    std::function<void(const uint16_t idConnection, const std::string & message)> readHandler = [](const uint16_t idConnection, const std::string & message){};
    std::pair<uint16_t,std::string> writed = {0,""};
    bool isWrited = false;
};