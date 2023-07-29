#include "utest.h"
#include "path.h"
#include "path_pool.h"

#include <gtest/gtest.h>

using namespace std;
using namespace testing;

namespace {
  using types = Types<Path<ListPathPool<int>>,Path<HashPathPool<int>>>;
  
  template<typename T>
  class IteratorEmptyRange : public Test
  {
  public:
    using TestObjT = T;
    using tag_t = typename TestObjT::tag_t;
  public:
    TestObjT root;
  };
  TYPED_TEST_CASE(IteratorEmptyRange, types);
  
  TYPED_TEST(IteratorEmptyRange, BeginOverlapsEnd)
  {
    ASSERT_EQ(this->root.begin(),this->root.end());
  }
  TYPED_TEST(IteratorEmptyRange, ConsecutiveBeginsAreEqual)
  {
    auto it1 = this->root.begin();
    auto it2 = this->root.begin();
    ASSERT_EQ(it1,it2);
  }
  TYPED_TEST(IteratorEmptyRange, ConsecutiveEndsAreEqual)
  {
    auto it1 = this->root.end();
    auto it2 = this->root.end();
    ASSERT_EQ(it1,it2);
  }
  TYPED_TEST(IteratorEmptyRange, BeginRetursPath)
  {
    auto it1 = this->root.begin();
    ASSERT_EQ(*it1, TypeParam{});
  }
}

namespace {
  template<typename T>
  class IteratorPopulatedRange : public Test
  {
  public:
    using TestObjT = T;
    using tag_t = typename TestObjT::tag_t;
  public:
    TestObjT root;
    TestObjT r_t1 {root,1};
    TestObjT r_t2 {r_t1,2};
  };
  TYPED_TEST_CASE(IteratorPopulatedRange, types);
  
  TYPED_TEST(IteratorPopulatedRange, ConsecutiveBeginsAreEqual)
  {
    ASSERT_EQ(this->root.begin(),this->root.begin());
    ASSERT_EQ(this->r_t1.begin(),this->r_t1.begin());
    ASSERT_EQ(this->r_t2.begin(),this->r_t2.begin());
  }
  TYPED_TEST(IteratorPopulatedRange, ConsecutiveEndsAreEqual)
  {
    ASSERT_EQ(this->root.end(),this->root.end());
    ASSERT_EQ(this->r_t1.end(),this->r_t1.end());
    ASSERT_EQ(this->r_t2.end(),this->r_t2.end());
  }
  TYPED_TEST(IteratorPopulatedRange, AllEndsAreEqual)
  {
    ASSERT_EQ(this->root.end(),this->r_t1.end());
    ASSERT_EQ(this->r_t1.end(),this->r_t2.end());
  }
  TYPED_TEST(IteratorPopulatedRange, PrefixIncrementWorks)
  {
    ASSERT_EQ(++this->root.begin(), this->root.end());
    ASSERT_EQ(++this->r_t1.begin(), this->root.begin());
    ASSERT_EQ(++this->r_t2.begin(), this->r_t1.begin());
  }
  TYPED_TEST(IteratorPopulatedRange, ReturnsCorrectPath)
  {
    ASSERT_EQ(*this->root.begin(), this->root);
    ASSERT_EQ(*this->r_t1.begin(), this->r_t1);
    ASSERT_EQ(*++this->r_t1.begin(), this->root);
    ASSERT_EQ(*this->r_t2.begin(), this->r_t2);
    ASSERT_EQ(*++this->r_t2.begin(), this->r_t1);
  }
  TYPED_TEST(IteratorPopulatedRange, PostfixIncrementWorks)
  {
    auto root_it = this->root.begin();
    auto it1 = this->r_t1.begin()++;
    auto it2 = this->r_t2.begin()++;
    ASSERT_EQ(root_it++, this->root.begin());
    ASSERT_EQ(it1++, this->r_t1.begin());
    ASSERT_EQ(it2++, this->r_t2.begin());
    // after incrementation
    ASSERT_EQ(root_it, this->root.end());
    ASSERT_EQ(it1, this->root.begin());
    ASSERT_EQ(it2, this->r_t1.begin());
  }
}
