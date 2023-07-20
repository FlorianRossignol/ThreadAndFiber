
#include <benchmark/benchmark.h>

#include "math_utils.h"
#include "random_fill.h"


static void BM_IsPrimeNaive(benchmark::State& state) {
    // Perform setup here

    std::vector<unsigned> numbers(state.range(0));
    fill_vector<unsigned>(numbers, 1u, 1000u);

    for (auto _ : state) {
        for(auto n : numbers)
        {
            benchmark::DoNotOptimize(maths::is_prime_naive(n));
        }
        benchmark::ClobberMemory();
    }
}
// Register the function as a benchmark
BENCHMARK(BM_IsPrimeNaive)->Range( 8, 8 << 22);

bool is_prime_opti(unsigned number)
{
    static auto prime_numbers = maths::generate_prime_under(1000u);

    for(const auto prime_number: prime_numbers)
    {
        if(prime_number*prime_number > number)
        {
            break;
        }
        if(number % prime_number == 0)
        {
            return false;
        }
    }
    return true;
}

static void BM_IsPrimeDynamic(benchmark::State& state) {
    // Perform setup here


    std::vector<unsigned> numbers(state.range(0));
    fill_vector<unsigned>(numbers, 1u, 1000u);

    for (auto _ : state) {
        for (auto n : numbers)
        {
            benchmark::DoNotOptimize(is_prime_opti(n));
        }
        benchmark::ClobberMemory();
    }
}
// Register the function as a benchmark
BENCHMARK(BM_IsPrimeDynamic)->Range(8, 8 << 22);