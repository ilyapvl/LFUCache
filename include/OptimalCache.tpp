#ifndef OPTIMALCACHE_TPP
#define OPTIMALCACHE_TPP

#include "OptimalCache.h"



template<typename T>
OptimalCache<T>::OptimalCache(size_t size) : cache_size(size) {}

template<typename T>
void OptimalCache<T>::preprocessRequests(const std::vector<T>& requests)
{
    future_indices.clear();
    
    for (size_t i = 0; i < requests.size(); i++)
    {
        future_indices[requests[i]].push(i);
    }
}

template<typename T>
size_t OptimalCache<T>::simulate(const std::vector<T>& requests)
{
    if (cache_size == 0) return 0;
    
    std::unordered_set<T> cache;
    size_t hits = 0;
    
    for (size_t i = 0; i < requests.size(); i++)
    {
        T page = requests[i];
        
        auto it = future_indices.find(page);
        if (it != future_indices.end() && !it->second.empty() && it->second.front() == i)
        {
            it->second.pop();
        }
        
        if (cache.find(page) != cache.end())
        {
            hits++;
            continue;
        }
        
        if (cache.size() >= cache_size)
        {
            T evict_page;
            bool found = false;
            size_t farthest_use = 0;
            
            for (const T& cached_page : cache)
            {
                auto cached_it = future_indices.find(cached_page);
                if (cached_it == future_indices.end() || cached_it->second.empty())
                {
                    evict_page = cached_page;
                    found = true;
                    break;
                }
                
                if (cached_it->second.front() > farthest_use)
                {
                    farthest_use = cached_it->second.front();
                    evict_page = cached_page;
                    found = true;
                }
            }
            
            if (found)
            {
                cache.erase(evict_page);
            }
            else if (!cache.empty())
            {
                cache.erase(cache.begin());
            }
        }
        
        cache.insert(page);
    }
    
    return hits;
}




#endif // OPTIMALCACHE_TPP
