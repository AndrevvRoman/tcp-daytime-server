#include "BoostTcpEndpoint.hpp"

#include <iomanip>
#include <ctime>
#include <chrono>

std::string getCurrentGmtTime()
{

    const std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    
    const std::time_t t_c = std::chrono::system_clock::to_time_t(now);
    
    // std::cout << "24 hours ago, the time was "
    //           << std::put_time(std::localtime(&t_c), "%F %T.\n") << std::flush;
    std::stringstream ss;
    ss << std::put_time(std::gmtime(&t_c), "%a %b  %d %T %Z %Y");
    std::string a = ss.str();
    return a;
    // Thu Jun  1 10:09:17 BST 2017
}

int main()
{
    std::cout << getCurrentGmtTime() << std::endl;
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
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}
