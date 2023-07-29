#include "utest.h"
#include "path_pool.h"

#include <gtest/gtest.h>

using namespace std;
using namespace testing;
using types = Types<ListPathPool<int>,HashPathPool<int>>;

namespace {
  template<typename T>
  class IteratorEmptyRange : public Test
  {
  public:
    using TestObjT = T;
    using tag_t = typename TestObjT::tag_t;
    using path_t = typename TestObjT::pathid_t;
    void SetUp() override
    {}
  public:
    TestObjT test_obj;
  };
  TYPED_TEST_CASE(IteratorEmptyRange, types);
  
  TYPED_TEST(IteratorEmptyRange, BeginOverlapsEnd)
  {
    auto& test_obj = this->test_obj;
    auto root = test_obj.get_root();
    auto root_subs = test_obj.get_subnodes(root);
    ASSERT_EQ(root_subs.first,root_subs.second);
  }
  TYPED_TEST(IteratorEmptyRange, ConsecutiveCallsAreEqual)
  {
    auto& test_obj = this->test_obj;
    auto root = test_obj.get_root();
    auto root_subs = test_obj.get_subnodes(root);
    auto root_subs2 = test_obj.get_subnodes(root);
    ASSERT_EQ(root_subs2.first,root_subs.first);
    ASSERT_EQ(root_subs2.first,root_subs.second);
  }
}
namespace {
  template<typename T>
  class IteratorPopulatedRange : public Test
  {
  public:
    using TestObjT = T;
    using tag_t = typename TestObjT::tag_t;
    using pathid_t = typename TestObjT::pathid_t;
    void SetUp() override
    {
      auto root = test_obj.get_root();
      test_obj.get_subnode(root,1);
      test_obj.get_subnode(root,2);
    }
  public:
    TestObjT test_obj;
  };
  TYPED_TEST_CASE(IteratorPopulatedRange, types);
  
  TYPED_TEST(IteratorPopulatedRange, ConsecutiveCallsAreEqual)
  {
    auto& test_obj = this->test_obj;
    auto root = test_obj.get_root();
    auto root_subs = test_obj.get_subnodes(root);
    auto root_subs2 = test_obj.get_subnodes(root);
    ASSERT_EQ(root_subs.first,root_subs2.first);
    ASSERT_EQ(root_subs.second,root_subs2.second);
  }
  TYPED_TEST(IteratorPopulatedRange, PrefixIncrementWorks)
  {
    auto& test_obj = this->test_obj;
    auto root = test_obj.get_root();
    auto root_subs = test_obj.get_subnodes(root);
    auto it = root_subs.first;
    ASSERT_EQ(std::distance(root_subs.first,++it), 1);
    it = root_subs.first;
    ASSERT_EQ(std::distance(++it,root_subs.second), 1);
    it = root_subs.first;
    ASSERT_EQ(std::distance(root_subs.first,++(++it)), 2);    
    it = root_subs.first;
    ASSERT_EQ(++(++it),root_subs.second);
  }
  TYPED_TEST(IteratorPopulatedRange, PostfixIncrement)
  {
    auto& test_obj = this->test_obj;
    auto root = test_obj.get_root();
    auto root_subs = test_obj.get_subnodes(root);
    auto it = root_subs.first;
    ASSERT_EQ(std::distance(root_subs.first,it++), 0);
    ASSERT_EQ(std::distance(root_subs.first,it), 1);
    it = root_subs.first;
    ASSERT_EQ(std::distance(root_subs.first,it++), 0);
    ASSERT_EQ(std::distance(root_subs.first,it), 1);
    it = root_subs.first;
    ASSERT_EQ(std::distance(root_subs.first,(it++)++), 0);
    ASSERT_EQ(std::distance(root_subs.first,(it++)++), 1);
  }
}
