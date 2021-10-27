#pragma once

#include <iostream>
#include <string>
#include <mutex>
#include <functional>
#include <memory>
#include <mutex>

#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/asio/deadline_timer.hpp>

#include "Handlers.hpp"

using boost::asio::ip::tcp;

namespace BoostTcpEndpoint
{
    class TcpConnection : public std::enable_shared_from_this<TcpConnection>
    {
    public:
        TcpConnection(boost::asio::io_context &io_context, int updateIntervalMiliSec = 500) : m_socket(io_context),
                                                                                              m_workingInterval(updateIntervalMiliSec),
                                                                                              m_timer(io_context, m_workingInterval)
        {
        }

        static std::shared_ptr<TcpConnection> create(boost::asio::io_context &io_context)
        {
            return std::make_shared<TcpConnection>(io_context);
        }

        tcp::socket &socket()
        {
            return m_socket;
        }

        void start()
        {
            _readTcpSocket();
            m_timer.async_wait(boost::bind(&TcpConnection::_bufferUpdateHandler, this));
        }

        size_t subscribeToRead(const std::function<void(const std::string)> handler)
        {
            return m_readHandlers.add(handler);
        }

        void scheduleWrite(const std::string message)
        {
            boost::asio::async_write(m_socket, boost::asio::buffer(message),
                                     boost::bind(&TcpConnection::_writeFinishedHandler, shared_from_this(),
                                                 boost::asio::placeholders::error,
                                                 boost::asio::placeholders::bytes_transferred));
        }

    private:
        void _bufferUpdateHandler()
        {
            if (m_socket.available() > 0)
            {
                std::cout << "Bytes ava: " << m_socket.available() << std::endl;
            }
            std::string s((std::istreambuf_iterator<char>(&m_buffer)), std::istreambuf_iterator<char>());
            if (s.size() != 0)
            {
                std::cout << s;
                m_readHandlers.handle(s);
            }
            m_timer.expires_at(m_timer.expires_at() + m_workingInterval);
            m_timer.async_wait(boost::bind(&TcpConnection::_bufferUpdateHandler, this));
        }
        void _writeFinishedHandler(const boost::system::error_code &, size_t bytes)
        {
            std::cout << "Bytes written: " << bytes << std::endl;
            //добавить std future для возможности получения результата
        }
        void _readTcpSocket()
        {
            std::lock_guard<decltype(m_socketMutex)> lock(m_socketMutex);

            boost::asio::async_read(m_socket, m_buffer,
                                    [this](boost::system::error_code ec, std::size_t sizeOfPack)
                                    {
                                        std::cout << "EOF. End of reading" << std::endl;
                                    });
        }
        boost::asio::streambuf m_buffer;
        tcp::socket m_socket;
        std::mutex m_socketMutex;
        boost::posix_time::millisec m_workingInterval;
        boost::asio::deadline_timer m_timer;
        Handlers<std::function<void(const std::string &)>> m_readHandlers;
    };

    class TcpServer
    {
    public:
        TcpServer(const uint16_t port)
            : m_acceptor(m_ioContext, tcp::endpoint(tcp::v4(), port))
        {
        }

        void start()
        {
            startAcceptingConnections();
            std::thread tr([this]()
                           { m_ioContext.run(); });
            tr.detach();
        }

        void stop()
        {
            m_ioContext.stop();
        }

        void startAcceptingConnections()
        {
            std::shared_ptr<TcpConnection> newConnection = TcpConnection::create(m_ioContext);
            m_acceptor.async_accept(newConnection->socket(), boost::bind(&TcpServer::_handleAccept, this, newConnection, boost::asio::placeholders::error));
        }

        void stopAcceptingConnections()
        {
            m_acceptor.close();
        }

        void disconnectAll()
        {
            {
                std::lock_guard<decltype(m_connectionsVecMutex)> lock(m_connectionsVecMutex);
                m_connections.clear();
            }
        }

        void disconnect(const int i)
        {
            {
                std::lock_guard<decltype(m_connectionsVecMutex)> lock(m_connectionsVecMutex);
                m_connections.erase(m_connections.begin() + i);
            }
        }

        size_t subscribeToNewConnection(const std::function<void(std::shared_ptr<TcpConnection>)> handler)
        {
            return m_connectionHandlers.add(handler);
        }

    private:
        void _handleAccept(std::shared_ptr<TcpConnection> newConnection, const boost::system::error_code &error)
        {
            std::cout << "New connecton!" << std::endl;
            if (!error)
            {
                {
                    std::lock_guard<decltype(m_connectionsVecMutex)> lock(m_connectionsVecMutex);
                    m_connections.push_back(newConnection);
                }

                {
                    std::lock_guard<decltype(m_connectionsHandlersMutex)> lock(m_connectionsHandlersMutex);
                    newConnection->start();
                    m_connectionHandlers.handle(m_connections[m_connections.size() - 1]);
                }
            }
            startAcceptingConnections();
        }

        boost::asio::io_context m_ioContext;
        tcp::acceptor m_acceptor;
        std::mutex m_connectionsVecMutex;
        std::vector<std::shared_ptr<TcpConnection>> m_connections;
        std::mutex m_connectionsHandlersMutex;
        Handlers<std::function<void(const std::shared_ptr<TcpConnection> &)>> m_connectionHandlers;
    };
}