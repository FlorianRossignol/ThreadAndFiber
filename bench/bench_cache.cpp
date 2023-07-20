//
// Created by efarhan on 2/22/21.
//


#include <benchmark/benchmark.h>
#include <vector>
#include <random>
#include "vector3.h"
#include "bench_utils.h"

const long fromRange = 8;

const long toRange = 1 << 15;

static void BM_CacheBench(benchmark::State &state)
{
    std::size_t bytes = 1u << state.range(0);
    std::size_t count = (bytes/sizeof(int))/2u;
    std::vector<int> v(count);
    FillRandom(v, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    for(auto _ : state)
    {
        long sum = 0;
        for(auto n: v)
        {
            sum += n;
        }
        benchmark::DoNotOptimize(sum);
    }
    state.SetBytesProcessed(static_cast<std::size_t>(state.iterations())* static_cast<std::size_t>(bytes));
    state.SetLabel(bytes/1024 > 1000?std::to_string(bytes/1024/1024)+"mb":std::to_string(bytes/1024)+"kb");

}
BENCHMARK(BM_CacheBench)->DenseRange(13, 26)->ReportAggregatesOnly(true);

static void BM_RandomCacheBench(benchmark::State &state)
{
    std::size_t bytes = 1u << state.range(0);
    std::size_t count = (bytes/sizeof(int))/2u;
    std::vector<int> v(count);
    FillRandom(v, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    std::vector<int> indices(count);
    FillRandom(indices, 0, count);
    for(auto _ : state)
    {
        long sum = 0;
        for(auto i : indices)
        {
            sum += v[i];
        }
        benchmark::DoNotOptimize(sum);
    }
    state.SetBytesProcessed(std::size_t(state.iterations())* std::size_t(bytes));
    state.SetLabel(bytes/1024 > 1000?std::to_string(bytes/1024/1024)+"mb":std::to_string(bytes/1024)+"kb");

}
BENCHMARK(BM_RandomCacheBench)->DenseRange(13, 26)->ReportAggregatesOnly(true);