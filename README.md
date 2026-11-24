# cpp_base

一组 C++ 基础组件：动态数组、线程池、线程安全栈、中文分词、表达式求值、余弦相似度。
目标不是造框架，而是用基准测试量化“同一功能，C++ 实现和 Python 实现差多少”。

## 这是什么 / 不是什么

**是**：6 个互相独立、可单独取用的小组件，其中 5 个有单元测试，IntArray 另有完整的 Google Benchmark 套件。
**不是**：通用基础库、STL 替代品，也不承诺 API 稳定——它是一份可验证的工程练习。

## 架构

```text
┌──────────────┐  ┌──────────────┐  ┌──────────────┐
│   tests/     │  │ benchmarks/  │  │  Python 调用 │
│ (GoogleTest) │  │(Google Bench)│  │  (pybind11)  │
└──────┬───────┘  └──────┬───────┘  └──────┬───────┘
       │                 │                 │
       └─────────────────┼─────────────────┘
                         │ 依赖
       ┌─────────────────▼──────────────────────────┐
       │              核心库（本项目代码）            │
       │  IntArray   ThreadPool   ThreadSafeStack   │
       │  Tokenizer  Calculator   Cosine            │
       └─────────────────▲──────────────────────────┘
                         │ 构建
       ┌────────────────────────────────────────────┐
       │  CMake  +  第三方：GoogleTest / Benchmark   │
       └────────────────────────────────────────────┘
```

依赖方向是单向的：调用方依赖核心库，核心库不依赖调用方。

## 模块

| 模块 | 解决什么问题 |
|---|---|
| `IntArray` | 用 RAII 管理堆内存，拷贝/移动语义明确，避免泄漏和 double free |
| `ThreadPool` | 提交任务后立刻返回 `future`，避免为每个任务创建一个线程 |
| `ThreadSafeStack` | 给栈加互斥锁和条件变量，多线程 push/pop 不必自己管同步 |
| `Tokenizer` | 把中文句子切成词，供余弦相似度等下游使用 |
| `Calculator` | 递归下降解析表达式，支持 `+ - * /`、括号和负数，不依赖第三方库 |
| `Cosine` | C++ 版余弦相似度，用于和 Python 版本做同口径对比 |

## 快速开始

依赖：CMake ≥ 3.16、支持 C++17 的编译器、Google Benchmark（系统包）、GoogleTest（由 CMake 自动拉取）。

```bash
# Debian/Ubuntu
sudo apt install libbenchmark-dev

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
ctest --test-dir build --output-on-failure
```

跑基准：

```bash
./build/benchmark_suite --benchmark_filter=IntArray
```

## 基准结果

环境：16 vCPU @ 3.69 GHz，L1d 48 KiB / L2 3 MiB / L3 18 MiB，Linux，GCC `-O3 -DNDEBUG`。

| 基准 | 1024 元素 | 1M 元素 |
|---|---|---|
| Copy | 39.0 ns | 169.0 µs |
| Move | 35.1 ns | 74.3 µs |
| Construct | 35.4 ns | 74.0 µs |

**方法说明**：移动构造会把源对象掏空，所以 `Move` 组必须在每轮循环内重建源对象；`Copy` 组的源对象在循环外只构造一次。两者直接比较是不公平的，因此额外测了 `Construct` 作为基线：

- `Move − Construct` 在 1M 规模为 +0.3 µs（噪声内）→ 移动是 O(1)，开销可忽略；
- `Copy ≈ 2.3 × Construct`（1M 时 169 µs vs 74 µs）→ 差额即 memcpy 成本。

单元测试：32 个用例，`ctest` 全部通过（GoogleTest）。

## 设计取舍

1. **线程池用 `condition_variable` 而非自旋等待**：节省 CPU，代价是唤醒有微秒级延迟。证据：`thread_pool.cpp` 中 worker 用 `cv_.wait(...)` 阻塞，而非 `while` 轮询。
2. **分词用最大匹配而非 Trie**：实现简单、词典易改，代价是单次匹配 O(4n)，且效果强依赖词典质量。证据：`tokenizer.cpp` 中 `max_len = 4`，从长到短逐级尝试。
3. **队列用 `std::queue + mutex` 而非无锁队列**：正确性容易保证，代价是高并发下有锁竞争。证据：成员 `tasks_` 为 `std::queue`，入队/出队都在锁保护下。

## 已知限制

- 系统安装的 Google Benchmark 以 Debug 模式编译，运行时提示 `Library was built as DEBUG`。绝对耗时可能略有偏差；本报告所有对比在同一库、同一环境下测得，相对结论不受影响。
- `Tokenizer` 的词典需手动 `load_dict()` 加载，没有内置默认词典；最长匹配固定 4 个字符。
- `Cosine` 模块暂无单元测试，仅有基准数据。
- 线程池队列无界，没有任务窃取和优先级。

