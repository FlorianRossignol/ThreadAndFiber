
#include <chrono>
#include <benchmark/benchmark.h>


static void BM_Malloc(benchmark::State& state) {
    // Perform setup here

    for (auto _ : state) {
        std::chrono::duration<double> totalTime{};
        for (int i = 0; i < state.range(1); i++)
        {
            auto start = std::chrono::high_resolution_clock::now();
            auto* ptr = std::malloc(state.range(0));
            benchmark::DoNotOptimize(ptr);
            auto end = std::chrono::high_resolution_clock::now();
            std::free(ptr);
            totalTime +=
                std::chrono::duration_cast<std::chrono::duration<double>>(
                    end - start);
        }
        state.SetIterationTime(totalTime.count());
        benchmark::ClobberMemory();
    }
}
// Register the function as a benchmark
BENCHMARK(BM_Malloc)->Ranges({ {8, 8 << 20}, {1, 512} })->UseManualTime();


static void BM_New(benchmark::State& state) {
    // Perform setup here

    for (auto _ : state) {
        std::chrono::duration<double> totalTime{};
        for (int i = 0; i < state.range(1); i++)
        {
            auto start = std::chrono::high_resolution_clock::now();
            auto* ptr = new char[state.range(0)];
            benchmark::DoNotOptimize(ptr);
            auto end = std::chrono::high_resolution_clock::now();
            delete[] ptr;
            totalTime +=
                std::chrono::duration_cast<std::chrono::duration<double>>(
                    end - start);
        }
        state.SetIterationTime(totalTime.count());
        benchmark::ClobberMemory();
    }
}
// Register the function as a benchmark
BENCHMARK(BM_New)->Ranges({ {8, 8 << 20}, {1, 512} })->UseManualTime();