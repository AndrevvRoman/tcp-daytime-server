#pragma once
#include <map>
#include <functional>
#include <mutex>
#include <stdint.h>
#include <thread>
#include <vector>
#include <algorithm>

template <typename T>
class Handlers
{
public:
    size_t add(T handler)
    {
        std::lock_guard<decltype(m_handlersMutex)> lock(m_handlersMutex);
        size_t handlerId = _newHandlerId();
        m_handlers.emplace(handlerId, handler);
        return handlerId;
    }

    bool has(size_t handlerId) const
    {
        std::lock_guard<decltype(m_handlersMutex)> lock(m_handlersMutex);

        bool result = false;

        if (m_handlers.find(handlerId) != m_handlers.end())
        {
            result = true;
        }
        return result;
    }

    T handler(size_t handlerId) const
    {
        std::lock_guard<decltype(m_handlersMutex)> lock(m_handlersMutex);
        if (m_handlers.find(handlerId) != m_handlers.end())
        {
            return m_handlers[handlerId];
        }
        else
        {
            throw std::runtime_error("Handler with id " + std::to_string(handlerId) + " not found!");
        }
    }

    std::map<size_t, T> handlers() const
    {
        std::lock_guard<decltype(m_handlersMutex)> lock(m_handlersMutex);
        return m_handlers;
    }

    template <typename... Args>
    void handle(const Args &...args)
    {
        std::vector<size_t> keys;

        std::lock_guard<decltype(m_handlersMutex)> lock(m_handlersMutex);
        std::transform(m_handlers.begin(), m_handlers.end(), std::back_inserter(keys), [](auto &pair)
                       { return pair.first; });

        for (auto key : keys)
        {
            if (m_handlers.count(key))
            {
                auto &handler = m_handlers.at(key);
                handler(args...);
            }
        }
    }

    void remove(size_t handlerId)
    {
        std::lock_guard<decltype(m_handlersMutex)> lock(m_handlersMutex);
        if (m_handlers.find(handlerId) != m_handlers.end())
        {
            m_handlers.erase(handlerId);
        }
    }

private:
    size_t _newHandlerId()
    {
        size_t handlerId = 0;
        size_t maxId = std::numeric_limits<size_t>::max();

        size_t candidateId = 0;
        if (m_lastId < maxId)
        {
            candidateId = m_lastId + 1;
        }

        auto idIsUsed = [this](size_t candidateId)
        {
            bool found = false;
            if (m_handlers.find(candidateId) != m_handlers.end())
            {
                found = true;
            }
            return found;
        };

        bool newIdFound = false;
        while (candidateId != m_lastId)
        {
            if (idIsUsed(candidateId) == false)
            {
                newIdFound = true;
                handlerId = candidateId;
                break;
            }

            if (candidateId < maxId)
            {
                ++candidateId;
            }
            else
            {
                candidateId = 0;
            }
        }

        if (newIdFound)
        {
            m_lastId = handlerId;
        }
        else
        {
            throw std::runtime_error("Can not create new handler ID");
        }
        return handlerId;
    }
    std::map<size_t, T> m_handlers;
    size_t m_lastId = 0;
    mutable std::recursive_mutex m_handlersMutex;
};
