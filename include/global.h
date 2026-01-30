/**
 * @file global.h
 * @brief Общие определения и функции для системы кэширования
 */

#ifndef GLOBAL_H
#define GLOBAL_H

#include <functional>
#include <vector>
#include <memory.h>

/**
 * @brief Структура, представляющая страницу в кэше
 */
struct Page
{
    int index;     
    int size;     
    std::unique_ptr<char[]> data;   
    
    Page(int idx, int sz) : index(idx), size(sz), data(std::make_unique<char[]>(sz)) {}
    
    ~Page() noexcept = default;
    
    Page(const Page&) = delete;
    Page& operator=(const Page&) = delete;
    
    Page(Page&& other) : index(other.index), size(other.size), data(std::move(other.data)) 
    {
        other.size = 0;
    }
    
    Page& operator=(Page&& other)
    {
        if (this != &other)
        {
            index = other.index;
            size = other.size;
            data = std::move(other.data);
            other.size = 0;
        }
        return *this;
    }
};

/**
 * @brief Функция медлнного получения страницы

 * @param index Индекс запрашиваемой страницы
 * @return Значение страницы (здесь в качестве значения используется просто индекс)
 */
inline int slow_get_page_int(int index)
{
    return index;
}

/**
 * @brief Тип функции для медленного получения страницы
 */
using SlowGetPageFunc = std::function<int(int)>;

/**
 * @brief Тип для последовательности запросов
 */
using RequestSequence = std::vector<int>;

/**
 * @brief Тип для хранения результатов бенчмаркинга
 */
struct BenchmarkResult
{
    size_t cache_size;       
    double lfu_hit_rate;        
    double optimal_hit_rate;    
    double difference;         
    
    BenchmarkResult(size_t size, double lfu, double optimal)
        : cache_size(size), lfu_hit_rate(lfu), optimal_hit_rate(optimal),
          difference(optimal - lfu) {}
};

#endif // GLOBAL_H
