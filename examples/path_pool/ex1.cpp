#include "path_pool.h"

#include <iostream>

int main()
{
  using pathid_t = ListPathPool<std::string>::pathid_t;
  ListPathPool<std::string> pool { "root" };
  pathid_t root = pool.get_root();
  pathid_t p1 = pool.get_subnode(root, "path1");
  pathid_t p2 = pool.get_subnode(root, "path2");
  pathid_t p3 = pool.get_subnode(root, "path1");
  pathid_t p4 = pool.get_subnode(p2, "path1");
  // Notice no pool object is needed
  std::cout<< (p1 == p2) << std::endl; // false
  std::cout<< (p1 == p3) << std::endl; // true
  std::cout<< (p1 == p4) << std::endl; // false
  std::cout<< pool.get_tag(root) << std::endl; // root
  std::cout<< pool.get_tag(p4) << std::endl; // path1
  std::cout<< pool.get_tag(pool.get_parent(p4)) << std::endl; // path2
}
