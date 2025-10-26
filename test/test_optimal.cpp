#include <gtest/gtest.h>
#include <vector>
#include "OptimalCache.h"
#include "global.h"

using namespace testing;

class OptimalCacheTest : public Test
{
protected:
    void SetUp() override
    {
        // Common setup for all tests
    }
    
    void TearDown() override
    {
        // Clean up after each test
    }
};

TEST_F(OptimalCacheTest, BasicSimulation)
{
    OptimalCache cache(2, slow_get_page_int);
    std::vector<int> requests = {1, 2, 1, 3, 2};
    
    cache.preprocessRequests(requests);
    size_t hits = cache.simulate(requests);
    
    EXPECT_GT(hits, 0);
}

TEST_F(OptimalCacheTest, PerfectSequence)
{
    OptimalCache cache(3, slow_get_page_int);
    std::vector<int> requests = {1, 2, 3, 1, 2, 3, 1, 2, 3};
    
    cache.preprocessRequests(requests);
    size_t hits = cache.simulate(requests);
    
    // After first 3 misses, all should be hits
    EXPECT_EQ(hits, 6);
}

TEST_F(OptimalCacheTest, ZeroCacheSize)
{
    OptimalCache cache(0, slow_get_page_int);
    std::vector<int> requests = {1, 2, 3, 1, 2, 3};
    
    cache.preprocessRequests(requests);
    size_t hits = cache.simulate(requests);
    
    EXPECT_EQ(hits, 0);
}

TEST_F(OptimalCacheTest, ComplexEviction)
{
    OptimalCache cache(3, slow_get_page_int);
    // This sequence tests the optimal eviction strategy
    std::vector<int> requests = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
    
    cache.preprocessRequests(requests);
    size_t hits = cache.simulate(requests);
    
    EXPECT_GT(hits, 0);
}
