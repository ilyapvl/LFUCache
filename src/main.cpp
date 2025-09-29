#include <iostream>
#include <vector>
#include <random>
#include <iomanip>

#include "LFUCache.h"
#include "OptimalCache.h"

std::vector<int> generateRandomRequests(int num_requests, int num_pages) 
{
    std::vector<int> requests;
    requests.reserve(num_requests);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, num_pages);
    
    for (int i = 0; i < num_requests; i++) 
    {
        requests.push_back(dist(gen));
    }
    
    return requests;
}

double testLFUCache(int cache_size, const std::vector<int>& requests) 
{
    LFUCache<int, int> cache(cache_size);
    int hits = 0;

    for (int page : requests) 
    {
        try 
        {
            cache.get(page);
            hits++;
        } 
        catch (const std::out_of_range& e) 
        {
            cache.put(page, page);
        }

        //std::cout << cache << '\n';
    }
    
    return static_cast<double>(hits) / requests.size();
}

double testOptimalCache(int cache_size, const std::vector<int>& requests) 
{
    OptimalCache optimal(cache_size);
    optimal.preprocessRequests(requests);
    size_t hits = optimal.simulate(requests);
    return static_cast<double>(hits) / requests.size();
}

int main() 
{
    int num_requests = 0, num_pages = 0, cache_size = 0;
    
    std::cout << "Введите количество запросов, количество страниц и размер кэша:" << std::endl;
    std::cin >> num_requests >> num_pages >> cache_size;

    std::cout << "Генерация запросов..." << std::endl;
    auto requests = generateRandomRequests(num_requests, num_pages);
    
    std::cout << "Тестирование LFU кэша..." << std::endl;
    double lfu_hit_rate = testLFUCache(cache_size, requests);
    
    std::cout << "Тестирование идеального кэша..." << std::endl;
    double optimal_hit_rate = testOptimalCache(cache_size, requests);
    
    std::cout << "Результаты тестирования (hit rate):" << std::endl;
    std::cout << "LFU кэш: " << std::fixed << std::setprecision(2) << (lfu_hit_rate * 100) << "%" << std::endl;
    std::cout << "Идеальный кэш: " << std::fixed << std::setprecision(2) << (optimal_hit_rate * 100) << "%" << std::endl;
    
    return 0;
}
