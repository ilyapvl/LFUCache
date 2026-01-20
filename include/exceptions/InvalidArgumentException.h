/**
 * @file InvalidArgumentException.h
 * @brief Исключение для неверных аргументов функций
 */

#ifndef INVALIDARGUMENTEXCEPTION_H
#define INVALIDARGUMENTEXCEPTION_H

#include "CacheException.h"
#include <string>

class InvalidArgumentException : public CacheException
{
public:
    explicit InvalidArgumentException(const std::string& message) 
        : CacheException("Invalid argument: " + message) {}
    
    virtual ~InvalidArgumentException() noexcept = default;
};

#endif // INVALIDARGUMENTEXCEPTION_H
