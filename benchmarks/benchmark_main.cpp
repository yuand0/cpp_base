#include <benchmark/benchmark.h>
#include "intarray.h"
#include "thread_safe_stack.h"
#include "tokenizer.h"
#include "calculator.h"
#include "cosine.h"
#include <vector>
#include <random>

// ========== IntArray ==========
static void BM_IntArray_Copy(benchmark::State& state) {
    size_t size = state.range(0);
    IntArray a(size);
    for (auto _ : state) {
        IntArray b(a);
        benchmark::DoNotOptimize(b);
    }
    state.SetComplexityN(size);
}
BENCHMARK(BM_IntArray_Copy)->Range(1024, 1<<20)->Complexity();

static void BM_IntArray_Move(benchmark::State& state) {
    size_t size = state.range(0);
    for (auto _ : state) {
        IntArray a(size);
        IntArray b(std::move(a));
        benchmark::DoNotOptimize(b);
    }
    state.SetComplexityN(size);
}
BENCHMARK(BM_IntArray_Move)->Range(1024, 1<<20)->Complexity();

// ========== Tokenizer ==========
static void BM_Tokenizer(benchmark::State& state) {
    std::unordered_set<std::string> dict = {"你好", "世界", "中国", "人民", "计算机", "科学"};
    Tokenizer t(dict);
    std::string text;
    for (int i = 0; i < 1000; ++i) {
        text += "你好世界中国";
    }
    for (auto _ : state) {
        auto tokens = t.tokenize(text);
        benchmark::DoNotOptimize(tokens);
    }
    state.SetBytesProcessed(state.iterations() * text.size());
}
BENCHMARK(BM_Tokenizer);

// ========== Cosine ==========
static void BM_Cosine(benchmark::State& state) {
    size_t dim = state.range(0);
    std::vector<double> a(dim, 1.0), b(dim, 2.0);
    for (auto _ : state) {
        double r = cosine_similarity(a, b);
        benchmark::DoNotOptimize(r);
    }
    state.SetComplexityN(dim);
}
BENCHMARK(BM_Cosine)->Range(128, 4096)->Complexity();

// ========== Calculator ==========
static void BM_Calculator(benchmark::State& state) {
    Calculator c;
    std::string expr = "3 + 5 * (2 - 8) / 2 - (-4)";
    for (auto _ : state) {
        double r = c.evaluate(expr);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(BM_Calculator);

// ========== ThreadSafeStack ==========
static void BM_ThreadSafeStack_PushPop(benchmark::State& state) {
    ThreadSafeStack<int> stack;
    for (auto _ : state) {
        stack.push(42);
        auto val = stack.pop();
        benchmark::DoNotOptimize(val);
    }
}
BENCHMARK(BM_ThreadSafeStack_PushPop);

BENCHMARK_MAIN();
