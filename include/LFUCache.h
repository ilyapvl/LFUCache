#ifndef LFUCACHE_H
#define LFUCACHE_H

#include <unordered_map>
#include <list>

#include "global.h"


template<typename K, typename V>
class LFUCache 
{
private:
    struct Node 
    {
        K key;
        V value;
        int freq;
        Node(const K& k, const V& v, int f) : key(k), value(v), freq(f) {}
    };

    using NodeIter = typename std::list<Node>::iterator;
    using FuncType = std::function<V(K)>;
    

    int capacity_;
    int min_freq_;
    FuncType slow_get_page_;
    std::unordered_map<int, std::list<Node>> freq_map_;
    std::unordered_map<K, NodeIter> key_map_;

    void increase_freq(NodeIter it);

public:
    LFUCache(int capacity, FuncType slow_get_page);
    V& get(const K& key);
    void put(const K& key);
    void evict();
    size_t size() const;
    bool empty() const;
    int capacity() const;
    void clear();

    friend std::ostream& operator<<(std::ostream& os, const LFUCache& cache)
    {
        os << "=== Cache state ===" << std::endl;
        os << "Capacity: " << cache.capacity_ << std::endl;
        os << "Size: " << cache.size() << std::endl;
        os << "Min frequency: " << cache.min_freq_ << std::endl;
    
        if (cache.empty())
        {
            os << "Cache is empty" << std::endl;
        }
    
        else
        {
            std::vector<int> frequencies;
            for (const auto& pair : cache.freq_map_)
            {
                frequencies.push_back(pair.first);
            }
            std::sort(frequencies.begin(), frequencies.end());
        
            for (int freq : frequencies)
            {
                const auto& node_list = cache.freq_map_.at(freq);
                os << "Freq " << freq << " (" << node_list.size() << "): ";
            
                for (const auto& node : node_list)
                {
                    os << "[" << node.key << "]->" << node.value << " ";
                }
                os << std::endl;
            }
        }
        os << "=================" << std::endl;
        return os;
    }
};

#include "LFUCache.tpp"

#endif // LFUCACHE_H
