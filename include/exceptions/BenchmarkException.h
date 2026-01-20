/**
 * @file BenchmarkException.h
 * @brief Исключение для ошибок бенчмаркинга
 */

#ifndef BENCHMARKEXCEPTION_H
#define BENCHMARKEXCEPTION_H

#include "CacheException.h"
#include <string>

class BenchmarkException : public CacheException
{
public:

    explicit BenchmarkException(const std::string& message) 
        : CacheException("Benchmark error: " + message) {}
    
    virtual ~BenchmarkException() noexcept = default;
};

#endif // BENCHMARKEXCEPTION_H
