#include "path_pool.h"
#include "utest.h"

#include <gtest/gtest.h>
#include <functional>
#include <random>

using namespace std;
using namespace testing;
using types = Types<std::pair<ListPathPool<int>,HashPathPool<int>>>;

namespace {
  template<typename T>
  class CrossCompare : public Test
  {
  public:
    using TestObjT1 = typename T::first_type;
    using TestObjT2 = typename T::second_type;
    static_assert(std::is_same<typename TestObjT1::tag_t,typename TestObjT2::tag_t>(),"tag_t missmatch - comparison is not possible");
    static_assert(std::is_same<typename TestObjT1::pathid_t,typename TestObjT2::pathid_t>(),"pathid_t missmatch - comparison is not possible");
    using tag_t = typename TestObjT1::tag_t;
    using pathid_t = typename TestObjT1::pathid_t;
    void SetUp() override
    {
      auto rand = std::bind(std::uniform_int_distribution<size_t>{0}, std::default_random_engine{});
      for( size_t i =1; i< 10000; ++i)
	{
	  int index = rand() % i;
	  auto first = test_obj1.get_subnode(index, i);
	  auto second  = test_obj2.get_subnode(index, i);
	  paths.push_back({first,second});
	}
    }
  public:
    TestObjT1 test_obj1;
    TestObjT2 test_obj2;
    vector<std::pair<pathid_t,pathid_t>> paths;
    std::vector<pathid_t> subs1, subs2;
  };
  TYPED_TEST_CASE(CrossCompare, types);
  
  TYPED_TEST(CrossCompare, RandomInsertion)
  {
    using pathid_t = typename TypeParam::first_type::pathid_t;
    auto& test_obj1 = this->test_obj1;
    auto& test_obj2 = this->test_obj2;
    for( auto& x : this->paths )
      {
	auto first = x.first;
	auto second = x.second;
	ASSERT_EQ(test_obj1.get_tag(first), test_obj2.get_tag(second));
	ASSERT_EQ(test_obj1.get_parent(first), test_obj2.get_parent(second));
	auto subs1 = test_obj1.get_subnodes(first,vector<pathid_t>{});
	auto subs2 = test_obj2.get_subnodes(second,vector<pathid_t>{});
	ASSERT_EQ(subs1.size(),subs2.size());
	std::stable_sort(subs1.begin(),subs1.end());
	std::stable_sort(subs2.begin(),subs2.end());
	ASSERT_TRUE( equals(subs1, subs2));
      }
  };
}
