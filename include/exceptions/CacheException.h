/**
 * @file CacheException.h
 * @brief Базовый класс исключений для системы кэширования
 * @details Определяет общий базовый класс для всех исключений, связанных с кэшем
 */

#ifndef CACHEEXCEPTION_H
#define CACHEEXCEPTION_H

#include <stdexcept>
#include <string>

class CacheException : public std::runtime_error
{
public:

    explicit CacheException(const std::string& message) 
        : std::runtime_error(message) {}
    
    virtual ~CacheException() noexcept = default;
};

#endif // CACHEEXCEPTION_H
