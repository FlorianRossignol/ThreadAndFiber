#include <chrono>
#include <benchmark/benchmark.h>

#include "custom_allocator.h"

constexpr auto allocationSize = 16;
constexpr auto alignment = 8;

static void BM_Malloc(benchmark::State& state) {
    // Perform setup here

    for (auto _ : state) {
        std::chrono::duration<double> totalTime{};
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < state.range(0); i++)
        {
            auto* ptr = std::malloc(allocationSize);
            benchmark::DoNotOptimize(ptr);
            std::free(ptr);

        }
        auto end = std::chrono::high_resolution_clock::now();
        totalTime += std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
        state.SetIterationTime(totalTime.count());
        benchmark::ClobberMemory();
    }
}
// Register the function as a benchmark
BENCHMARK(BM_Malloc)->Range( 1, 512 )->UseManualTime();


static void BM_LinearAllocator(benchmark::State& state) {
    // Perform setup here
    const std::size_t totalSize = allocationSize * state.range(0);
    void* rootPtr = std::malloc(totalSize);
    const auto allocationNum = state.range(0);
    for (auto _ : state) {
        std::chrono::duration<double> totalTime{};
        LinearAllocator allocator(rootPtr, totalSize);

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < allocationNum; i++)
        {
            auto* ptr = allocator.Allocate(allocationSize, alignment);
            benchmark::DoNotOptimize(ptr);

        }
        auto end = std::chrono::high_resolution_clock::now();
        totalTime +=
                std::chrono::duration_cast<std::chrono::duration<double>>(
                        end - start);
        state.SetIterationTime(totalTime.count());
        benchmark::ClobberMemory();
    }
}
// Register the function as a benchmark
BENCHMARK(BM_LinearAllocator)->Range(1, 512)->UseManualTime();


static void BM_StackAllocator(benchmark::State& state) {
    // Perform setup here
    const std::size_t totalSize = allocationSize * state.range(0) * 2;
    void* rootPtr = std::malloc(totalSize);
    const auto allocationNum = state.range(0);
    for (auto _ : state) {
        std::chrono::duration<double> totalTime{};
        StackAllocator allocator(rootPtr, totalSize);
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < allocationNum; i++)
        {
            auto* ptr = allocator.Allocate(allocationSize, alignment);
            benchmark::DoNotOptimize(ptr);
        }
        auto end = std::chrono::high_resolution_clock::now();
        totalTime +=
                std::chrono::duration_cast<std::chrono::duration<double>>(
                        end - start);
        state.SetIterationTime(totalTime.count());
        benchmark::ClobberMemory();

    }
}
// Register the function as a benchmark
BENCHMARK(BM_StackAllocator)->Range(1, 512)->UseManualTime();


static void BM_FreeListAllocator(benchmark::State& state) {
    // Perform setup here
    const std::size_t totalSize = allocationSize * state.range(0) * 2;
    void* rootPtr = std::malloc(totalSize);
    const auto allocationNum = state.range(0);
    for (auto _ : state) {
        std::chrono::duration<double> totalTime{};
        FreeListAllocator allocator(rootPtr, totalSize);
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < allocationNum; i++)
        {
            auto* ptr = allocator.Allocate(allocationSize, alignment);
            benchmark::DoNotOptimize(ptr);
        }
        auto end = std::chrono::high_resolution_clock::now();
        totalTime +=
            std::chrono::duration_cast<std::chrono::duration<double>>(
                end - start);
        state.SetIterationTime(totalTime.count());
        benchmark::ClobberMemory();

    }
}
// Register the function as a benchmark
BENCHMARK(BM_FreeListAllocator)->Range(1, 512)->UseManualTime();