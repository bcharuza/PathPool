#ifndef UTEST_UTILS_H
#define UTEST_UTILS_H

#include <algorithm>
#include <memory>
#include <exception>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#ifndef UTEST_FNAME
// defined by cmake in compile-time
// if not defined - no action
#define UTEST_FNAME (void)
#endif

template <typename T>
class ResultSorted
{
  std::unique_ptr<T> sorted_result = nullptr;
public:
  void expected_result(T arg)  {
    std::stable_sort(begin(arg),end(arg));
    sorted_result = std::make_unique<T,T&&>(std::move(arg));
  }
  const T& expected_result() const  {
    if (!sorted_result)
      throw std::runtime_error("result not specified. Define result first");
    return *sorted_result; 
  }
};


template<typename T1>
bool empty(std::pair<T1,T1> cont1)
{
  return std::distance(cont1.first,cont1.second) == 0;
}

template<typename T1,typename T2>
bool equals(T1 c1_first,T1 c1_second, T2 c2_first, T2 c2_second)
{
  return std::equal(c1_first,c1_second,c2_first,c2_second);
}
template<typename T1,typename T2>
bool equals(T1 c1_first, T1 c1_second,std::pair<T2,T2> cont2)
{
  return equals(c1_first,c1_second,cont2.first,cont2.second);
}
template<typename T1,typename T2>
bool equals(T1 c1_first, T1 c1_second,T2 const& cont2)
{
  return equals(c1_first,c1_second,cont2.begin(),cont2.end());
}
template<typename T1,typename T2>
bool equals(T1 const& cont1, std::pair<T2,T2> cont2)
{
  return equals(cont2.first,cont2.second,cont1);
}
template<typename T1,typename T2>
bool equals(T1 const& cont1, T2 const& cont2)
{
  return equals(cont2.begin(),cont2.end(),cont1);
}
template<typename T1,typename T2>
bool equals(T1 const& cont1, std::initializer_list<T2> cont2)
{
  return equals(cont2.begin(),cont2.end(),cont1);
}

template<typename T1,typename T2>
bool contains_all(T1 begin, T1 end, T2 cont2)
{
  return std::all_of(cont2.begin(),cont2.end(),
		     [begin,end](auto x){ return std::find(begin,end,x) != end; });
}

template<typename T1,typename T2>
bool contains_all(std::pair<T1,T1> cont1, T2 cont2)
{
  return contains_all(cont1.first,cont1.second,cont2);
}

template<typename T1,typename T2>
bool contains_all(std::pair<T1,T1> cont1, std::initializer_list<T2> cont2)
{
  return contains_all(cont1.first,cont1.second,cont2);
}

template<typename T1,typename T2>
bool contains_all(T1 const& cont1, T2 cont2)
{
  return contains_all(cont1.cbegin(),cont1.cend(),cont2);
}

template<typename T1,typename T2>
bool contains_all(T1 const& cont1, std::initializer_list<T2> cont2)
{
  return contains_all(cont1.cbegin(),cont1.cend(),cont2);
}

#endif /*UTEST_UTILS_H*/
