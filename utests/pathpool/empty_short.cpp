#include "path_pool.h"
#include "utest.h"

#include <gtest/gtest.h>

using namespace std;
using namespace testing;

namespace {
  template<typename T>
  class DefaultEmpty : public Test
  {
  public:
    using TestObjT = T;
    using tag_t = typename TestObjT::tag_t;
    using path_t = typename TestObjT::pathid_t;
    void SetUp() override
    {}
  public:
    TestObjT m_test_obj;
  };
  template<typename T>
  class NamedEmpty : public Test
  {
  public:
    using TestObjT = T;
    using tag_t = typename TestObjT::tag_t;
    using path_t = typename TestObjT::pathid_t;
    void SetUp() override
    {}
  public:
    tag_t name{11};
    TestObjT m_test_obj{name};
  };
  
  using types = Types<ListPathPool<int>,HashPathPool<int>>;
  TYPED_TEST_CASE(DefaultEmpty, types);
  TYPED_TEST_CASE(NamedEmpty, types);


  TYPED_TEST(DefaultEmpty, RootParentIsRoot)
  {
    auto& test_obj = this->m_test_obj;
    auto root = test_obj.get_root();
    auto root_parent = test_obj.get_parent(root);
    ASSERT_EQ(root,root_parent);
  }
  TYPED_TEST(NamedEmpty, RootNameIsAsSpecified)
  {
    auto& test_obj = this->m_test_obj;
    auto root = test_obj.get_root();
    auto root_tag = test_obj.get_tag(root);
    ASSERT_EQ(root_tag,this->name);
  }
  TYPED_TEST(DefaultEmpty, RootHasNoSiblings)
  {
    auto& test_obj = this->m_test_obj;
    auto root = test_obj.get_root();
    auto subnodes = test_obj.get_subnodes(root);
    ASSERT_EQ(std::distance(subnodes.first,subnodes.second), 0);
  }
  TYPED_TEST(DefaultEmpty, ProperRootInsertion)
  {
    using tag_t = typename TypeParam::tag_t;
    auto& test_obj = this->m_test_obj;
    tag_t tag {1};
    auto root = test_obj.get_root();
    auto subnode = test_obj.get_subnode(root, tag);

    ASSERT_EQ( test_obj.get_parent(subnode), root);
    ASSERT_EQ( test_obj.get_tag(subnode), tag);
    auto subnode_subs = test_obj.get_subnodes(subnode);
    ASSERT_TRUE(empty(subnode_subs));
    auto root_subs = test_obj.get_subnodes(root);
    ASSERT_TRUE(equals(root_subs, {tag}));
  }
  TYPED_TEST(DefaultEmpty, ProperRootInsertTwice)
  {
    using tag_t = typename TypeParam::tag_t;
    using pathid_t = typename TypeParam::pathid_t;
    auto& test_obj = this->m_test_obj;
    tag_t tag {1};
    tag_t tag2 {2};
    auto root = test_obj.get_root();
    auto subnode = test_obj.get_subnode(root, tag);
    auto subnode_2 = test_obj.get_subnode(root, tag2);

    ASSERT_EQ( test_obj.get_parent(subnode), root);
    ASSERT_EQ( test_obj.get_tag(subnode), tag);
    auto subnode_subs = test_obj.get_subnodes(subnode);
    ASSERT_TRUE(empty(subnode_subs));
    ASSERT_EQ( test_obj.get_parent(subnode_2), root);
    ASSERT_EQ( test_obj.get_tag(subnode_2), tag2);
    auto subnode_subs2 = test_obj.get_subnodes(subnode);
    ASSERT_TRUE(empty(subnode_subs2));
    auto root_subnodes = test_obj.get_subnodes(root);
    ASSERT_TRUE(contains_all(root_subnodes,vector<pathid_t>{subnode,subnode_2}));
  }
  TYPED_TEST(DefaultEmpty, ProperStackedInsertion)
  {
    using tag_t = typename TypeParam::tag_t;
    auto& test_obj = this->m_test_obj;
    tag_t tag {1};
    tag_t tag2 {2};
    auto root = test_obj.get_root();
    auto subnode = test_obj.get_subnode(root, tag);
    auto subnode2 = test_obj.get_subnode(subnode, tag2);

    auto root_subs = test_obj.get_subnodes(root);
    ASSERT_TRUE(equals(root_subs,{subnode}));
    auto subnode_subs = test_obj.get_subnodes(subnode);
    ASSERT_EQ( test_obj.get_parent(subnode), root);
    ASSERT_EQ( test_obj.get_tag(subnode), tag);
    ASSERT_TRUE(equals(root_subs,{subnode}));
    auto subnode2_subs = test_obj.get_subnodes(subnode2);
    ASSERT_EQ( test_obj.get_parent(subnode2), subnode);
    ASSERT_EQ( test_obj.get_tag(subnode2), tag2);
    ASSERT_TRUE(empty(subnode2_subs));
  }
  TYPED_TEST(DefaultEmpty, ProperRootInsertSameTwice)
  {
    using tag_t = typename TypeParam::tag_t;
    auto& test_obj = this->m_test_obj;
    tag_t tag {1};
    auto root = test_obj.get_root();
    auto subnode = test_obj.get_subnode(root, tag);
    auto subnode_2 = test_obj.get_subnode(root, tag);
   
    ASSERT_EQ(subnode,subnode_2);
    ASSERT_EQ( test_obj.get_parent(subnode), root);
    ASSERT_EQ( test_obj.get_tag(subnode), tag);
    auto subnode_subs = test_obj.get_subnodes(subnode);
    ASSERT_TRUE(empty(subnode_subs));
    auto root_subnodes = test_obj.get_subnodes(root);
    ASSERT_TRUE(equals(root_subnodes, {1}));
  }
}
