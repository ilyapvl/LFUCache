#include <gtest/gtest.h>
#include <vector>
#include "LFUCache.h"
#include "global.h"

using namespace testing;

class LFUCacheTest : public Test
{
protected:
    void SetUp() override
    {
    }
    
    void TearDown() override
    {
    }
};

TEST_F(LFUCacheTest, BasicPutAndGet)
{
    LFUCache<int, int> cache(2, slow_get_page_int);
    
    cache.put(1);
    EXPECT_NO_THROW(cache.get(1));
    EXPECT_EQ(cache.get(1), 1);
}

TEST_F(LFUCacheTest, CacheMissThrowsException)
{
    LFUCache<int, int> cache(2, slow_get_page_int);
    
    cache.put(1);
    EXPECT_THROW(cache.get(2), std::out_of_range);
}

TEST_F(LFUCacheTest, EvictionPolicy)
{
    LFUCache<int, int> cache(2, slow_get_page_int);
    
    cache.put(1);
    cache.put(2);
    cache.put(3);
    
    EXPECT_EQ(cache.size(), 2);
}

TEST_F(LFUCacheTest, FrequencyTracking)
{
    LFUCache<int, int> cache(2, slow_get_page_int);
    
    cache.put(1);
    cache.put(2);
    
    EXPECT_NO_THROW(cache.get(1));
    EXPECT_NO_THROW(cache.get(1));
    
    cache.put(3);
    
    EXPECT_NO_THROW(cache.get(1)); 
    EXPECT_THROW(cache.get(2), std::out_of_range); 
}

TEST_F(LFUCacheTest, ClearCache)
{
    LFUCache<int, int> cache(3, slow_get_page_int);
    
    cache.put(1);
    cache.put(2);
    cache.put(3);
    
    EXPECT_EQ(cache.size(), 3);
    
    cache.clear();
    
    EXPECT_EQ(cache.size(), 0);
    EXPECT_TRUE(cache.empty());
}

TEST_F(LFUCacheTest, CapacityMethods)
{
    LFUCache<int, int> cache(5, slow_get_page_int);
    
    EXPECT_EQ(cache.capacity(), 5);
    EXPECT_TRUE(cache.empty());
    
    cache.put(1);
    EXPECT_FALSE(cache.empty());
    EXPECT_EQ(cache.size(), 1);
}
