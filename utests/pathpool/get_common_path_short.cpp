#include "path_pool.h"
#include "utest.h"

#include <gtest/gtest.h>

using namespace std;
using namespace testing;

using types = Types<ListPathPool<int>,HashPathPool<int>>;

namespace {
  template<typename T>
  class GetCommonPath : public Test
  {
  public:
    using TestObjT = T;
    using tag_t = typename TestObjT::tag_t;
    using path_t = typename TestObjT::pathid_t;
  public:
    void SetUp() override
    {
      root = test_obj.get_root();
      r_t1 = test_obj.get_subnode(root,tag1);
      r_t2 = test_obj.get_subnode(root,tag2);
      r_t1_t1 = test_obj.get_subnode(r_t1,tag1);
      r_t1_t2 = test_obj.get_subnode(r_t1,tag2);
      r_t2_t1 = test_obj.get_subnode(r_t2,tag1);
      r_t2_t2 = test_obj.get_subnode(r_t2,tag2);
      r_t1_t1_t1 = test_obj.get_subnode(r_t1_t1,tag1);
      r_t1_t1_t2 = test_obj.get_subnode(r_t1_t1,tag2);
      r_t1_t2_t1 = test_obj.get_subnode(r_t1_t2,tag1);
      r_t1_t2_t2 = test_obj.get_subnode(r_t1_t2,tag2);
      r_t2_t1_t1 = test_obj.get_subnode(r_t2_t1,tag1);
      r_t2_t1_t2 = test_obj.get_subnode(r_t2_t1,tag2);
      r_t2_t2_t1 = test_obj.get_subnode(r_t2_t2,tag1);
      r_t2_t2_t2 = test_obj.get_subnode(r_t2_t2,tag2);
    }
  public:
    TestObjT test_obj {0};
    tag_t tag1 {1};
    tag_t tag2 {2};
    path_t root;
    path_t r_t1;
    path_t r_t2;
    path_t r_t1_t1;
    path_t r_t1_t2;
    path_t r_t2_t1;
    path_t r_t2_t2;
    path_t r_t1_t1_t1;
    path_t r_t1_t1_t2;
    path_t r_t1_t2_t1;
    path_t r_t1_t2_t2;
    path_t r_t2_t1_t1;
    path_t r_t2_t1_t2;
    path_t r_t2_t2_t1;
    path_t r_t2_t2_t2;
  };
  TYPED_TEST_CASE(GetCommonPath, types);

  TYPED_TEST(GetCommonPath, RootToRoot)
  {
    auto result = get_common_path(this->root, this->root, this->test_obj);
    ASSERT_TRUE( equals(result, {this->root,this->root,this->root}) );
  }
  TYPED_TEST(GetCommonPath, RootToRT1)
  {
    auto result = get_common_path(this->root, this->r_t1, this->test_obj);
    ASSERT_TRUE( equals(result, {this->root,this->root,this->r_t1}) );
  }
  TYPED_TEST(GetCommonPath, RT1ToRoot)
  {
    auto result = get_common_path(this->r_t1, this->root, this->test_obj);
    ASSERT_TRUE( equals(result, {this->root,this->r_t1,this->root}) );
  }
  TYPED_TEST(GetCommonPath, RootToRT1T2)
  {
    auto result = get_common_path(this->root, this->r_t1_t2, this->test_obj);
    ASSERT_TRUE( equals(result, {this->root,this->root,this->r_t1}) );
  }
  TYPED_TEST(GetCommonPath, RT1T2ToRoot)
  {
    auto result = get_common_path(this->r_t1_t2, this->root, this->test_obj);
    ASSERT_TRUE( equals(result, {this->root,this->r_t1,this->root}) );
  }
  TYPED_TEST(GetCommonPath, RT2T2ToRoot)
  {
    auto result = get_common_path(this->r_t2_t2, this->root, this->test_obj);
    ASSERT_TRUE( equals(result, {this->root,this->r_t2,this->root}) );
  }
  TYPED_TEST(GetCommonPath, RootToRT2T2)
  {
    auto result = get_common_path(this->root, this->r_t2_t2, this->test_obj);
    ASSERT_TRUE( equals(result, {this->root,this->root,this->r_t2}) );
  }
  TYPED_TEST(GetCommonPath, RT1T1T1ToRT2T2T2)
  {
    auto result = get_common_path(this->r_t1_t1_t1, this->r_t2_t2_t2, this->test_obj);
    ASSERT_TRUE( equals(result, {this->root,this->r_t1,this->r_t2}) );
  }
  TYPED_TEST(GetCommonPath, RT1T2T1ToRT2T1T2)
  {
    auto result = get_common_path(this->r_t1_t1_t1, this->r_t2_t2_t2, this->test_obj);
    ASSERT_TRUE( equals(result, {this->root,this->r_t1,this->r_t2}) );
  }
  TYPED_TEST(GetCommonPath, RT1ToRT1T1T2)
  {
    auto result = get_common_path(this->r_t1, this->r_t1_t1_t2, this->test_obj);
    ASSERT_TRUE( equals(result, {this->r_t1,this->r_t1,this->r_t1_t1}) );
  }
  TYPED_TEST(GetCommonPath, RT1ToRT1T2T2)
  {
    auto result = get_common_path(this->r_t1, this->r_t1_t2_t2, this->test_obj);
    ASSERT_TRUE( equals(result, {this->r_t1,this->r_t1,this->r_t1_t2}) );
  }
  TYPED_TEST(GetCommonPath, RT1T1ToRT1T2)
  {
    auto result = get_common_path(this->r_t1_t1, this->r_t1_t2, this->test_obj);
    ASSERT_TRUE( equals(result, {this->r_t1,this->r_t1_t1,this->r_t1_t2}) );
  }
  TYPED_TEST(GetCommonPath, RT1T1ToRT1T2T1)
  {
    auto result = get_common_path(this->r_t1_t1, this->r_t1_t2_t1, this->test_obj);
    ASSERT_TRUE( equals(result, {this->r_t1,this->r_t1_t1,this->r_t1_t2}) );
  }
  TYPED_TEST(GetCommonPath, RT1T1T2ToRT1T2)
  {
    auto result = get_common_path(this->r_t1_t1_t2, this->r_t1_t2, this->test_obj);
    ASSERT_TRUE( equals(result, {this->r_t1,this->r_t1_t1,this->r_t1_t2}) );
  }
}
