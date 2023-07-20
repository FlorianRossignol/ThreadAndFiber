//
// Created by efarhan on 30/01/2022.
//
#include <benchmark/benchmark.h>
#include <atomic>
#include <memory>
#include <mutex>
#include <array>

static constexpr std::size_t maxThreads = 64;

class MyIntAtomic
{
public:
    MyIntAtomic(int count): count(count){}
    void Increment()
    {
        count.fetch_add(1, std::memory_order_relaxed);
    }
    [[nodiscard]] int GetNumber() const {return count;}
private:
    std::atomic<int> count;
};

class MyIntAtomicArray
{
public:
    void Increment(std::size_t index)
    {
        count[index].fetch_add(1, std::memory_order_relaxed);
    }
    [[nodiscard]] int GetNumber(std::size_t index) const {return count[index];}
private:
    std::array<std::atomic<int>, maxThreads> count;
};
class MyIntAtomicArrayPadding
{
public:
    void Increment(std::size_t index)
    {
        count[index].count.fetch_add(1, std::memory_order_relaxed);
    }
    [[nodiscard]] int GetNumber(std::size_t index) const {return count[index].count;}
private:
    struct alignas(64) MyIntPadding
    {
        std::atomic<int> count;
    };
    std::array<MyIntPadding, maxThreads> count;
};

class MyIntLock
{
public:
    MyIntLock(int count): count(count){}
    void Increment()
    {
        std::lock_guard<std::mutex> lock(m);
        count++;
    }
    int GetNumber() const {return count;}
private:
    int count;
    mutable std::mutex m;
};

static void BM_01_Atomic(benchmark::State& state) {
    static std::unique_ptr<MyIntAtomic> counter;
    if (state.thread_index() == 0) {
        // Setup code here.
        counter = std::make_unique<MyIntAtomic>(0);
    }
    for (auto _ : state) {
        // Run the test as normal.
        counter->Increment();
    }
    if (state.thread_index() == 0) {
        // Teardown code here.

    }
}
BENCHMARK(BM_01_Atomic)->ThreadRange(1, maxThreads);



static void BM_01_Lock(benchmark::State& state) {
    static std::unique_ptr<MyIntLock> counter;
    if (state.thread_index() == 0) {
        // Setup code here.
        counter = std::make_unique<MyIntLock>(0);
    }
    for (auto _ : state) {
        // Run the test as normal.
        counter->Increment();
    }
    if (state.thread_index() == 0) {
        // Teardown code here.

    }
}
BENCHMARK(BM_01_Lock)->ThreadRange(1, maxThreads);

static void BM_02_AtomicArray(benchmark::State& state) {
    static std::unique_ptr<MyIntAtomicArray> counter;
    if (state.thread_index() == 0) {
        // Setup code here.
        counter = std::make_unique<MyIntAtomicArray>();
    }
    for (auto _ : state) {
        // Run the test as normal.
        counter->Increment(state.thread_index());
    }
    if (state.thread_index() == 0) {
        // Teardown code here.

    }
}
BENCHMARK(BM_02_AtomicArray)->ThreadRange(1, maxThreads);

static void BM_02_AtomicArrayPadding(benchmark::State& state) {
    static std::unique_ptr<MyIntAtomicArrayPadding> counter;
    if (state.thread_index() == 0) {
        // Setup code here.
        counter = std::make_unique<MyIntAtomicArrayPadding>();
    }
    for (auto _ : state) {
        // Run the test as normal.
        counter->Increment(state.thread_index());
    }
    if (state.thread_index() == 0) {
        // Teardown code here.

    }
}
BENCHMARK(BM_02_AtomicArrayPadding)->ThreadRange(1, maxThreads);

static void BM_02_Array(benchmark::State& state) {
    static std::array<int, maxThreads> counter;
    if (state.thread_index() == 0) {
    }
    for (auto _ : state) {
        // Run the test as normal.
        counter[state.thread_index()]++;
    }
    if (state.thread_index() == 0) {
        // Teardown code here.
        benchmark::DoNotOptimize(counter.data());
    }
}
BENCHMARK(BM_02_Array)->ThreadRange(1, maxThreads);

struct alignas(64) IntPadding
{
    int count;
};
static void BM_02_ArrayPadding(benchmark::State& state) {
    static std::array<IntPadding, maxThreads> counter;
    if (state.thread_index() == 0) {
    }
    for (auto _ : state) {
        // Run the test as normal.
        counter[state.thread_index()].count++;
    }
    if (state.thread_index() == 0) {
        // Teardown code here.
        benchmark::DoNotOptimize(counter);
    }
}
BENCHMARK(BM_02_ArrayPadding)->ThreadRange(1, maxThreads);