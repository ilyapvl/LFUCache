#ifndef LFUCACHE_TPP
#define LFUCACHE_TPP

#include "LFUCache.h"

template<typename K, typename V>
void LFUCache<K, V>::increase_freq(NodeIter it)
{
    Node node = *it;
    int old_freq = node.freq;
    
    freq_map_[old_freq].erase(it);
    
    if (freq_map_[old_freq].empty())
    {
        freq_map_.erase(old_freq);
        if (min_freq_ == old_freq)
        {
            min_freq_++;
        }
    }
    
    node.freq++;
    freq_map_[node.freq].push_front(node);
    key_map_[node.key] = freq_map_[node.freq].begin();
}

template<typename K, typename V>
LFUCache<K, V>::LFUCache(int capacity) : capacity_(capacity), min_freq_(0)
{
    if (capacity_ <= 0)
    {
        throw std::invalid_argument("Capacity must be positive");
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
    increase_freq(it->second);
    return it->second->value;
}

template<typename K, typename V>
void LFUCache<K, V>::put(const K& key, const V& value)
{
    if (capacity_ == 0) return;

    auto it = key_map_.find(key);
    if (it != key_map_.end())
    {
        it->second->value = value;
        increase_freq(it->second);
        return;
    }

    if (key_map_.size() >= capacity_)
    {
        evict();
    }

    min_freq_ = 1;
    freq_map_[1].push_front(Node(key, value, 1));
    key_map_[key] = freq_map_[1].begin();
}

template<typename K, typename V>
void LFUCache<K, V>::evict()
{
    auto it = freq_map_.find(min_freq_);
    if (it == freq_map_.end() || it->second.empty()) 
    {
        min_freq_ = freq_map_.begin()->first;
        for (const auto& pair : freq_map_) 
        {
            if (pair.first < min_freq_ && !pair.second.empty()) 
            {
                min_freq_ = pair.first;
            }
        }
        it = freq_map_.find(min_freq_);
    }
    
    if (it != freq_map_.end() && !it->second.empty()) 
    {
        K key_to_remove = it->second.back().key;
        
        it->second.pop_back();
        
        key_map_.erase(key_to_remove);
        
        if (it->second.empty()) 
        {
            freq_map_.erase(min_freq_);
        }
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
int LFUCache<K, V>::capacity() const
{
    return capacity_;
}

template<typename K, typename V>
void LFUCache<K, V>::clear()
{
    freq_map_.clear();
    key_map_.clear();
    min_freq_ = 0;
}

#endif // LFUCACHE_TPP
