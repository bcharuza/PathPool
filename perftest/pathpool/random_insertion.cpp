#include <benchmark/benchmark.h>

#include "path_pool.h"
#include <vector>
#include <string>
#include <random>
#include <functional>
#include <tuple>
#include <climits>

struct RandomUIntGen
{
  std::function<unsigned()> rand = std::bind(std::uniform_int_distribution<unsigned>{},std::default_random_engine{});
  size_t operator()() { return rand(); }
};

struct RandomStringGen
{
  std::function<double()> rand = std::bind(std::uniform_real_distribution<double>{},std::default_random_engine{});
  std::string operator()() { return std::to_string(rand()); }
};

inline
size_t byten(int){ return sizeof(int);}
inline
size_t byten(std::string s){ return s.size()*sizeof(char);}

template<typename ContainerT,typename SetT>
void populate(SetT& set)
{
  using path_t = typename ContainerT::pathid_t;
  ContainerT cont;
  std::vector<path_t> nodes;
  nodes.reserve(set.size());
  nodes.push_back(cont.get_root());
  for( auto x : set )
    nodes.push_back(cont.get_subnode(nodes[x.first], x.second));
}

template<typename Args>
void RandomInsertion(benchmark::State& state)
{
  using ContainerT = typename std::tuple_element<0,Args>::type;
  using GeneratorT = typename std::tuple_element<1,Args>::type;
  using T = decltype(GeneratorT{}.operator()());
  //populate
  GeneratorT gen;
  size_t size = state.range(0);
  std::vector<std::pair<size_t,T>> insertions;
  insertions.reserve(size);
  for(size_t i=0; i<size; ++i)
    insertions.emplace_back(rand()%(insertions.size()+1), gen());
  //test
  size_t i=0;
  for(auto _ : state)
    {
      populate<ContainerT>(insertions);
      ++i;
    }
  //setting parameters after test
  state.SetItemsProcessed( i*size );
}

using HashPathPool_int = std::tuple<HashPathPool<int>,RandomUIntGen>;
using HashPathPool_string = std::tuple<HashPathPool<std::string>,RandomStringGen>;
using ListPathPool_int = std::tuple<ListPathPool<int>,RandomUIntGen>;
using ListPathPool_string = std::tuple<ListPathPool<std::string>,RandomStringGen>;

BENCHMARK_TEMPLATE(RandomInsertion, HashPathPool_int )->RangeMultiplier(1<<4)->Range(64, 64<<16);
BENCHMARK_TEMPLATE(RandomInsertion, HashPathPool_string )->RangeMultiplier(1<<4)->Range(64, 64<<16);
BENCHMARK_TEMPLATE(RandomInsertion, ListPathPool_int )->RangeMultiplier(1<<4)->Range(64, 64<<16);
BENCHMARK_TEMPLATE(RandomInsertion, ListPathPool_string )->RangeMultiplier(1<<4)->Range(64, 64<<16);
