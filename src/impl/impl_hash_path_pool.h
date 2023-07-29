#ifndef IMPL_HASH_PATH_POOL_H
#define IMPL_HASH_PATH_POOL_H

#include "custom_iterators.h"
// wardens prevent cyclic inclusion
#include "../path_pool.h"

#include <unordered_map>
#include <vector>

//////////////////
// HashPathPool //
//////////////////
template<typename TagT, template<typename T> class AllocatorT, typename HashF,typename EqualsF>
template <typename>
inline
HashPathPool<TagT,AllocatorT,HashF,EqualsF>::
HashPathPool():
  HashPathPool(tag_t{})
{}


template<typename TagT, template<typename T> class AllocatorT, typename HashF,typename EqualsF>
inline
HashPathPool<TagT,AllocatorT,HashF,EqualsF>::
HashPathPool(tag_t root):
  m_nodes {{node_t{root,null,{}}}}
{}

template<typename TagT, template<typename T> class AllocatorT, typename HashF,typename EqualsF>
inline
typename HashPathPool<TagT,AllocatorT,HashF,EqualsF>::pathid_t
HashPathPool<TagT,AllocatorT,HashF,EqualsF>::
get_subnode(pathid_t path,tag_t const& subnode)
{
  auto result = m_nodes[path].emplace(subnode,m_nodes.size());
  if(result.second)
    m_nodes.push_back(node_t{subnode,path,{}});
  return result.first->second;
}

template<typename TagT, template<typename T> class AllocatorT, typename HashF,typename EqualsF>
inline
std::pair<typename HashPathPool<TagT,AllocatorT,HashF,EqualsF>::iterator_t,
	  typename HashPathPool<TagT,AllocatorT,HashF,EqualsF>::iterator_t >
HashPathPool<TagT,AllocatorT,HashF,EqualsF>::
get_subnodes(typename HashPathPool<TagT,AllocatorT,HashF,EqualsF>::pathid_t path) const
{
  using it_t = typename HashPathPool<TagT,AllocatorT,HashF,EqualsF>::iterator_t;
  return { it_t{m_nodes[path].cbegin()}, it_t{m_nodes[path].cend()}};
}

template<typename TagT, template<typename T> class AllocatorT, typename HashF,typename EqualsF>
template<typename ResultT>
inline
ResultT HashPathPool<TagT,AllocatorT,HashF,EqualsF>::
get_subnodes(typename HashPathPool<TagT,AllocatorT,HashF,EqualsF>::pathid_t path, ResultT result) const
{
  auto value_t_ref = &std::decay<decltype(*m_nodes[path].begin())>::type::second;
  std::copy(field_ref(m_nodes[path].begin(),value_t_ref),
	    field_ref(m_nodes[path].end(),value_t_ref),
	    std::back_inserter(result));
  return result;
}

template<typename TagT, template<typename T> class AllocatorT, typename HashF,typename EqualsF>
inline
typename HashPathPool<TagT,AllocatorT,HashF,EqualsF>::pathid_t
HashPathPool<TagT,AllocatorT,HashF,EqualsF>::
get_parent(pathid_t path) const
{
  return m_nodes[path].m_parent;
}

template<typename TagT, template<typename T> class AllocatorT, typename HashF,typename EqualsF>
inline
typename HashPathPool<TagT,AllocatorT,HashF,EqualsF>::tag_t const&
HashPathPool<TagT,AllocatorT,HashF,EqualsF>::
get_tag(pathid_t path) const
{
  return m_nodes[path].m_name;
}

template<typename TagT, template<typename T> class AllocatorT, typename HashF,typename EqualsF>
inline
typename HashPathPool<TagT,AllocatorT,HashF,EqualsF>::pathid_t
HashPathPool<TagT,AllocatorT,HashF,EqualsF>::
get_root() const noexcept
{
  return rootid;
}

//////////////////////////
// HashPathPool::node_t //
//////////////////////////
template<typename TagT, template<typename T> class AllocatorT, typename HashF,typename EqualsF> 
struct HashPathPool<TagT,AllocatorT,HashF,EqualsF>::node_t :
  public std::unordered_map<tag_t, pathid_t, HashF, EqualsF>
  {
    using MapT = std::unordered_map<tag_t, pathid_t, HashF, EqualsF>;
    node_t(tag_t name, pathid_t parent,const MapT& children):
      MapT{children},
      m_parent{parent},
      m_name{name}
    {}
    pathid_t m_parent;
    tag_t m_name;
  };


//////////////////////////////
// HashPathPool::iterator_t //
//////////////////////////////
template<typename TagT, template<typename T> class AllocatorT, typename HashF,typename EqualsF> 
class HashPathPool<TagT,AllocatorT,HashF,EqualsF>::iterator_t
  {
    using it_t = typename node_t::const_iterator;
  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = pathid_t;
    using difference_type = decltype (std::declval<value_type>() - std::declval<value_type>());
    using pointer = value_type*;
    using reference = value_type&;
  public:
    explicit
    iterator_t(it_t it):
      m_it{it}
    {}
    ~iterator_t(){};
    value_type operator *()
    {
      return m_it->second;
    }
    iterator_t& operator ++()
    {
      ++m_it;
      return *this;
    };
    iterator_t operator ++(int){
      iterator_t result = *this;
      ++*this;
      return result;
    }
    bool operator ==(iterator_t const& r) const
    {
      return m_it == r.m_it;
    }
    bool operator !=(iterator_t const& r) const
    {
      return m_it != r.m_it;
    }
  private:
    it_t m_it;
  };

#endif /* IMPL_HASH_PATH_POOL_H */
