#include <gtest/gtest.h> //TODO - написать в readme
#include <vector>
#include "LFUCache.h"
#include "global.h"

//TODO - сделать возможность тестирования по последовательности запросов из файла

using namespace testing;

class LFUCacheTest : public Test
{
protected:
    void SetUp() override {}
    
    void TearDown() override {}
};

TEST_F(LFUCacheTest, Basic)
{
    lfu::LFUCache<int, int> cache(2, slow_get_page_int);
    
    cache.put(1);
    EXPECT_NO_THROW(cache.get(1));
    EXPECT_EQ(cache.get(1), 1);
    //std::cout << cache.get(1);
}

TEST_F(LFUCacheTest, Eviction)
{
    lfu::LFUCache<int, int> cache(2, slow_get_page_int);
    
    cache.put(1);
    cache.put(2);
    cache.put(3);
    
    EXPECT_EQ(cache.size(), 2);
}

TEST_F(LFUCacheTest, ClearCache) 
{
    lfu::LFUCache<int, int> cache(3, slow_get_page_int);
    
    cache.put(1);
    cache.put(2);
    cache.put(3);
    
    EXPECT_EQ(cache.size(), 3);
    
    cache.clear();
    
    EXPECT_EQ(cache.size(), 0);
    EXPECT_TRUE(cache.empty());
}


