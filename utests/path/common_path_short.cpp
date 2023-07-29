#include "path.h"
#include "path_pool.h"

#include <gtest/gtest.h>

using namespace std;
using namespace testing;

namespace {
  using types = Types<Path<ListPathPool<int>>,Path<HashPathPool<int>>>;

  template<typename T>
  class CommonPath : public Test
  {
  public:
    using TestObjT = T;
    using tag_t = typename TestObjT::tag_t;
  public:
    void SetUp() override
    {
      r_t1 = root / tag1;
      r_t2 = root / tag2;
      r_t1_t1 = r_t1 / tag1;
      r_t1_t2 = r_t1 / tag2;
      r_t2_t1 = r_t2 / tag1;
      r_t2_t2 = r_t2 / tag2;
      r_t1_t1_t1 = r_t1_t1 / tag1;
      r_t1_t1_t2 = r_t1_t1 / tag2;
      r_t1_t2_t1 = r_t1_t2 / tag1;
      r_t1_t2_t2 = r_t1_t2 / tag2;
      r_t2_t1_t1 = r_t2_t1 / tag1;
      r_t2_t1_t2 = r_t2_t1 / tag2;
      r_t2_t2_t1 = r_t2_t2 / tag1;
      r_t2_t2_t2 = r_t2_t2 / tag2;
      // sequence does matter
      paths = { root,
		r_t1,
		r_t1_t1, r_t1_t1_t1, r_t1_t1_t2,
		r_t1_t2, r_t1_t2_t1, r_t1_t2_t2,
		r_t2,
		r_t2_t1, r_t2_t1_t1, r_t2_t1_t2,
		r_t2_t2, r_t2_t2_t1, r_t2_t2_t2 };
    }
  public:
    tag_t tag1 {1};
    tag_t tag2 {2};
    TestObjT root {};
    TestObjT r_t1;
    TestObjT r_t2;
    TestObjT r_t1_t1;
    TestObjT r_t1_t2;
    TestObjT r_t2_t1;
    TestObjT r_t2_t2;
    TestObjT r_t1_t1_t1;
    TestObjT r_t1_t1_t2;
    TestObjT r_t1_t2_t1;
    TestObjT r_t1_t2_t2;
    TestObjT r_t2_t1_t1;
    TestObjT r_t2_t1_t2;
    TestObjT r_t2_t2_t1;
    TestObjT r_t2_t2_t2;
    std::vector<TestObjT> paths;
  };
  TYPED_TEST_CASE(CommonPath, types);

  TYPED_TEST(CommonPath, RootToRoot)
  {
    auto result = common_path(this->root, this->root);
    ASSERT_EQ(result, this->root);
  }
  TYPED_TEST(CommonPath, RootToRT1)
  {
    auto result = common_path(this->root, this->r_t1);
    ASSERT_EQ( result, this->root );
  }
  TYPED_TEST(CommonPath, RT1ToRoot)
  {
    auto result = common_path(this->r_t1, this->root);
    ASSERT_EQ( result, this->root );
  }
  TYPED_TEST(CommonPath, RootToRT1T2)
  {
    auto result = common_path(this->root, this->r_t1_t2);
    ASSERT_EQ( result, this->root );
  }
  TYPED_TEST(CommonPath, RT1T2ToRoot)
  {
    auto result = common_path(this->r_t1_t2, this->root);
    ASSERT_EQ( result, this->root );
  }
  TYPED_TEST(CommonPath, RT2T2ToRoot)
  {
    auto result = common_path(this->r_t2_t2, this->root);
    ASSERT_EQ( result, this->root );
  }
  TYPED_TEST(CommonPath, RootToRT2T2)
  {
    auto result = common_path(this->root, this->r_t2_t2);
    ASSERT_EQ( result, this->root );
  }
  TYPED_TEST(CommonPath, RT1T1T1ToRT2T2T2)
  {
    auto result = common_path(this->r_t1_t1_t1, this->r_t2_t2_t2);
    ASSERT_EQ( result, this->root );
  }
  TYPED_TEST(CommonPath, RT1T2T1ToRT2T1T2)
  {
    auto result = common_path(this->r_t1_t1_t1, this->r_t2_t2_t2);
    ASSERT_EQ( result, this->root );
  }
  TYPED_TEST(CommonPath, RT1ToRT1T1T2)
  {
    auto result = common_path(this->r_t1, this->r_t1_t1_t2);
    ASSERT_EQ( result, this->r_t1 );
  }
  TYPED_TEST(CommonPath, RT1ToRT1T2T2)
  {
    auto result = common_path(this->r_t1, this->r_t1_t2_t2);
    ASSERT_EQ( result, this->r_t1 );
  }
  TYPED_TEST(CommonPath, RT1T1ToRT1T2)
  {
    auto result = common_path(this->r_t1_t1, this->r_t1_t2);
    ASSERT_EQ( result, this->r_t1 );
  }
  TYPED_TEST(CommonPath, RT1T1ToRT1T2T1)
  {
    auto result = common_path(this->r_t1_t1, this->r_t1_t2_t1);
    ASSERT_EQ( result, this->r_t1 );
  }
  TYPED_TEST(CommonPath, RT1T1T2ToRT1T2)
  {
    auto result = common_path(this->r_t1_t1_t2, this->r_t1_t2);
    ASSERT_EQ( result, this->r_t1 );
  }
}
