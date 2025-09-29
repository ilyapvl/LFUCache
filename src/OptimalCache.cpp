#include "OptimalCache.h"

OptimalCache::OptimalCache(size_t size) : cache_size(size) {}

void OptimalCache::preprocessRequests(const std::vector<int>& requests) 
{
    future_indices.clear();
    
    for (size_t i = 0; i < requests.size(); i++) 
    {
        future_indices[requests[i]].push(i);
    }
}

size_t OptimalCache::simulate(const std::vector<int>& requests) 
{
    if (cache_size == 0) return 0;
    
    std::unordered_set<int> cache;
    size_t hits = 0;
    
    for (size_t i = 0; i < requests.size(); i++) 
    {
        int page = requests[i];
        
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
            int evict_page = -1;
            size_t farthest_use = 0;
            
            for (int cached_page : cache) 
            {
                auto cached_it = future_indices.find(cached_page);
                if (cached_it == future_indices.end() || cached_it->second.empty()) 
                {
                    evict_page = cached_page;
                    break;
                }
                
                if (cached_it->second.front() > farthest_use) 
                {
                    farthest_use = cached_it->second.front();
                    evict_page = cached_page;
                }
            }
            
            if (evict_page != -1) 
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
