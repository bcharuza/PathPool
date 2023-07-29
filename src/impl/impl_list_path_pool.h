#ifndef LIST_PATH_POOL_H
#define IMPL_LIST_PATH_POOL_H

// wardens prevent cyclic inclusion
#include "../path_pool.h"

#include <vector>

//////////////////
// ListPathPool //
//////////////////
template<typename TagT, template<typename T> class AllocatorT> 
template <typename>
inline
ListPathPool<TagT,AllocatorT>::
ListPathPool():
  ListPathPool(ListPathPool<TagT,AllocatorT>::tag_t {})
{}

template<typename TagT, template<typename T> class AllocatorT>
inline
ListPathPool<TagT,AllocatorT>::
ListPathPool(tag_t root):
  m_nodes {{node_t{root,null,null,null}}}
{}

template<typename TagT, template<typename T> class AllocatorT>
inline
typename ListPathPool<TagT,AllocatorT>::pathid_t
ListPathPool<TagT,AllocatorT>::
get_subnode(pathid_t path,tag_t const& subnode)
{
  pathid_t node = path;
  for(node = m_nodes[node].m_child; node != null; node = m_nodes[node].m_sibling)
    if(get_tag(node) == subnode)
      return node;
  return insert_subnode(path,subnode);
}

template<typename TagT, template<typename T> class AllocatorT> 
inline
std::pair<typename ListPathPool<TagT,AllocatorT>::iterator_t,
	  typename ListPathPool<TagT,AllocatorT>::iterator_t>
ListPathPool<TagT,AllocatorT>::
get_subnodes(pathid_t path) const
{
  return { iterator_t{m_nodes[path].m_child, *this } , iterator_t{ null, *this}  };
}

template<typename TagT, template<typename T> class AllocatorT> 
template<typename ResultT>
inline
ResultT
ListPathPool<TagT,AllocatorT>::
get_subnodes(pathid_t path, ResultT result) const
{
  for(path = m_nodes[path].m_child; path != null; path = m_nodes[path].m_sibling)
    result.push_back(path);
  return result;
}

template<typename TagT, template<typename T> class AllocatorT> 
inline
typename ListPathPool<TagT,AllocatorT>::pathid_t
ListPathPool<TagT,AllocatorT>::
get_parent(pathid_t path) const
{
  return m_nodes[path].m_parent;
}

template<typename TagT, template<typename T> class AllocatorT> 
inline
typename ListPathPool<TagT,AllocatorT>::tag_t const&
ListPathPool<TagT,AllocatorT>::
get_tag(pathid_t path) const
{
  return m_nodes[path].m_name;
}

template<typename TagT, template<typename T> class AllocatorT> 
inline
typename ListPathPool<TagT,AllocatorT>::pathid_t
ListPathPool<TagT,AllocatorT>::
get_root() const noexcept
{
  return rootid;
}

template<typename TagT, template<typename T> class AllocatorT> 
inline
typename ListPathPool<TagT,AllocatorT>::pathid_t
ListPathPool<TagT,AllocatorT>::
insert_subnode(pathid_t path, tag_t const& subnode)
{
  pathid_t i = m_nodes.size();
  m_nodes.push_back(node_t{subnode,path,m_nodes[path].m_child,null});
  return m_nodes[path].m_child = i;
}

template<typename TagT, template<typename T> class AllocatorT> 
inline
typename ListPathPool<TagT,AllocatorT>::pathid_t
ListPathPool<TagT,AllocatorT>::
get_next_sibling(pathid_t path) const
{
  return m_nodes[path].m_sibling;
}
//////////////////////////
// ListPathPool::node_t //
//////////////////////////
template<typename TagT, template<typename T> class AllocatorT> 
struct ListPathPool<TagT,AllocatorT>::node_t
{
  node_t(tag_t name, pathid_t parent,pathid_t sibling, pathid_t child):
    m_child{child},
    m_sibling{sibling},
    m_parent{parent},
    m_name{name}
  {}
  pathid_t m_child;
  pathid_t m_sibling;
  pathid_t m_parent;
  tag_t m_name;
};
//////////////////////////////
// ListPathPool::iterator_t //
//////////////////////////////
template<typename TagT, template<typename T> class AllocatorT> 
struct ListPathPool<TagT,AllocatorT>::iterator_t
{
  using iterator_category = std::forward_iterator_tag;
  using value_type = pathid_t;
  using difference_type = decltype (std::declval<value_type>() - std::declval<value_type>());
  using pointer = value_type*;
  using reference = value_type&;
public:
  explicit
  iterator_t(value_type val,const PoolT& pool):
    m_pool{&pool},
    m_val{val}
  {}
  ~iterator_t()
  {};
  value_type operator *()
  {
    return m_val;
  }
  iterator_t& operator ++()
  {
    m_val = m_pool->get_next_sibling(m_val);
    return *this;
  };
  iterator_t operator ++(int)
  {
    iterator_t result = *this;
    ++*this;
    return result;
  }
  bool operator ==(iterator_t const& r) const
  {
    return (m_val == r.m_val) & (m_pool == r.m_pool);
  }
  bool operator !=(iterator_t const& r) const
  {
    return !(*this == r);
  }
private:
  const PoolT* m_pool;
  value_type m_val;
};

#endif /* IMPL_LIST_PATH_POOL_H */
