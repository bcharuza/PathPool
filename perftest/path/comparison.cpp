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
  std::function<int()> rand = std::bind(std::uniform_int_distribution<int>{},std::default_random_engine{});
  std::string operator()() { return std::to_string(rand()); }
};

template<typename Args>
void EqualityComparison(benchmark::State& state)
{
  using PathT = typename std::tuple_element<0,Args>::type;
  using GeneratorT = typename std::tuple_element<1,Args>::type;
  using T = decltype(GeneratorT{}.operator()());
  //populate
  GeneratorT gen;
  size_t size = state.range(0);
  std::vector<PathT> data_set;
  data_set.reserve(size);
  data_set.push_back( PathT{} );
  for(size_t i=1; i<size; ++i)
      data_set.push_back( PathT {data_set[i/10], std::to_string(rand()%10+1234550)} );
  std::vector<std::pair<size_t,size_t>> instr_set;
  for(size_t i=0; i<size; ++i)
    instr_set.emplace_back( rand()%(instr_set.size()+1), rand()%(instr_set.size()+1) );
  size_t i=0;
  for(auto _ : state)
    {
      for( auto x : instr_set)
	data_set[x.first] == data_set[x.second];
      ++i;
    }
  //setting parameters after test
  state.SetItemsProcessed( i*size );
}

template<typename Args>
void OrderComparison(benchmark::State& state)
{
  using PathT = typename std::tuple_element<0,Args>::type;
  using GeneratorT = typename std::tuple_element<1,Args>::type;
  using T = decltype(GeneratorT{}.operator()());
  //populate
  GeneratorT gen;
  size_t size = state.range(0);
  std::vector<PathT> data_set;
  data_set.reserve(size);
  data_set.push_back( PathT{} );
  for(size_t i=1; i<size; ++i)
      data_set.push_back( PathT {data_set[i/10], std::to_string(rand()%10+1234550)} );
  std::vector<std::pair<size_t,size_t>> instr_set;
  for(size_t i=0; i<size; ++i)
    instr_set.emplace_back( rand()%(instr_set.size()+1), rand()%(instr_set.size()+1) );
  size_t i=0;
  for(auto _ : state)
    {
      for( auto x : instr_set)
	data_set[x.first] < data_set[x.second];
      ++i;
    }
  //setting parameters after test
  state.SetItemsProcessed( i*size );
}

using HashPath_string = std::tuple<Path<HashPathPool<std::string>>,RandomStringGen>;
using ListPath_string = std::tuple<Path<ListPathPool<std::string>>,RandomStringGen>;
using BoostPath_string = std::tuple<BoostPath,RandomStringGen>;

BENCHMARK_TEMPLATE(EqualityComparison, HashPath_string )->RangeMultiplier(1<<4)->Range(64, 64<<16);
BENCHMARK_TEMPLATE(EqualityComparison, ListPath_string )->RangeMultiplier(1<<4)->Range(64, 64<<16);
BENCHMARK_TEMPLATE(EqualityComparison, BoostPath_string )->RangeMultiplier(1<<4)->Range(64, 64<<16);

BENCHMARK_TEMPLATE(OrderComparison, HashPath_string )->RangeMultiplier(1<<4)->Range(64, 64<<16);
BENCHMARK_TEMPLATE(OrderComparison, ListPath_string )->RangeMultiplier(1<<4)->Range(64, 64<<16);
BENCHMARK_TEMPLATE(OrderComparison, BoostPath_string )->RangeMultiplier(1<<4)->Range(64, 64<<16);
