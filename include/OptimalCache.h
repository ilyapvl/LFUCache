#ifndef OPTIMALCACHE_H
#define OPTIMALCACHE_H

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>
#include <functional>

class OptimalCache 
{
private:
    size_t cache_size;
    std::function<int(int)> slow_get_page_;
    std::unordered_map<int, std::queue<size_t>> future_indices;
    std::unordered_map<int, int> cache_values; // Store actual values

public:
    OptimalCache(size_t size, std::function<int(int)> slow_get_page);
    void preprocessRequests(const std::vector<int>& requests);
    size_t simulate(const std::vector<int>& requests);
};

#endif // OPTIMALCACHE_H
