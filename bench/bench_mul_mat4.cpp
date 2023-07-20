#include <benchmark/benchmark.h>
#include <vector>
#include <random>
#include "matrix4.h"
#include "bench_utils.h"

const long fromRange = 8;

const long toRange = 1 << 15;


static void BM_Mat4MulNaive(benchmark::State &state)
{
    std::vector<maths::Mat4f> m1(state.range(0));
    std::for_each(m1.begin(), m1.end(),[](maths::Mat4f& m){FillRandom(m);});
    std::vector<maths::Mat4f> m2(state.range(0));
    std::for_each(m1.begin(), m1.end(),[](maths::Mat4f& m){FillRandom(m);});
    for(auto _ : state)
    {
        for(std::int64_t i = 0; i < state.range(0); i++)
        {
            benchmark::DoNotOptimize(m1[i]*m2[i]);
        }
    }
}
BENCHMARK(BM_Mat4MulNaive)->Range(fromRange, toRange);

static void BM_Mat4MulOpti(benchmark::State &state)
{
    std::vector<maths::Mat4f> m1(state.range(0));
    std::for_each(m1.begin(), m1.end(),[](maths::Mat4f& m){FillRandom(m);});
    std::vector<maths::Mat4f> m2(state.range(0));
    std::for_each(m1.begin(), m1.end(),[](maths::Mat4f& m){FillRandom(m);});
    for(auto _ : state)
    {
        for(std::int64_t i = 0; i < state.range(0); i++)
        {
            benchmark::DoNotOptimize(m1[i].MultIntrinsics(m2[i]));
        }
    }
}
BENCHMARK(BM_Mat4MulOpti)->Range(fromRange, toRange);