/**
 * @file OptimalCache.tpp
 * @brief Реализация методов OptimalCache
 */

#ifndef OPTIMALCACHE_TPP
#define OPTIMALCACHE_TPP

#include "OptimalCache.h"
#include <iostream>
#include <stdexcept>

template<typename K, typename V>
OptimalCache<K, V>::OptimalCache(size_t capacity, std::function<V(K)> slow_get_func) 
    : capacity_(capacity), 
      slow_get_func_(std::move(slow_get_func)),
      hit_count_(0),
      miss_count_(0),
      current_step_(0)
{
    if (capacity_ == 0)
    {
        throw InvalidArgumentException("Cache capacity must be greater than 0");
    }
}

template<typename K, typename V>
void OptimalCache<K, V>::preprocessRequests(const std::vector<K>& requests)
{
    future_indices_.clear();
    clear();




    current_step_ = 0;
    
    for (size_t i = 0; i < requests.size(); i++)
    {
        future_indices_[requests[i]].push(i);
    }
    
    for (auto& pair : future_indices_)
    {
        pair.second.push(std::numeric_limits<size_t>::max());
    }
}

template<typename K, typename V> size_t OptimalCache<K, V>::getNextUse(const K& key) const
{
    auto it = future_indices_.find(key);
    if (it == future_indices_.end() || it->second.empty())
    {
        return std::numeric_limits<size_t>::max();
    }
    return it->second.front();
}

template<typename K, typename V>
K OptimalCache<K, V>::findEvictionKey()
{
    if (current_cache_.empty())
    {
        return K();
    }
    
    K key_to_evict = K();
    size_t farthest_use = 0;
    
    for (const K& cached_key : current_cache_)
    {
        auto it = future_indices_.find(cached_key);
        if (it == future_indices_.end())
        {
            return cached_key;
        }
        
        while (!it->second.empty() && it->second.front() < current_step_)
        {
            it->second.pop();
        }
        
        if (it->second.empty() || it->second.front() == std::numeric_limits<size_t>::max())
        {
            return cached_key;
        }
        
        if (it->second.front() > farthest_use)
        {
            farthest_use = it->second.front();
            key_to_evict = cached_key;
        }
    }
    
    return key_to_evict;
}

template<typename K, typename V>
bool OptimalCache<K, V>::step(const K& key)
{
    if (future_indices_.empty())
    {
        throw CacheOperationException("preprocessRequests must be called before step");
    }
    
    current_step_++;
    
    auto it = future_indices_.find(key);
    if (it != future_indices_.end() && !it->second.empty())
    {
        if (it->second.front() == current_step_ - 1)
        {
            it->second.pop();
        }
    }
    
    if (current_cache_.find(key) != current_cache_.end())
    {
        hit_count_++;
        return true;
    }
    
    miss_count_++;
    



    if (current_cache_.size() >= capacity_)
    {
        K key_to_evict = findEvictionKey();
        
        if (key_to_evict != K() && current_cache_.find(key_to_evict) != current_cache_.end())
        {
            current_cache_.erase(key_to_evict);
            cache_values_.erase(key_to_evict);
        }
        else if (!current_cache_.empty())
        {
            K first_key = *current_cache_.begin();
            current_cache_.erase(current_cache_.begin());
            cache_values_.erase(first_key);
        }
    }
    
    current_cache_.insert(key);
    cache_values_[key] = slow_get_func_(key);
    
    return false;
}

template<typename K, typename V>
size_t OptimalCache<K, V>::simulate(const std::vector<K>& requests)
{
    if (future_indices_.empty())
    {
        throw CacheOperationException("preprocessRequests() must be called before simulate");
    }
    
    clear();
    current_step_ = 0;
    




    std::unordered_map<K, std::queue<size_t>> new_future_indices;
    for (const auto& pair : future_indices_)
    {
        new_future_indices[pair.first] = std::queue<size_t>();
    }
    
    for (size_t i = 0; i < requests.size(); i++)
    {
        new_future_indices[requests[i]].push(i);
    }
    
    for (auto& pair : new_future_indices)
    {
        pair.second.push(std::numeric_limits<size_t>::max());
    }
    
    future_indices_ = std::move(new_future_indices);
    
    for (const K& key : requests)
    {
        step(key);
    }
    
    return hit_count_;
}

template<typename K, typename V>
std::vector<std::pair<K, V>> OptimalCache<K, V>::getCacheContents() const
{
    std::vector<std::pair<K, V>> contents;
    contents.reserve(current_cache_.size());
    
    for (const K& key : current_cache_)
    {
        auto it = cache_values_.find(key);
        if (it != cache_values_.end())
        {
            contents.emplace_back(key, it->second);
        }
    }
    
    return contents;
}

template<typename K, typename V>
V OptimalCache<K, V>::getValue(const K& key) const
{
    auto it = cache_values_.find(key);
    if (it == cache_values_.end())
    {
        throw std::out_of_range("Key not found in cache");
    }
    return it->second;
}

template<typename K, typename V>
double OptimalCache<K, V>::getHitRate() const
{
    size_t total = hit_count_ + miss_count_;
    if (total == 0) return 0.0;

    
    return static_cast<double>(hit_count_) / total;
}

template<typename K, typename V>
void OptimalCache<K, V>::clear()
{
    current_cache_.clear();
    cache_values_.clear();
    hit_count_ = 0;
    miss_count_ = 0;
    current_step_ = 0;
}

#endif // OPTIMALCACHE_TPP
