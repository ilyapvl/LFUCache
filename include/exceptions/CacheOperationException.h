/**
 * @file CacheOperationException.h
 * @brief Исключение для ошибок операций с кэшем
 */

#ifndef CACHEOPERATIONEXCEPTION_H
#define CACHEOPERATIONEXCEPTION_H

#include "CacheException.h"
#include <string>

class CacheOperationException : public CacheException
{
public:

    explicit CacheOperationException(const std::string& message) 
        : CacheException("Cache operation failed: " + message) {}
    
    virtual ~CacheOperationException() noexcept = default;
};

#endif // CACHEOPERATIONEXCEPTION_H
