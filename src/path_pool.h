#ifndef PATH_POOL_H
#define PATH_POOL_H

#include <vector>
#include <array>
#include <functional>

//////////////////////
// Global functions //
//////////////////////
template<typename PathPoolT>
std::vector<typename PathPoolT::tag_t>
get_taglist(const PathPoolT& pool,typename PathPoolT::pathid_t path);

template<typename PoolT>
std::array<typename PoolT::pathid_t,3>
get_common_path(typename PoolT::pathid_t l, typename PoolT::pathid_t r, PoolT& p);

////////////////////////
// HashPathPool class //
////////////////////////
template<typename TagT, template<typename T> class AllocatorT = std::allocator, typename HashF = std::hash<TagT>,typename EqualsF = std::equal_to<TagT>> 
class HashPathPool
{
  struct node_t;
  using ContainerT = typename std::vector<node_t,AllocatorT<node_t>>;
  using PoolT = HashPathPool<TagT,AllocatorT,HashF,EqualsF>;
public:
  // TagT is EmplaceConstructible - EmplaceConstructible is well formed
  // TagT must be Erasable, CopyInsertable, allow for: unordered_map::hasher(TagT), TagT == TagT
  static_assert(std::is_copy_constructible<TagT>(),"Template argument is not copy-constructible");
  class iterator_t;
  using tag_t = TagT;
  using pathid_t = typename ContainerT::size_type;
public:
  template <typename = typename std::enable_if<std::is_default_constructible<tag_t>::value>::type>
  HashPathPool();
  explicit
  HashPathPool(tag_t root);
  auto get_parent(pathid_t path) const -> pathid_t;
  auto get_root() const noexcept -> pathid_t ;
  auto get_subnode(pathid_t path,tag_t const& subnode) -> pathid_t;
  auto get_subnodes(pathid_t path) const -> std::pair<iterator_t,iterator_t>;
  template<typename ResultT>
  auto get_subnodes(pathid_t path, ResultT) const -> ResultT;
  auto get_tag(pathid_t path) const -> tag_t const& ;
private:
  static constexpr pathid_t null {0};
  static constexpr pathid_t rootid {0};
  ContainerT m_nodes;
};

////////////////////////
// ListPathPool class //
////////////////////////
template<typename TagT, template<typename T> class AllocatorT = std::allocator> 
class ListPathPool
{
  struct node_t;
  using ContainerT = typename std::vector<node_t,AllocatorT<node_t>>;
  using PoolT =  ListPathPool<TagT,AllocatorT>;
public:
  // TagT is EmplaceConstructible - EmplaceConstructible is well formed
  // TagT must be Erasable, CopyInsertable, allow for: unordered_map::hasher(TagT), TagT == TagT
  static_assert(std::is_copy_constructible<TagT>(),"Template argument is not copy-constructible");
  struct iterator_t;
  using tag_t = TagT;
  using pathid_t = typename ContainerT::size_type;
public:
  template <typename = typename std::enable_if<std::is_default_constructible<tag_t>::value>::type>
  ListPathPool();
  explicit
  ListPathPool(tag_t root);
  auto get_parent(pathid_t path) const -> pathid_t;
  auto get_root() const noexcept -> pathid_t;
  auto get_subnode(pathid_t path,tag_t const& subnode) -> pathid_t;
  auto get_subnodes(pathid_t path) const -> std::pair<iterator_t,iterator_t>;
  template<typename ResultT>
  auto get_subnodes(pathid_t path, ResultT) const -> ResultT;
  auto get_tag(pathid_t path) const -> tag_t const&;
private:
  auto insert_subnode(pathid_t path, tag_t const& subnode) -> pathid_t;
  auto get_next_sibling(pathid_t path) const -> pathid_t;
private:
  static constexpr pathid_t null {0};
  static constexpr pathid_t rootid {0};
  ContainerT m_nodes;
};

////////////////////
// Implementation //
////////////////////
#include "impl/impl_hash_path_pool.h"
#include "impl/impl_list_path_pool.h"
#include "impl/impl_path_pool_utils.h"

#endif /* PATH_POOL_H */
