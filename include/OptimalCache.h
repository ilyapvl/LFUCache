/**
 * @file OptimalCache.h
 * @brief Заголовочный файл для оптимального кэша
 */

#ifndef OPTIMALCACHE_H
#define OPTIMALCACHE_H

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>
#include <functional>
#include <limits>
#include <iostream>
#include <string>

#include "global.h"
#include "exceptions/InvalidArgumentException.h"
#include "exceptions/CacheOperationException.h"

/**
 * @brief Оптимальный кэш
 * 
 * @tparam K Тип ключа
 * @tparam V Тип значения
 */
template<typename K, typename V>
class OptimalCache
{
private:
    size_t capacity_;                  
    std::function<V(K)> slow_get_func_;  
    
    /**
     * @brief Будщие индексы

     */
    std::unordered_map<K, std::queue<size_t>> future_indices_;
    
    /**
     * @brief Ключи в кэше
     */
    std::unordered_set<K> current_cache_;
    
    /**
     * @brief Значения в кэше
     */
    std::unordered_map<K, V> cache_values_;
    
    size_t hit_count_;    
    size_t miss_count_;   
    size_t current_step_; 

    /**
     * @brief Находит ключ для вытеснения из кэша
     * @return Ключ для вытеснения
     */
    K findEvictionKey();

public:
    /**
     * @brief Конструктор оптимального кэша
     */
    OptimalCache(size_t capacity, std::function<V(K)> slow_get_func);
    
    /**
     * @brief Предобработка последовательности запросов
     */
    void preprocessRequests(const std::vector<K>& requests);
    
    /**
     * @brief Симуляция работы кэша
     */
    size_t simulate(const std::vector<K>& requests);
    bool step(const K& key);
    
    /**
     * @brief Получить содержимое
     */
    std::vector<std::pair<K, V>> getCacheContents() const;
    
    /**
     * @brief Получить значение по ключу
     */
    V getValue(const K& key) const;
    


    size_t getCurrentSize()         const { return current_cache_.size(); }
    size_t getCapacity()            const { return capacity_; }
    bool contains(const K& key)     const { return current_cache_.find(key) != current_cache_.end(); }
    size_t getHitCount()            const { return hit_count_; }
    





    double getHitRate() const;



    /**
     * @brief Очистить состояние кэша
     */
    void clear();
    
    /**
     * @brief Получить следующее использование
     */
    size_t getNextUse(const K& key) const;
};

#include "OptimalCache.tpp"

#endif // OPTIMALCACHE_H
