/**
 * @file LFUCache.tpp
 * @brief Реализация шаблонных методов LFU кэша
 */

#ifndef LFUCACHE_TPP
#define LFUCACHE_TPP

#include "LFUCache.h"
#include <stdexcept>

template<typename K, typename V>
void LFUCache<K, V>::increase_frequency(NodeIterator it)
{
    if (it == NodeIterator())
    {
        throw CacheOperationException("Invalid iterator in increase_frequency");
    }
    
    Node node = *it;
    int old_freq = node.frequency;
    
    auto freq_it = frequency_map_.find(old_freq);
    if (freq_it == frequency_map_.end() || freq_it->second.empty())
    {
        throw CacheOperationException("Problems with freq map");
    }
    
    freq_it->second.erase(it);
    
    if (freq_it->second.empty())
    {
        frequency_map_.erase(old_freq);
        if (min_frequency_ == old_freq)
        {
            min_frequency_++;
        }
    }
    
    node.frequency++;
    frequency_map_[node.frequency].push_front(node);
    key_map_[node.key] = frequency_map_[node.frequency].begin();
}

template<typename K, typename V>
LFUCache<K, V>::LFUCache(size_t capacity, SlowGetFunc slow_get_func) 
    : capacity_(capacity), min_frequency_(0), slow_get_func_(std::move(slow_get_func))
{
    if (capacity_ <= 0)
    {
        throw InvalidArgumentException("Cache capacity must be greater than 0");
    }
}

template<typename K, typename V>
V& LFUCache<K, V>::get(const K& key)
{
    auto it = key_map_.find(key);
    if (it == key_map_.end())
    {
        throw std::out_of_range("Key not found");
    }
    
    increase_frequency(it->second);
    return it->second->value;
}

template<typename K, typename V>
void LFUCache<K, V>::put(const K& key)
{
    if (capacity_ == 0)
    {
        return;
    }
    
    auto it = key_map_.find(key);
    if (it != key_map_.end())
    {
        it->second->value = slow_get_func_(key);
        increase_frequency(it->second);
        return;
    }
    
    if (key_map_.size() >= capacity_)
    {
        evict();
    }
    
    min_frequency_ = 1; //TODO - 1 или 0
    frequency_map_[1].push_front(Node(key, slow_get_func_(key), 1));
    key_map_[key] = frequency_map_[1].begin();
}

template<typename K, typename V>
void LFUCache<K, V>::evict()
{
    if (empty())
    {
        throw CacheOperationException("Cannot evict from empty cache");
    }
    
    auto it = frequency_map_.find(min_frequency_);
    
    while (it == frequency_map_.end() || it->second.empty())
    {
        min_frequency_++;
        it = frequency_map_.find(min_frequency_);
    }
    
    K key_to_remove = it->second.back().key;
    it->second.pop_back();
    
    key_map_.erase(key_to_remove);
    
    if (it->second.empty())
    {
        frequency_map_.erase(min_frequency_);
    }
}

template<typename K, typename V>
size_t LFUCache<K, V>::size() const
{
    return key_map_.size();
}

template<typename K, typename V>
bool LFUCache<K, V>::empty() const
{
    return key_map_.empty();
}

template<typename K, typename V>
size_t LFUCache<K, V>::capacity() const
{
    return capacity_;
}

template<typename K, typename V>
void LFUCache<K, V>::clear()
{
    frequency_map_.clear();
    key_map_.clear();
    min_frequency_ = 0;
}

#endif // LFUCACHE_TPP
