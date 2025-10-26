#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <string>
#include <algorithm>

#include "LFUCache.h"
#include "OptimalCache.h"
#include "global.h"

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

std::vector<int> generateSequentialRequests(int num_requests, int num_pages)
{
    std::vector<int> requests;
    requests.reserve(num_requests);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, num_pages);
    
    for (int i = 0; i < num_requests; i++)
    {
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
    
    return requests;
}

double testLFUCache(int cache_size, const std::vector<int>& requests) 
{
    LFUCache<int, int> cache(cache_size, slow_get_page_int);
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
            cache.put(page);
        }
    }
    
    return static_cast<double>(hits) / requests.size();
}

double testOptimalCache(int cache_size, const std::vector<int>& requests) 
{
    OptimalCache optimal(cache_size, slow_get_page_int);
    optimal.preprocessRequests(requests);
    size_t hits = optimal.simulate(requests);
    return static_cast<double>(hits) / requests.size();
}

void printHelp()
{
    std::cout << "Usage:\n";
    std::cout << "  --mode=lfu              : Run only LFU cache simulation\n";
    std::cout << "  --mode=optimal          : Run only Optimal cache simulation\n";
    std::cout << "  --mode=compare          : Compare both cache algorithms (default)\n";
    std::cout << "  --requests=<number>     : Number of requests to generate\n";
    std::cout << "  --pages=<number>        : Number of unique pages\n";
    std::cout << "  --cache-size=<number>   : Cache size\n";
    std::cout << "  --request-type=random   : Generate random requests (default)\n";
    std::cout << "  --request-type=sequential : Generate sequential patterns\n";
    std::cout << "  --help                  : Show this help message\n\n";
}

int main(int argc, char* argv[])
{
    std::string mode = "compare";
    int num_requests = 1000;
    int num_pages = 100;
    int cache_size = 10;
    std::string request_type = "random";
    
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        
        if (arg == "--help")
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
            num_requests = std::stoi(arg.substr(11));
        }
        else if (arg.substr(0, 8) == "--pages=")
        {
            num_pages = std::stoi(arg.substr(8));
        }
        else if (arg.substr(0, 13) == "--cache-size=")
        {
            cache_size = std::stoi(arg.substr(13));
        }
        else if (arg.substr(0, 15) == "--request-type=")
        {
            request_type = arg.substr(15);
        }
        else
        {
            std::cerr << "Unknown argument: " << arg << std::endl;
            printHelp();
            return 1;
        }
    }
    
    if (num_requests <= 0 || num_pages <= 0 || cache_size < 0)
    {
        std::cerr << "Error: All numeric parameters must be positive, cache-size can be zero\n";
        return 1;
    }
    
    if (mode != "lfu" && mode != "optimal" && mode != "compare")
    {
        std::cerr << "Error: Invalid mode. Use 'lfu', 'optimal', or 'compare'\n";
        return 1;
    }

    std::cout << "Parameters:\n";
    std::cout << "  Mode: " << mode << "\n";
    std::cout << "  Requests: " << num_requests << "\n";
    std::cout << "  Pages: " << num_pages << "\n";
    std::cout << "  Cache size: " << cache_size << "\n";
    std::cout << "  Request type: " << request_type << "\n\n";
    
    std::cout << "Generating requests..." << std::endl;
    std::vector<int> requests;
    
    if (request_type == "sequential")
    {
        requests = generateSequentialRequests(num_requests, num_pages);
    }
    else
    {
        requests = generateRandomRequests(num_requests, num_pages);
    }
    
    if (mode == "lfu" || mode == "compare")
    {
        std::cout << "Testing LFU cache..." << std::endl;
        double lfu_hit_rate = testLFUCache(cache_size, requests);
        std::cout << "LFU cache hit rate: " << std::fixed << std::setprecision(2) 
                  << (lfu_hit_rate * 100) << "%" << std::endl;
    }
    
    if (mode == "optimal" || mode == "compare")
    {
        std::cout << "Testing Optimal cache..." << std::endl;
        double optimal_hit_rate = testOptimalCache(cache_size, requests);
        std::cout << "Optimal cache hit rate: " << std::fixed << std::setprecision(2) 
                  << (optimal_hit_rate * 100) << "%" << std::endl;
    }
    
    return 0;
}
