#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <algorithm>
#include "../include/skip_list.h"

using namespace stl_containers;

class SkipListTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize common test data
    }
};

TEST_F(SkipListTest, DefaultConstructor) {
    SkipList<int> list;
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0);
}

TEST_F(SkipListTest, InsertAndFind) {
    SkipList<int> list;
    
    // Insert elements
    auto result1 = list.insert(10);
    EXPECT_TRUE(result1.second);
    EXPECT_EQ(*result1.first, 10);
    
    auto result2 = list.insert(20);
    EXPECT_TRUE(result2.second);
    EXPECT_EQ(*result2.first, 20);
    
    auto result3 = list.insert(30);
    EXPECT_TRUE(result3.second);
    EXPECT_EQ(*result3.first, 30);
    
    // Duplicate insert
    auto result4 = list.insert(20);
    EXPECT_FALSE(result4.second);
    EXPECT_EQ(*result4.first, 20);
    
    // Find elements
    auto it1 = list.find(10);
    EXPECT_NE(it1, list.end());
    EXPECT_EQ(*it1, 10);
    
    auto it2 = list.find(20);
    EXPECT_NE(it2, list.end());
    EXPECT_EQ(*it2, 20);
    
    // Find non-existent element
    auto it4 = list.find(40);
    EXPECT_EQ(it4, list.end());
    
    // Check size
    EXPECT_EQ(list.size(), 3);
    EXPECT_FALSE(list.empty());
}

TEST_F(SkipListTest, Iterators) {
    SkipList<int> list;
    
    std::vector<int> values = {50, 30, 70, 20, 40, 60, 80};
    for (const auto& val : values) {
        list.insert(val);
    }
    
    // Sort the expected values (SkipList is ordered)
    std::sort(values.begin(), values.end());
    
    // Check forward iteration
    std::vector<int> actual;
    for (const auto& val : list) {
        actual.push_back(val);
    }
    
    EXPECT_EQ(actual, values);
}

TEST_F(SkipListTest, Erase) {
    SkipList<int> list;
    
    std::vector<int> values = {10, 20, 30, 40, 50};
    for (const auto& val : values) {
        list.insert(val);
    }
    
    EXPECT_EQ(list.size(), 5);
    
    // Erase by iterator
    auto it = list.find(30);
    list.erase(it);
    EXPECT_EQ(list.size(), 4);
    EXPECT_EQ(list.find(30), list.end());
    
    // Erase by value
    size_t erased = list.erase(10);
    EXPECT_EQ(erased, 1);
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.find(10), list.end());
    
    // Erase non-existent element
    erased = list.erase(99);
    EXPECT_EQ(erased, 0);
    EXPECT_EQ(list.size(), 3);
}

TEST_F(SkipListTest, Clear) {
    SkipList<int> list;
    
    std::vector<int> values = {10, 20, 30, 40, 50};
    for (const auto& val : values) {
        list.insert(val);
    }
    
    EXPECT_EQ(list.size(), 5);
    
    list.clear();
    EXPECT_EQ(list.size(), 0);
    EXPECT_TRUE(list.empty());
    
    // Make sure we can still use the list after clearing
    list.insert(100);
    EXPECT_EQ(list.size(), 1);
    EXPECT_FALSE(list.empty());
    EXPECT_NE(list.find(100), list.end());
}

TEST_F(SkipListTest, CopyConstructor) {
    SkipList<int> list1;
    std::vector<int> values = {10, 20, 30, 40, 50};
    for (const auto& val : values) {
        list1.insert(val);
    }
    
    // Copy construct
    SkipList<int> list2(list1);
    
    // Check sizes
    EXPECT_EQ(list1.size(), list2.size());
    
    // Check elements
    for (const auto& val : values) {
        EXPECT_NE(list2.find(val), list2.end());
    }
}

TEST_F(SkipListTest, ContainsAndEquality) {
    SkipList<int> list1 = {10, 20, 30};
    SkipList<int> list2 = {10, 20, 30};
    SkipList<int> list3 = {10, 20, 40};
    
    // Contains
    EXPECT_TRUE(list1.contains(10));
    EXPECT_TRUE(list1.contains(30));
    EXPECT_FALSE(list1.contains(15));
    
    // Equal lists
    EXPECT_TRUE(list1 == list2);
    EXPECT_FALSE(list1 != list2);
    
    // Different lists
    EXPECT_FALSE(list1 == list3);
    EXPECT_TRUE(list1 != list3);
}

TEST_F(SkipListTest, CustomCompare) {
    // Using greater for a descending-ordered container
    SkipList<int, std::greater<int>> list;
    
    std::vector<int> values = {10, 20, 30, 40, 50};
    for (const auto& val : values) {
        list.insert(val);
    }
    
    // Check the elements are in reverse order
    std::vector<int> expected = {50, 40, 30, 20, 10};
    std::vector<int> actual;
    for (const auto& val : list) {
        actual.push_back(val);
    }
    
    EXPECT_EQ(actual, expected);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 