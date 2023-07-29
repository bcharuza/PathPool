#ifndef IMPL_PATH_POOL_UTILS_H
#define IMPL_PATH_POOL_UTILS_H

// wardens prevent cyclic inclusion
#include "../path_pool.h"

#include <vector>
#include <array>
#include <tuple>

template<typename PathPoolT>
std::vector<typename PathPoolT::tag_t>
get_taglist(const PathPoolT& pool,typename PathPoolT::pathid_t path)
{
  std::vector<typename PathPoolT::tag_t> result;  
  for(; path != pool.get_root(); path = pool.get_parent(path))
    result.push_back(pool.get_tag(path));
  result.push_back(pool.get_tag(path));
  return result;
}

template<typename PoolT>
std::array<typename PoolT::pathid_t,3>
get_common_path(typename PoolT::pathid_t l, typename PoolT::pathid_t r, PoolT& p)
{
  auto pl = p.get_parent(l);
  auto pr = p.get_parent(r);
  while( (pl != pr) & (pl != r) & (pr != l) )
    {
      if(pl > pr )
	std::tie(l,pl) = std::make_tuple(pl,p.get_parent(pl));
      else
	std::tie(r,pr) = std::make_tuple(pr,p.get_parent(pr));
    }
  pl = (pr == l)? l : pl;
  return {pl,l,r};
}

#endif /* IMPL_PATH_POOL_UTILS_H */
