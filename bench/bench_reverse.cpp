//
// Created by efarhan on 11/10/22.
//


#include <benchmark/benchmark.h>
#include <vector>
#include <random>
#include "vector3.h"
#include "bench_utils.h"
#include <algorithm>

const long fromRange = 8;

const long toRange = 1 << 15;


static void BM_PushBackAndReverse(benchmark::State &state)
{
    std::vector<maths::Vec3f> v1(state.range(0));
    std::for_each(v1.begin(), v1.end(),[](maths::Vec3f& v){FillRandom(v);});
    std::vector<maths::Vec3f> v2;
    v2.reserve(state.range(0));
    for(auto _ : state)
    {
        for(std::size_t i = 0; i < state.range(0); i++)
        {
            v2.push_back(v1[i]);
        }
        std::ranges::reverse(v2.begin(), v2.end());
        state.PauseTiming();
        v2.clear();
        state.ResumeTiming();
    }
}

BENCHMARK(BM_PushBackAndReverse)->Range(fromRange, toRange);

static void BM_PushFront(benchmark::State &state)
{
    std::vector<maths::Vec3f> v1(state.range(0));
    std::for_each(v1.begin(), v1.end(),[](maths::Vec3f& v){FillRandom(v);});
    std::vector<maths::Vec3f> v2;
    v2.reserve(state.range(0));
    for(auto _ : state)
    {
        for(std::size_t i = 0; i < state.range(0); i++)
        {
            v2.insert(v2.begin(), v1[i]);
        }
        state.PauseTiming();
        v2.clear();
        state.ResumeTiming();
    }
}

BENCHMARK(BM_PushFront)->Range(fromRange, toRange);