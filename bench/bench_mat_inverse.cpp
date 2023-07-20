//
// Created by efarhan on 2/22/21.
//

#include <benchmark/benchmark.h>
#include <vector>
#include "matrix4.h"
#include "bench_utils.h"

const long fromRange = 8;

const long toRange = 1 << 15;


static void BM_Mat4InverseNaive(benchmark::State &state)
{
    std::vector<maths::Mat4f> m1(state.range(0));
    std::for_each(m1.begin(), m1.end(),[](maths::Mat4f& m){FillRandom(m);});
    for(auto _ : state)
    {
        for(std::int64_t i = 0; i < state.range(0); i++)
        {
            benchmark::DoNotOptimize(m1[i].Inverse());
        }
    }
}
BENCHMARK(BM_Mat4InverseNaive)->Range(fromRange, toRange);

static void BM_Mat4InverseOpti(benchmark::State &state)
{
    std::vector<maths::Mat4f> m1(state.range(0));
    std::for_each(m1.begin(), m1.end(),[](maths::Mat4f& m){FillRandom(m);});
    for(auto _ : state)
    {
        for(std::int64_t i = 0; i < state.range(0); i++)
        {
            benchmark::DoNotOptimize(m1[i].InverseOpti());
        }
    }
}
BENCHMARK(BM_Mat4InverseOpti)->Range(fromRange, toRange);

static void BM_Mat4InverseIdentityNaive(benchmark::State &state)
{
    std::vector<maths::Mat4f> m1(state.range(0), maths::Mat4f::identity());
    for(auto _ : state)
    {
        for(std::int64_t i = 0; i < state.range(0); i++)
        {
            benchmark::DoNotOptimize(m1[i].Inverse());
        }
    }
}
BENCHMARK(BM_Mat4InverseIdentityNaive)->Range(fromRange, toRange);

static void BM_Mat4InverseIdentityOpti(benchmark::State &state)
{
    std::vector<maths::Mat4f> m1(state.range(0), maths::Mat4f::identity());
    for(auto _ : state)
    {
        for(std::int64_t i = 0; i < state.range(0); i++)
        {
            benchmark::DoNotOptimize(m1[i].InverseOpti());
        }
    }
}
BENCHMARK(BM_Mat4InverseIdentityOpti)->Range(fromRange, toRange);