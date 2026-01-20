/**
 * @file ConfigurationException.h
 * @brief Исключение для ошибок конфигурации
 */

#ifndef CONFIGURATIONEXCEPTION_H
#define CONFIGURATIONEXCEPTION_H

#include "CacheException.h"
#include <string>

class ConfigurationException : public CacheException
{
public:

    explicit ConfigurationException(const std::string& message) 
        : CacheException("Configuration error: " + message) {}
    
    virtual ~ConfigurationException() noexcept = default;
};

#endif // CONFIGURATIONEXCEPTION_H
