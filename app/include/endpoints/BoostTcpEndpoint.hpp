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
        TcpConnection(boost::asio::io_context& io_context, uint32_t id) : m_socket(io_context), m_id(id)
        {
        }
        ~TcpConnection()
        {
            std::cout << "Closing socket in destructor" << std::endl;
            m_socket.close();
        }
        static std::shared_ptr<TcpConnection> create(boost::asio::io_context& io_context, uint32_t id)
        {
            return std::make_shared<TcpConnection>(io_context, id);
        }
        tcp::socket& socket()
        {
            return m_socket;
        }
        void start()
        {
            _readTcpSocket();
        }
        size_t subscribeToRead(const std::function<void(uint32_t id, const std::string)> handler)
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
        void pullBuffer()
        {
            if (m_socket.available() > 0)
            {
                std::cout << "Bytes ava: " << m_socket.available() << std::endl;
            }
            std::string s((std::istreambuf_iterator<char>(&m_buffer)), std::istreambuf_iterator<char>());
            if (s.size() != 0)
            {
                std::cout << s;
                m_readHandlers.handle(m_id, s);
            }
        }
        std::atomic<bool> isAlive()
        {
            return m_isAlive.load();
        }
        uint32_t id()
        {
            return m_id;
        }
    private:
        void _writeFinishedHandler(const boost::system::error_code&, size_t bytes)
        {
            std::cout << "Bytes written: " << bytes << std::endl;
            //добавить std future для возможности получения результата
        }
        void _readTcpSocket()
        {
            boost::asio::async_read(m_socket, m_buffer,
                [this](boost::system::error_code ec, std::size_t sizeOfPack)
                {
                    std::cout << "EOF. End of reading" << std::endl;
                    m_isAlive.store(false);
                });
        }
        boost::asio::streambuf m_buffer;
        tcp::socket m_socket;
        Handlers<std::function<void(uint32_t, const std::string&)>> m_readHandlers;
        std::atomic<bool> m_isAlive = true;
        const uint32_t m_id;
    };

    class TcpServer
    {
    public:
        TcpServer(const uint16_t port, int updateIntervalMiliSec = 500)
            : m_acceptor(m_ioContext, tcp::endpoint(tcp::v4(), port)),
            m_workingInterval(updateIntervalMiliSec),
            m_timer(m_ioContext, m_workingInterval),
            m_port(port)
        {
        }
        void start()
        {
            std::cout << "Starting server at " << m_port << std::endl;
            startAcceptingConnections();
            std::thread tr([this]()
                { m_ioContext.run(); });
            tr.detach();
        }
        void stop()
        {
            m_acceptor.close();
            m_ioContext.stop();
        }
        void startAcceptingConnections()
        {
            std::shared_ptr<TcpConnection> newConnection = TcpConnection::create(m_ioContext, m_lastId);
            m_lastId++;
            m_acceptor.async_accept(newConnection->socket(), boost::bind(&TcpServer::_handleAcceptConnection, this, newConnection, boost::asio::placeholders::error));
        }
        size_t subscribeToNewMessage(std::function<void(const uint16_t clientId, const std::string& message)> handler)
        {
            return m_readHandlers.add(handler);
        }
        void writeMessageToClinet(uint16_t clientId, const std::string& message)
        {
            try
            {
                m_connections.at(clientId)->scheduleWrite(message);
            }
            catch (const std::out_of_range e)
            {
                //pass
            }

        }
    private:
        void _handleAcceptConnection(std::shared_ptr<TcpConnection> newConnection, const boost::system::error_code& error)
        {
            std::cout << "New connecton!" << std::endl;
            if (!error)
            {
                {
                    std::lock_guard<decltype(m_connectionsMapMutex)> lock(m_connectionsMapMutex);
                    m_connections[newConnection->id()] = newConnection;
                    m_connections[newConnection->id()]->subscribeToRead([this](uint32_t ConnId, const std::string msg)
                        {
                            m_readHandlers.handle(ConnId, msg);
                        });
                    newConnection->start();
                    m_timer.expires_at(m_timer.expiry() + m_workingInterval);
                    m_timer.async_wait(boost::bind(&TcpServer::_updateConnections, this));
                }
            }
            startAcceptingConnections();
        }
        void _updateConnections()
        {
            if (m_connections.size() > 0)
            {
                auto it = m_connections.begin();
                auto end = m_connections.end();
                for (; it != end; )
                {
                    if (it->second->isAlive() == false)
                    {
                        it = m_connections.erase(it);
                    }
                    else 
                    {
                        std::lock_guard<decltype(m_connectionsMapMutex)> lock(m_connectionsMapMutex);
                        it->second->pullBuffer();
                        ++it;
                    }
                }
                m_timer.expires_at(m_timer.expiry() + m_workingInterval);
                m_timer.async_wait(boost::bind(&TcpServer::_updateConnections, this));
            }
        }

        boost::asio::io_context m_ioContext;
        boost::asio::chrono::milliseconds m_workingInterval;
        boost::asio::steady_timer m_timer;
        tcp::acceptor m_acceptor;

        std::mutex m_connectionsMapMutex;
        std::map<uint32_t, std::shared_ptr<TcpConnection>> m_connections;
        uint32_t m_lastId = 1;
        
        Handlers<std::function<void(const uint16_t id, const std::string&)>> m_readHandlers;

        uint16_t m_port;
    };
}