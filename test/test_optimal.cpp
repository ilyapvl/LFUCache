#include <gtest/gtest.h>
#include <vector>
#include "OptimalCache.h"
#include "global.h"


//TODO - сделать возможность тестирования по последовательности запросов из файла


using namespace testing;

class OptimalCacheTest : public Test
{
protected:
    void SetUp() override {}
    
    void TearDown() override {}
};

TEST_F(OptimalCacheTest, Basic)
{
    opt::OptimalCache<int, int> cache(2, slow_get_page_int);
    std::vector<int> requests = {1, 2, 1, 3, 2};
    
    cache.preprocessRequests(requests);
    size_t hits = cache.simulate(requests);
    //std::cout << cache.get(3);
    
    EXPECT_GT(hits, 0);
}

