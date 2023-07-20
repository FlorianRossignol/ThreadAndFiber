//
// Created by efarhan on 20/02/2021.
//

#include <benchmark/benchmark.h>
#include <vector>
#include <random>
#include "vector3.h"
#include "bench_utils.h"
#include "planet.h"

const long fromRange = 8;

const long toRange = 1 << 15;


static void BM_PlanetNaive(benchmark::State &state)
{
    PlanetSystem planetSystem(state.range(0));
    for(auto _ : state)
    {
        planetSystem.Update(0.02f);
    }
}

BENCHMARK(BM_PlanetNaive)->Range(fromRange, toRange);
static void BM_Planet4(benchmark::State &state)
{
    PlanetSystem4 planetSystem(state.range(0));
    for(auto _ : state)
    {
        planetSystem.Update(0.02f);
    }
}

BENCHMARK(BM_Planet4)->Range(fromRange, toRange);
static void BM_Planet8(benchmark::State &state)
{
    PlanetSystem8 planetSystem(state.range(0));
    for(auto _ : state)
    {
        planetSystem.Update(0.02f);
    }
}

BENCHMARK(BM_Planet8)->Range(fromRange, toRange);

BENCHMARK_MAIN();