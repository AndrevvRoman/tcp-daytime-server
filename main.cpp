#include "BoostTcpEndpoint.hpp"

int main()
{
    std::shared_ptr<IConnection> connCopy;
    TcpServer srv(8000);
    srv.subscribeToNewConnection([&connCopy](std::shared_ptr<IConnection> conn)
    {
        connCopy = conn;
        conn->subscribeToRead([&connCopy](const std::string msg)
        {
            std::cout << "Answering to read subscribe" << std::endl;
            connCopy->scheduleWrite(msg);
        });
    });
    srv.start();
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}
