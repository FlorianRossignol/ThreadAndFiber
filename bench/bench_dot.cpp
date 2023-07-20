//
// Created by efarhan on 19/02/2021.
//

#include <benchmark/benchmark.h>
#include <vector>
#include <random>
#include "vector3.h"
#include "bench_utils.h"

const long fromRange = 8;

const long toRange = 1 << 15;


static void BM_DotNaive(benchmark::State &state)
{
    std::vector<maths::Vec3f> v1(state.range(0));
    std::for_each(v1.begin(), v1.end(),[](maths::Vec3f& v){FillRandom(v);});
    std::vector<maths::Vec3f> v2(state.range(0));
    std::for_each(v2.begin(), v2.end(),[](maths::Vec3f& v){FillRandom(v);});
    for(auto _ : state)
    {
        for(std::size_t i = 0; i < state.range(0); i++)
        {
            benchmark::DoNotOptimize(maths::Dot(v1[i], v2[i]));
        }
    }
}

BENCHMARK(BM_DotNaive)->Range(fromRange, toRange);

static void BM_DotAoSoA4(benchmark::State &state)
{
    std::vector<maths::FourVec3f> v1(state.range(0)/4);
    std::for_each(v1.begin(), v1.end(),[](maths::FourVec3f& v){FillRandom(v);});
    std::vector<maths::FourVec3f> v2(state.range(0)/4);
    std::for_each(v2.begin(), v2.end(),[](maths::FourVec3f& v){FillRandom(v);});
    for(auto _ : state)
    {
        for(std::size_t i = 0; i < state.range(0)/4; i++)
        {
            benchmark::DoNotOptimize(maths::FourVec3f::Dot(v1[i], v2[i]));
        }
    }
}
BENCHMARK(BM_DotAoSoA4)->Range(fromRange, toRange);

static void BM_DotAoSoA8(benchmark::State &state)
{
    std::vector<maths::EightVec3f> v1(state.range(0)/8);
    std::for_each(v1.begin(), v1.end(),[](maths::EightVec3f& v){FillRandom(v);});
    std::vector<maths::EightVec3f> v2(state.range(0)/8);
    std::for_each(v2.begin(), v2.end(),[](maths::EightVec3f& v){FillRandom(v);});
    for(auto _ : state)
    {
        for(std::size_t i = 0; i < state.range(0)/8; i++)
        {
            benchmark::DoNotOptimize(maths::EightVec3f::Dot(v1[i], v2[i]));
        }
    }
}
BENCHMARK(BM_DotAoSoA8)->Range(fromRange, toRange);

static void BM_DotAoSoA16(benchmark::State &state)
{
    using Vec3f16 = maths::NVec3f<16>;
    std::vector<Vec3f16> v1(state.range(0)/16);
    std::for_each(v1.begin(), v1.end(),[](auto & v){FillRandom(v);});
    std::vector<Vec3f16> v2(state.range(0)/16);
    std::for_each(v2.begin(), v2.end(),[](auto& v){FillRandom(v);});
    for(auto _ : state)
    {
        for(std::size_t i = 0; i < state.range(0)/16; i++)
        {
            benchmark::DoNotOptimize(Vec3f16::Dot(v1[i], v2[i]));
        }
    }
}
BENCHMARK(BM_DotAoSoA16)->Range(16, toRange);

static void BM_DotAoSoA32(benchmark::State &state)
{
    using Vec3f32 = maths::NVec3f<32>;
    std::vector<Vec3f32> v1(state.range(0)/16);
    std::for_each(v1.begin(), v1.end(),[](auto & v){FillRandom(v);});
    std::vector<Vec3f32> v2(state.range(0)/16);
    std::for_each(v2.begin(), v2.end(),[](auto& v){FillRandom(v);});
    for(auto _ : state)
    {
        for(std::size_t i = 0; i < state.range(0)/16; i++)
        {
            benchmark::DoNotOptimize(Vec3f32::Dot(v1[i], v2[i]));
        }
    }
}
BENCHMARK(BM_DotAoSoA32)->Range(32, toRange);


BENCHMARK_MAIN ();