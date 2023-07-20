
#include <benchmark/benchmark.h>
#include "random_fill.h"

#include <list>
#include <vector>


constexpr long fromRange = 8;
constexpr long toRange = 1 << 20;

static void BM_VectorIterate(benchmark::State& state)
{
    const std::size_t length = state.range(0);
    std::vector<float> numbers_src(length);

    fill_vector(numbers_src, 0.0f, 100.0f);
    
    for (auto _ : state)
    {
        float sum = 0.0f;
        for(std::size_t i = 0; i < length;i++)
        {
            sum += numbers_src[i];
        }
        benchmark::DoNotOptimize(sum);
    }
}

BENCHMARK(BM_VectorIterate)->Range(fromRange, toRange);

static void BM_ListIterate(benchmark::State& state)
{
    const std::size_t length = state.range(0);
    std::list<float> numbers_src(length);

    fill_vector(numbers_src, 0, 100);
    
    for (auto _ : state)
    {
        float sum = 0.0f;
        for(auto& n : numbers_src)
        {
            sum += n;
        }
        benchmark::DoNotOptimize(sum);
    }
}

BENCHMARK(BM_ListIterate)->Range(fromRange, toRange);