#ifndef BOOSTPATH_H
#define BOOSTPATH_H

#include <boost/filesystem.hpp>

class BoostPath;

bool operator == (BoostPath const& l, BoostPath const& r);
bool operator < (BoostPath const& l, BoostPath const& r);

class BoostPath
{
  using path_t = boost::filesystem::path;
public:
  class vertical_iterator;
public:
  explicit
  BoostPath()
  {}
  BoostPath(BoostPath const& parent,std::string const& name):
    m_path { parent.m_path / name }
  {}
  auto operator /(std::string const& t) const -> BoostPath
  {
    return BoostPath { m_path, t };
  }
  auto get_parent() const -> BoostPath
  {
    return BoostPath{ m_path.parent_path() };
  }
  auto get_tag() const -> std::string
  {
    return m_path.filename().string();
  }
  auto begin()
  {
    return m_path.begin();
  }
  auto end()
  {
    return m_path.end();
  }
  auto cbegin() const
  {
    return m_path.begin();
  }
  auto cend() const
  {
    return m_path.end();
  }
private:
  path_t m_path;
private:
  BoostPath(path_t const& parent,std::string const& name):
    m_path { parent / name }
  {}
  explicit
  BoostPath(path_t const& path):
    m_path { path }
  {}
  // friend
  // Path<PoolT,poolno>
  // common_path<>(Path<PoolT,poolno> const& l, Path<PoolT,poolno> const& r);
  friend
  bool operator == (BoostPath const& l, BoostPath const& r);
  friend
  bool operator < (BoostPath const& l, BoostPath const& r);
};

bool operator == (BoostPath const& l, BoostPath const& r)
{
  return l.m_path == r.m_path;
}

bool operator < (BoostPath const& l, BoostPath const& r)
{
  return l.m_path == r.m_path;
}

#endif /* BOOSTPATH_H */
