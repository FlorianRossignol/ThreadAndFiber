
#include <benchmark/benchmark.h>
#include "random_fill.h"
#include <cstring>


constexpr long fromRange = 8;
constexpr long toRange = 1 << 20;

static void BM_Copy(benchmark::State& state)
{
    const std::size_t length = state.range(0);
    std::vector<int> numbers_src(length);
    std::vector<int> numbers_dst(length);

    fill_vector(numbers_src, 0, 100);
    
    for (auto _ : state)
    {
        for(std::size_t i = 0; i < length;i++)
        {
            numbers_dst[i] = numbers_src[i];
        }
    }
}

BENCHMARK(BM_Copy)->Range(fromRange, toRange);


static void BM_CopyMemcpy(benchmark::State& state)
{
    const std::size_t length = state.range(0);
    std::vector<int> numbers_src(length);
    std::vector<int> numbers_dst(length);

    fill_vector(numbers_src, 0, 100);

    for (auto _ : state)
    {
        std::memcpy(numbers_dst.data(), numbers_src.data(), sizeof(int) * length);
    }
}

static void BM_CopyAlgo(benchmark::State& state)
{
    const std::size_t length = state.range(0);
    std::vector<int> numbers_src(length);
    std::vector<int> numbers_dst(length);

    fill_vector(numbers_src, 0, 100);

    for (auto _ : state)
    {
        std::copy(numbers_src.begin(), numbers_src.end(), numbers_dst.begin());
    }
}

BENCHMARK(BM_CopyAlgo)->Range(fromRange, toRange);

BENCHMARK(BM_CopyMemcpy)->Range(fromRange, toRange);

// Copy if only even
static void BM_CopyIf(benchmark::State& state)
{
    const std::size_t length = state.range(0);
    std::vector<int> numbers_src(length);
    std::vector<int> numbers_dst(length);

    fill_vector(numbers_src, 0, 100);

    for (auto _ : state)
    {
        for(std::size_t i = 0; i < length; i++)
        {
            if(numbers_src[i] % 2 == 0)
            {
                numbers_dst[i] = numbers_src[i];
            }
        }
    }
}

BENCHMARK(BM_CopyIf)->Range(fromRange, toRange);

// Copy if only even
static void BM_CopyIfSorted(benchmark::State& state)
{
    const std::size_t length = state.range(0);
    std::vector<int> numbers_src(length);
    std::vector<int> numbers_dst(length);

    fill_vector(numbers_src, 0, 100);
    std::sort(numbers_src.begin(), numbers_src.end(), [](auto n1, auto n2)
        {
            return (n1 % 2 == 0) < (n2 % 2 == 0);
        });

    for (auto _ : state)
    {
        for (std::size_t i = 0; i < length; i++)
        {
            if (numbers_src[i] % 2 == 0)
            {
                numbers_dst[i] = numbers_src[i];
            }
        }
    }
}

BENCHMARK(BM_CopyIfSorted)->Range(fromRange, toRange);