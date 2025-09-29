#ifndef OPTIMALCACHE_H
#define OPTIMALCACHE_H

#include <unordered_map>
#include <queue>
#include <vector>
#include <unordered_set>

class OptimalCache 
{
private:
    size_t cache_size;
    std::unordered_map<int, std::queue<size_t>> future_indices;

public:
    OptimalCache(size_t size);
    void preprocessRequests(const std::vector<int>& requests);
    size_t simulate(const std::vector<int>& requests);
};

#endif // OPTIMALCACHE_H
