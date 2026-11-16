import time
import random
import subprocess
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
plt.rcParams['font.sans-serif'] = ['WenQuanYi Zen Hei']
plt.rcParams['axes.unicode_minus'] = False

def run_cpp():
    result = subprocess.run(
        ['./build/cosine_benchmark'],
        capture_output=True, text=True,
        cwd='/home/yuandu/cpp_base'
    )
    lines = result.stdout.strip().split('\n')[1:]
    data = {}
    for line in lines:
        dim, us = line.split(',')
        data[int(dim)] = int(us)
    return data

def run_python(dims, iterations=1000):
    data = {}
    for dim in dims:
        a = [random.random() for _ in range(dim)]
        b = [random.random() for _ in range(dim)]
        
        start = time.perf_counter()
        for _ in range(iterations):
            dot = sum(x*y for x, y in zip(a, b))
            norm_a = sum(x*x for x in a)
            norm_b = sum(y*y for y in b)
            if norm_a > 0 and norm_b > 0:
                _ = dot / ((norm_a ** 0.5) * (norm_b ** 0.5))
        end = time.perf_counter()
        data[dim] = (end - start) / iterations * 1e6
    return data

if __name__ == '__main__':
    dims = [128, 256, 512, 1024, 2048, 4096]
    
    cpp_data = run_cpp()
    py_data = run_python(dims)
    
    print("维度 | C++ (μs) | Python (μs) | 加速比")
    print("-----|----------|-------------|--------")
    for dim in dims:
        cpp = cpp_data.get(dim, 0)
        py = py_data.get(dim, 0)
        speedup = py / cpp if cpp > 0 else 0
        print(f"{dim:4d} | {cpp:8d} | {py:11.1f} | {speedup:5.1f}x")
    
    plt.figure(figsize=(10, 6))
    plt.plot(dims, [cpp_data[d] for d in dims], 'o-', label='C++ (循环展开)')
    plt.plot(dims, [py_data[d] for d in dims], 's-', label='Python (列表推导)')
    plt.xlabel('向量维度')
    plt.ylabel('耗时 (μs)')
    plt.title('余弦相似度性能对比：C++ vs Python')
    plt.legend()
    plt.grid(True)
    plt.xscale('log', base=2)
    plt.yscale('log')
    plt.savefig('benchmarks/cosine_comparison.png', dpi=150)
    print("\n散点图已保存到 benchmarks/cosine_comparison.png")
