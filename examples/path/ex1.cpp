#include "path.h"
#include "path_pool.h"

#include <iostream>

int main()
{
  using Path = Path<ListPathPool<std::string>>;
  Path root;
  Path p1 { root, "path1" };
  Path p2 { root, "path2" };
  Path p3 { root, "path1" };
  Path p4 { p2, "path1" };
  std::cout<< (p1 == p2) << std::endl; // false
  std::cout<< (p1 == p3) << std::endl; // true
  std::cout<< (p1 == p4) << std::endl; // false
  std::cout<< root.get_tag() << std::endl; // ''
  std::cout<< p4.get_tag() << std::endl; // path1
  std::cout<< p4.get_parent().get_tag() << std::endl; // path2
}
