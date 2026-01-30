/**
 * @file main.cpp
 * @brief Основной файл
 */

#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <string>
#include <algorithm>
#include <map>
#include <cmath>
#include <memory>

#include "LFUCache.h"
#include "OptimalCache.h"
#include "global.h"
#include "exceptions/ConfigurationException.h"
#include "exceptions/BenchmarkException.h"



/**
 * @brief Генерирует случайную последовательность запросов
 * @param num_requests Количество запросов
 * @param num_pages Количество страниц
 * @return Вектор запросов
 * 
 * @throws std::invalid_argument если параметры некорректны
 */
std::vector<int> generateRandomRequests(int num_requests, int num_pages)
{
    if (num_requests <= 0)
    {
        throw std::invalid_argument("Number of requests must be positive: " + std::to_string(num_requests));
    }
    if (num_pages <= 0)
    {
        throw std::invalid_argument("Number of pages must be positive: " + std::to_string(num_pages));
    }
    
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

/**
 * @brief Генерирует последовательность запросов с повторениями
 * @param num_requests Количество запросов
 * @param num_pages Количество страниц
 * @return Вектор запросов
 */
std::vector<int> generateSequentialRequests(int num_requests, int num_pages)
{
    if (num_requests <= 0 || num_pages <= 0)
    {
        throw std::invalid_argument("Requests and pages must be positive");
    }
    
    std::vector<int> requests;
    requests.reserve(num_requests);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, num_pages);
    
    for (int i = 0; i < num_requests; i++)
    {
        //TODO - произвольная длина повторяемой последовательности
        if (i % 20 == 0 && i + 10 < num_requests)
        {
            int start = dist(gen);
            for (int j = 0; j < 10 && i + j < num_requests; j++)
            {
                requests.push_back((start + j) % num_pages + 1);
            }
            i += 9;
        }
        else
        {
            requests.push_back(dist(gen));
        }
    }
    
    while (requests.size() < num_requests)
    {
        requests.push_back(dist(gen));
    }
    
    return requests;
}

/**
 * @brief Тестирует LFU кэш
 * @param cache_size Размер кэша
 * @param requests Последовательность запросов
 * @return hit rate 
 * 
 * @throws CacheOperationException если ошибка
 */
double testLFUCache(size_t cache_size, const std::vector<int>& requests)
{
    try
    {
        lfu::LFUCache<int, int> cache(cache_size, slow_get_page_int);
        int hits = 0;
        
        for (int page : requests)
        {
            try
            {
                cache.get(page);
                //std::cout << "hit page " << page << std::endl;
                hits++;
            }
            catch (const std::out_of_range&)
            {
                //std::cout << "miss page " << page << std::endl;
                cache.put(page);
            }
        }
        
        return static_cast<double>(hits) / requests.size();
    }
    catch (const std::exception& e)
    {
        throw CacheOperationException(std::string("LFU cache test failed: ") + e.what());
    }
}

/**
 * @brief Тестирует оптимальный кэш
 * @param cache_size Размер кэша
 * @param requests Последовательность запросов
 * @return hit rate
 * 
 * @throws CacheOperationException если ошибка
 */
double testOptimalCache(size_t cache_size, const std::vector<int>& requests)
{
    try
    {
        opt::OptimalCache<int, int> optimal(cache_size, slow_get_page_int);
        optimal.preprocessRequests(requests);
        size_t hits = optimal.simulate(requests);
        return static_cast<double>(hits) / requests.size();
    }
    catch (const std::exception& e)
    {
        throw CacheOperationException(std::string("Optimal cache test failed: ") + e.what());
    }
}

/**
 * @brief Выполняет бенчмаркинг алгоритмов кэширования (сравнение + изменение размера кэшей при постоянных запросах)
 * @param min_cache_size Минимальный размер кэша
 * @param max_cache_size Максимальный размер кэша
 * @param step Шаг размера
 * @param requests Последовательность запросов
 * @return Вектор результатов
 * 
 * @throws BenchmarkException если параметры некорректны
 * @throws CacheOperationException если ошибка
 */
std::vector<BenchmarkResult> runBenchmark(size_t min_cache_size, size_t max_cache_size, 
                                     size_t step, const std::vector<int>& requests) //NOTE - нужны тесты
{
    if (min_cache_size == 0)
    {
        throw BenchmarkException("Minimum cache size must be >0");
    }
    if (max_cache_size < min_cache_size)
    {
        throw BenchmarkException("Maximum cache size must be >= to minimum");
    }
    if (step == 0)
    {
        throw BenchmarkException("Step must be greater than 0");
    }
    if (requests.empty())
    {
        throw BenchmarkException("Request sequence is empty");
    }
    
    std::vector<BenchmarkResult> results;
    
    for (size_t cache_size = min_cache_size; cache_size <= max_cache_size; cache_size += step)
    {
        try
        {
            std::cout << "Testing cache size " << cache_size << "..." << std::endl;
            
            double lfu_hit_rate = testLFUCache(cache_size, requests);
            double optimal_hit_rate = testOptimalCache(cache_size, requests);
            
            results.emplace_back(cache_size, lfu_hit_rate * 100, optimal_hit_rate * 100);
            
        }
        catch (const std::exception& e)
        {
            std::cerr << "Failed to test cache size " << cache_size << ": " << e.what() << std::endl;
        }
    }
    
    if (results.empty())
    {
        throw BenchmarkException("No benchmark results were collected");
    }
    
    return results;
}



/**
 * @brief Выводит результаты бенчмаркинга
 * @param results Вектор результатов
 */
void printBenchmarkResults(const std::vector<BenchmarkResult>& results) 
{
    if (results.empty())
    {
        std::cout << "No results to display." << std::endl;
        return;
    }
    
    std::cout << "Benchmark results" << std::endl;
    std::cout << std::string(45, '=') << std::endl;
    std::cout << std::left << std::setw(12) << "Cache size"
                << std::setw(15) << "LFU hit rate" 
                << std::setw(18) << "Optimal hit rate" << std::endl;


    std::cout << std::string(45, '-') << std::endl;
    
    for (const auto& result : results)
    {
        std::cout << std::left << std::setw(12) << result.cache_size
                << std::fixed << std::setprecision(2)
                << std::setw(15) << result.lfu_hit_rate 
                << std::setw(18) << result.optimal_hit_rate << std::endl;
    }
    
    std::cout << std::string(75, '-') << std::endl;
}



void printHelp()
{
    std::cout << "\nCompare lfu and optimal caches\n\n";

    std::cout << "Usage:\n";
    std::cout << "  --mode=lfu              : Run only LFU cache simulation\n";
    std::cout << "  --mode=optimal          : Run only Optimal cache simulation\n";
    std::cout << "  --mode=compare          : Compare both (default)\n";
    std::cout << "  --mode=benchmark        : Run benchmark\n\n";
    
    std::cout << "Simulation Parameters:\n";
    std::cout << "  --requests=<number>     : Number of requests to generate (default: 1000)\n";
    std::cout << "  --pages=<number>        : Number of unique pages (default: 100)\n";
    std::cout << "  --cache-size=<number>   : Cache size for simulation (default: 10)\n";
    std::cout << "  --request-type=<type>   : Type of requests (random/sequential, default: random)\n\n";
    
    std::cout << "Benchmark Parameters:\n";
    std::cout << "  --min-size=<number>     : Minimum cache size (default: 5)\n";
    std::cout << "  --max-size=<number>     : Maximum cache size (default: 50)\n";
    std::cout << "  --step=<number>         : Step for cache size (default: 5)\n\n";
}


int getParameters(int argc, char** argv,
                    std::string& mode,
                    int& num_requests, int& num_pages, int& cache_size,
                    std::string& request_type,
                    int& min_cache_size, int& max_cache_size, int& step)
{
    mode = "compare";
    num_requests = 1000;
    num_pages = 100;
    cache_size = 10;
    request_type = "random";
    
    min_cache_size = 5;
    max_cache_size = 50;
    step = 5;
    
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        
        if (arg == "--help" || arg == "-h")
        {
            printHelp();
            return 0;
        }
        else if (arg.substr(0, 7) == "--mode=")
        {
            mode = arg.substr(7);
        }
        else if (arg.substr(0, 11) == "--requests=")
        {
            num_requests = stoi(arg.substr(11));
        }
        else if (arg.substr(0, 8) == "--pages=")
        {
            num_pages = stoi(arg.substr(8));
        }
        else if (arg.substr(0, 13) == "--cache-size=")
        {
            cache_size = stoul(arg.substr(13));
        }
        else if (arg.substr(0, 15) == "--request-type=")
        {
            request_type = arg.substr(15);
        }
        else if (arg.substr(0, 11) == "--min-size=")
        {
            min_cache_size = stoul(arg.substr(11));
        }
        else if (arg.substr(0, 11) == "--max-size=")
        {
            max_cache_size = stoul(arg.substr(11));
        }
        else if (arg.substr(0, 7) == "--step=")
        {
            step = stoul(arg.substr(7));
        }
        else
        {
            throw ConfigurationException("Unknown argument: " + arg);
        }
    }
    
    if (num_requests <= 0)
    {
        throw std::invalid_argument("Number of requests must be > 0: " + std::to_string(num_requests));
    }

    if (num_pages <= 0)
    {
        throw std::invalid_argument("Number of pages must be > 0: " + std::to_string(num_pages));
    }

    if (mode != "lfu" && mode != "optimal" && mode != "compare" && mode != "benchmark")
    {
        throw ConfigurationException("Invalid mode: " + mode);
    }

    if (request_type != "random" && request_type != "sequential")
    {
        throw ConfigurationException("Invalid request type");
    }

    if (mode != "benchmark" && cache_size == 0)
    {
        throw std::invalid_argument("Cache size must be > 0");
    }

    if (mode == "benchmark")
    {
        if (min_cache_size <= 0)
        {
            throw std::invalid_argument("Minimum cache size must be greater than 0");
        }
        if (max_cache_size < min_cache_size)
        {
            throw std::invalid_argument("Maximum cache size must be >= minimum cache size");
        }
        if (step <= 0)
        {
            throw std::invalid_argument("Step must be greater than 0");
        }
    }

    return 0;
}

int main(int argc, char* argv[])
{
    std::string mode = "";
    int num_requests = 0;
    int num_pages = 0;
    int cache_size = 0;
    std::string request_type = "";
        
    int min_cache_size = 0;
    int max_cache_size = 0;
    int step = 0;
    
    try
    {
        getParameters(argc, argv, mode, num_requests, num_pages, cache_size,
                    request_type, min_cache_size, max_cache_size, step);



        std::cout << "\nParameters:\n";
        std::cout << std::left << std::setw(20) << "Mode:" << mode << std::endl;
        std::cout << std::setw(20) << "Requests:" << num_requests << std::endl;
        std::cout << std::setw(20) << "Pages:" << num_pages << std::endl;




        if (mode != "benchmark")
        {
            std::cout << std::setw(20) << "Cache size:" << cache_size << std::endl;
        }
        else
        {
            std::cout << std::setw(20) << "Benchmark range:" << min_cache_size << " to " << max_cache_size << std::endl;
            std::cout << std::setw(20) << "Step:" << step << std::endl;
        }




        std::cout << std::setw(20) << "Request type:" << request_type << std::endl;
        
        std::cout << "\nGenerating requests..." << std::endl;
        std::vector<int> requests;
        

        if (request_type == "sequential")
        {
            requests = generateSequentialRequests(num_requests, num_pages);
        }
        else
        {
            requests = generateRandomRequests(num_requests, num_pages);
        }
        
        std::cout << "Generated " << requests.size() << " requests" << std::endl;
        



        if (mode == "benchmark")
        {
            std::vector<BenchmarkResult> results = runBenchmark(min_cache_size, max_cache_size, step, requests);
            printBenchmarkResults(results);
        }

        else
        {

            if (mode == "lfu" || mode == "compare")
            {
                std::cout << "\nTesting LFU cache..." << std::endl;
                double lfu_hit_rate = testLFUCache(cache_size, requests);
                std::cout << "LFU cache hit rate: " << std::fixed << std::setprecision(2) 
                     << (lfu_hit_rate * 100) << "%" << std::endl;
            }
            
            if (mode == "optimal" || mode == "compare")
            {
                std::cout << "\nTesting optimal cache..." << std::endl;
                double optimal_hit_rate = testOptimalCache(cache_size, requests);
                std::cout << "Optimal cache hit rate: " << std::fixed << std::setprecision(2) 
                     << (optimal_hit_rate * 100) << "%" << std::endl;
            }
            
            if (mode == "compare")
            {
                double lfu_hit_rate = testLFUCache(cache_size, requests);
                double optimal_hit_rate = testOptimalCache(cache_size, requests);
                double difference = optimal_hit_rate - lfu_hit_rate;
                
                std::cout << "\nHit rates:\n";
                std::cout << "LFU:     " << std::fixed << std::setprecision(2) << (lfu_hit_rate * 100) << "%" << std::endl;
                std::cout << "Optimal: " << std::fixed << std::setprecision(2) << (optimal_hit_rate * 100) << "%" << std::endl;
            }
        }
    }










    catch (const std::invalid_argument& e)
    {
        std::cerr << "Invalid argument - " << e.what() << std::endl;
        return -1;
    }

    catch (const ConfigurationException& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    catch (const CacheOperationException& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    catch (const BenchmarkException& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    catch (const std::exception& e)
    {
        std::cerr << "Unexpected exception - " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
