#ifndef IMPL_PATH_H
#define IMPL_PATH_H

// wardens prevent cyclic inclusion
#include "../path.h"

//////////////////////
// Global Functions //
//////////////////////
namespace {
  template<typename PoolT>
  typename std::enable_if<std::is_convertible<typename PoolT::pathid_t, size_t>::value,
			  size_t>::type
  convert_to_size_t(typename PoolT::pathid_t arg)
  {
    return static_cast<size_t>(arg);
  }

  template<typename PoolT>
  typename std::enable_if<!std::is_convertible<typename PoolT::pathid_t, size_t>::value,
			  size_t>::type
  convert_to_size_t(typename PoolT::pathid_t arg)
  {
    static_assert(std::is_convertible<typename PoolT::pathid_t, size_t>::value,
		  "PoolT::pathid_t cannot be converted to size_t");
    return 0;
  }
}
////////////////
// Path class //
////////////////
template<typename PoolT,int poolno>
PoolT Path<PoolT,poolno>::s_pool {};

template<typename PoolT,int poolno>
inline
Path<PoolT,poolno>::
Path():
  m_path{s_pool.get_root()}
{}

template<typename PoolT,int poolno>
inline
Path<PoolT,poolno>::
Path(const Path& parent,const tag_t& name):
  m_path{ s_pool.get_subnode(parent.m_path, name)}
{}

template<typename PoolT,int poolno>
inline
Path<PoolT,poolno>::
Path(pathid_t p):
  m_path{ p }
{}

template<typename PoolT,int poolno>
inline
Path<PoolT,poolno>
Path<PoolT,poolno>::
operator /(const tag_t& t) const
{
  return Path { *this, t };
}

template<typename PoolT,int poolno>
inline
size_t
Path<PoolT,poolno>::
get_hash() const
{
  return convert_to_size_t<PoolT>(m_path);
}

template<typename PoolT,int poolno>
inline
std::pair<typename Path<PoolT,poolno>::horizontal_iterator,
	  typename Path<PoolT,poolno>::horizontal_iterator>
Path<PoolT,poolno>::
get_subpaths() const 
{
  auto subpaths = s_pool.get_subnodes(m_path);
  return std::make_pair(horizontal_iterator{subpaths.first}, horizontal_iterator{subpaths.second});
}

template<typename PoolT,int poolno>
inline
Path<PoolT,poolno>
Path<PoolT,poolno>::get_parent() const
{
  return Path{ s_pool.get_parent(m_path) };
}

template<typename PoolT,int poolno>
inline
typename Path<PoolT,poolno>::tag_t
Path<PoolT,poolno>::get_tag() const
{
  return s_pool.get_tag(m_path);
}

template<typename PoolT,int poolno>
inline
typename Path<PoolT,poolno>::vertical_iterator
Path<PoolT,poolno>::begin() const
{
  return vertical_iterator {*this};
}

template<typename PoolT,int poolno>
inline
typename Path<PoolT,poolno>::vertical_iterator
Path<PoolT,poolno>::cbegin() const
{
  return begin();
}

template<typename PoolT,int poolno>
inline
typename Path<PoolT,poolno>::vertical_iterator
Path<PoolT,poolno>::end() const
{
  return vertical_iterator {};
}

template<typename PoolT,int poolno>
inline
typename Path<PoolT,poolno>::vertical_iterator
Path<PoolT,poolno>::cend() const
{
  return end();
}

/////////////////////////
// horizontal_iterator //
/////////////////////////
template<typename PoolT,int poolno>
class Path<PoolT,poolno>::horizontal_iterator
{
  friend Path;
  using it_t = typename PoolT::iterator_t;
public:
  using iterator_category = std::forward_iterator_tag;
  using value_type = Path;
  using difference_type = size_t;
  using pointer = value_type*;
  using reference = value_type&;
public:
  value_type operator *()
  {
    return Path{*m_it};
  }
  bool operator ==(horizontal_iterator const& r) const
  {
    return m_it == r.m_it;
  }
  bool operator !=(horizontal_iterator const& r) const
  {
    return !(*this == r);
  }
  horizontal_iterator& operator ++()
  {
    ++m_it;
    return *this;
  }
  horizontal_iterator operator ++(int)
  {
    return horizontal_iterator{m_it++};
  }
private:
  explicit
  horizontal_iterator(it_t it):
    m_it{it}
  {}
  typename PoolT::iterator_t m_it;
};

///////////////////////
// vertical_iterator //
///////////////////////
template<typename PoolT,int poolno>
class Path<PoolT,poolno>::vertical_iterator
{
  using it_t = typename PoolT::iterator_t;
public:
  using iterator_category = std::forward_iterator_tag;
  using value_type = Path;
  using difference_type = size_t;
  using pointer = value_type*;
  using reference = value_type&;
public:
  explicit
  vertical_iterator(Path path):
    m_it{path}
  {}
  vertical_iterator()
  {}
  value_type operator *()
  {
    return m_it;
  }
  bool operator ==(vertical_iterator const& r) const
  {
    return m_it == r.m_it;
  }
  bool operator !=(vertical_iterator const& r) const
  {
    return !(*this == r);
  }
  vertical_iterator& operator ++()
  {
    this->m_it = this->m_it.get_parent();
    return *this;
  }
  vertical_iterator operator ++(int)
  {
    vertical_iterator result = *this;
    ++*this;
    return result;
  }
protected:
  Path m_it {};
};

#endif /* IMPL_PATH_H */
