#include <benchmark/benchmark.h>

#include "path.h"
#include "path_pool.h"
#include "BoostPath.h"

#include <vector>
#include <string>
#include <random>
#include <functional>
#include <tuple>
#include <climits>

struct RandomStringGen
{
  std::function<double()> rand = std::bind(std::uniform_real_distribution<double>{},std::default_random_engine{});
  std::string operator()() { return std::to_string(rand()); }
};

template<typename PathT,typename SetT>
void populate(SetT& set)
{
  std::vector<PathT> nodes;
  nodes.reserve(set.size());
  nodes.push_back(PathT {});
  for( auto x : set )
    nodes.push_back(PathT {nodes[x.first], x.second});
}

template<typename Args>
void RandomInsertion(benchmark::State& state)
{
  using PathT = typename std::tuple_element<0,Args>::type;
  using GeneratorT = typename std::tuple_element<1,Args>::type;
  using T = decltype(GeneratorT{}.operator()());
  //populate
  GeneratorT gen;
  size_t size = state.range(0);
  std::vector<std::pair<size_t,T>> insertions;
  insertions.reserve(size);
  std::function<int()> rand = std::bind(std::uniform_int_distribution<size_t>{},std::default_random_engine{});
  for(size_t i=0; i<size; ++i)
    insertions.emplace_back(rand()%(insertions.size()+1), gen());
  size_t i=0;
  for(auto _ : state)
    {
      populate<PathT>(insertions);
      ++i;
    }
  //setting parameters after test
  state.SetItemsProcessed( i*size );
}

template<typename Args>
void SequentialInsertion(benchmark::State& state)
{
  using PathT = typename std::tuple_element<0,Args>::type;
  using GeneratorT = typename std::tuple_element<1,Args>::type;
  using T = decltype(GeneratorT{}.operator()());
  //populate
  GeneratorT gen;
  size_t size = state.range(0);
  std::vector<std::pair<size_t,T>> insertions;
  insertions.reserve(size);
  for(size_t i=0; i<size; ++i)
    insertions.emplace_back(i, gen());
  size_t i=0;
  for(auto _ : state)
    {
      populate<PathT>(insertions);
      ++i;
    }
  //setting parameters after test
  state.SetItemsProcessed( i*size );
}

template<typename Args>
void Parallel16Insertion(benchmark::State& state)
{
  using PathT = typename std::tuple_element<0,Args>::type;
  using GeneratorT = typename std::tuple_element<1,Args>::type;
  using T = decltype(GeneratorT{}.operator()());
  //populate
  GeneratorT gen;
  size_t size = state.range(0);
  std::vector<std::pair<size_t,T>> insertions;
  insertions.reserve(size);
  for(size_t i=0; i<size; ++i)
    insertions.emplace_back(i/16, gen());
  size_t i=0;
  for(auto _ : state)
    {
      populate<PathT>(insertions);
      ++i;
    }
  //setting parameters after test
  state.SetItemsProcessed( i*size );
}

template<typename Args>
void Parallel64Insertion(benchmark::State& state)
{
  using PathT = typename std::tuple_element<0,Args>::type;
  using GeneratorT = typename std::tuple_element<1,Args>::type;
  using T = decltype(GeneratorT{}.operator()());
  //populate
  GeneratorT gen;
  size_t size = state.range(0);
  std::vector<std::pair<size_t,T>> insertions;
  insertions.reserve(size);
  for(size_t i=0; i<size; ++i)
    insertions.emplace_back(i/64, gen());
  size_t i=0;
  for(auto _ : state)
    {
      populate<PathT>(insertions);
      ++i;
    }
  //setting parameters after test
  state.SetItemsProcessed( i*size );
}

template<typename Args>
void Parallel256Insertion(benchmark::State& state)
{
  using PathT = typename std::tuple_element<0,Args>::type;
  using GeneratorT = typename std::tuple_element<1,Args>::type;
  using T = decltype(GeneratorT{}.operator()());
  //populate
  GeneratorT gen;
  size_t size = state.range(0);
  std::vector<std::pair<size_t,T>> insertions;
  insertions.reserve(size);
  for(size_t i=0; i<size; ++i)
    insertions.emplace_back(i/256, gen());
  size_t i=0;
  for(auto _ : state)
    {
      populate<PathT>(insertions);
      ++i;
    }
  //setting parameters after test
  state.SetItemsProcessed( i*size );
}

template<typename Args>
void Parallel4096Insertion(benchmark::State& state)
{
  using PathT = typename std::tuple_element<0,Args>::type;
  using GeneratorT = typename std::tuple_element<1,Args>::type;
  using T = decltype(GeneratorT{}.operator()());
  //populate
  GeneratorT gen;
  size_t size = state.range(0);
  std::vector<std::pair<size_t,T>> insertions;
  insertions.reserve(size);
  for(size_t i=0; i<size; ++i)
    insertions.emplace_back(i/4096, gen());
  size_t i=0;
  for(auto _ : state)
    {
      populate<PathT>(insertions);
      ++i;
    }
  //setting parameters after test
  state.SetItemsProcessed( i*size );
}

using HashPath_string = std::tuple<Path<HashPathPool<std::string>>,RandomStringGen>;
using ListPath_string = std::tuple<Path<ListPathPool<std::string>>,RandomStringGen>;
using BoostPath_string = std::tuple<BoostPath,RandomStringGen>;

BENCHMARK_TEMPLATE(RandomInsertion, HashPath_string )->RangeMultiplier(1<<4)->Range(64, 64<<16);
BENCHMARK_TEMPLATE(RandomInsertion, ListPath_string )->RangeMultiplier(1<<4)->Range(64, 64<<16);
BENCHMARK_TEMPLATE(RandomInsertion, BoostPath_string )->RangeMultiplier(1<<4)->Range(64, 64<<16);

BENCHMARK_TEMPLATE(SequentialInsertion, HashPath_string )->RangeMultiplier(1<<4)->Range(64, 64<<16);
BENCHMARK_TEMPLATE(SequentialInsertion, ListPath_string )->RangeMultiplier(1<<4)->Range(64, 64<<16);
BENCHMARK_TEMPLATE(SequentialInsertion, BoostPath_string )->RangeMultiplier(1<<4)->Range(64, 64<<8);

BENCHMARK_TEMPLATE(Parallel16Insertion, HashPath_string )->RangeMultiplier(1<<4)->Range(64, 64<<16);
BENCHMARK_TEMPLATE(Parallel16Insertion, ListPath_string )->RangeMultiplier(1<<4)->Range(64, 64<<16);
BENCHMARK_TEMPLATE(Parallel16Insertion, BoostPath_string )->RangeMultiplier(1<<4)->Range(64, 64<<16);

BENCHMARK_TEMPLATE(Parallel64Insertion, HashPath_string )->RangeMultiplier(1<<4)->Range(64, 64<<16);
BENCHMARK_TEMPLATE(Parallel64Insertion, ListPath_string )->RangeMultiplier(1<<4)->Range(64, 64<<16);
BENCHMARK_TEMPLATE(Parallel64Insertion, BoostPath_string )->RangeMultiplier(1<<4)->Range(64, 64<<16);

BENCHMARK_TEMPLATE(Parallel256Insertion, HashPath_string )->RangeMultiplier(1<<4)->Range(64, 64<<16);
BENCHMARK_TEMPLATE(Parallel256Insertion, ListPath_string )->RangeMultiplier(1<<4)->Range(64, 64<<16);
BENCHMARK_TEMPLATE(Parallel256Insertion, BoostPath_string )->RangeMultiplier(1<<4)->Range(64, 64<<16);

BENCHMARK_TEMPLATE(Parallel4096Insertion, HashPath_string )->RangeMultiplier(1<<4)->Range(64, 64<<16);
BENCHMARK_TEMPLATE(Parallel4096Insertion, ListPath_string )->RangeMultiplier(1<<4)->Range(64, 64<<16);
BENCHMARK_TEMPLATE(Parallel4096Insertion, BoostPath_string )->RangeMultiplier(1<<4)->Range(64, 64<<16);
