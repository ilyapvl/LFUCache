#ifndef OPTIMALCACHE_H
#define OPTIMALCACHE_H

#include <unordered_map>
#include <queue>
#include <vector>
#include <unordered_set>

template<typename T>
class OptimalCache
{
private:
    size_t cache_size;
    std::unordered_map<T, std::queue<size_t>> future_indices;

public:
    OptimalCache(size_t size);
    void preprocessRequests(const std::vector<T>& requests);
    size_t simulate(const std::vector<T>& requests);
};

#include "OptimalCache.tpp"

#endif // OPTIMALCACHE_H
