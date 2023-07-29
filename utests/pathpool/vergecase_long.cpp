#include "path_pool.h"
#include "utest.h"

#include <gtest/gtest.h>

using namespace std;
using namespace testing;
using types = Types<ListPathPool<int>,HashPathPool<int>>;

namespace {
  template<typename T>
  class VergeCaseLongestDepth : public Test
  {
  public:
    using TestObjT = T;
    using tag_t = typename TestObjT::tag_t;
    using pathid_t = typename TestObjT::pathid_t;
    void SetUp() override
    {
      range = {1,10000};
      auto path = test_obj.get_root();
      for(tag_t x = range[0]; x<range[1]; ++x)
	paths.push_back(path = test_obj.get_subnode(path, x));
    }
  public:
    TestObjT test_obj;
    vector<pathid_t> paths;
    std::array<tag_t,2> range;
  };
  
  TYPED_TEST_CASE(VergeCaseLongestDepth, types);
  
  TYPED_TEST(VergeCaseLongestDepth, IsConsistent)
  {
    using tag_t = typename TypeParam::tag_t;
    auto& test_obj = this->test_obj;
    auto path = test_obj.get_root();
    int i=0;
    for(tag_t x = this->range[0]; x< this->range[1]; ++x)
      {
	auto subnode = test_obj.get_subnode(path, x);
	ASSERT_EQ( subnode, this->paths[i++]);
	ASSERT_EQ( test_obj.get_parent(subnode), path);
	auto subs = test_obj.get_subnodes(path);
	ASSERT_TRUE( equals(subs,{subnode}) );
	path = subnode;
      }
  }
}
namespace {
  template<typename T>
  class VergeCaseShortestDepth : public Test
  {
  public:
    using TestObjT = T;
    using tag_t = typename TestObjT::tag_t;
    using pathid_t = typename TestObjT::pathid_t;
    void SetUp() override
    {
      range = {1,10000};
      auto root = test_obj.get_root();
      for(tag_t x = range[0]; x<range[1]; ++x)
	paths.push_back(test_obj.get_subnode(root, x));
    }
  public:
    TestObjT test_obj;
    vector<pathid_t> paths;
    std::array<tag_t,2> range;
  };
  
  TYPED_TEST_CASE(VergeCaseShortestDepth, types);

  TYPED_TEST(VergeCaseShortestDepth, ShortestDepth)
  {
    auto& test_obj = this->test_obj;
    auto subs = test_obj.get_subnodes(test_obj.get_root());
    ASSERT_EQ( std::distance(subs.first,subs.second), (this->range[1] - this->range[0]) );
    ASSERT_TRUE( contains_all(subs, this->paths) );

    auto root = test_obj.get_root();
    for( auto& x : this->paths)
      {
	auto subnode = test_obj.get_subnode(root, x);
	ASSERT_TRUE( empty( test_obj.get_subnodes(subnode) ) );
      }
  }
}
