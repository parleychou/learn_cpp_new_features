## uniform_random_bit_generator
uniform_random_bit_generator 是 C++20 引入的一个概念（concept），用于约束类型必须能够生成均匀分布的随机二进制位（即每个比特位是 0 或 1 的概率相等）。它是构建更复杂随机数生成器（如伪随机数引擎）的基础。
核心定义
一个类型 G 满足 uniform_random_bit_generator，需满足以下条件：
生成均匀分布
调用 G::operator() 返回的每个比特位独立且等概率为 0 或 1。
返回值范围
G::min() 和 G::max() 需返回生成器的最小和最大可能值，且满足 G::min() < G::max()。
类型别名
必须定义 result_type，表示生成器返回值的类型（通常为无符号整型）。
标准库中的实现
以下标准库组件满足 uniform_random_bit_generator：
伪随机引擎：如 std::mt19937、std::minstd_rand。
真随机设备：std::random_device（若硬件支持）。
1. 使用标准随机引擎
```cpp
#include <random>
#include <iostream>

int main() {
    std::mt19937 gen(std::random_device{}()); // 使用 mt19937 引擎
    // 验证是否满足 uniform_random_bit_generator
    static_assert(std::uniform_random_bit_generator<std::mt19937>);

    // 生成随机数（范围为 [gen.min(), gen.max()]）
    std::cout << "Random value: " << gen() << std::endl;
    return 0;
}
```
2. 自定义满足概念的生成器
```cpp
#include <concepts>
#include <cstdint>
#include <random>

struct SimpleBitGenerator {
    using result_type = uint32_t; // 必须定义 result_type

    static constexpr result_type min() { return 0; }
    static constexpr result_type max() { return 1; }

    result_type operator()() {
        // 生成 0 或 1（等概率）
        return std::rand() % 2;
    }
};

// 验证自定义类型是否符合概念
static_assert(std::uniform_random_bit_generator<SimpleBitGenerator>);
```
通过 requires 子句约束模板参数：
```cpp
#include <concepts>
#include <iostream>

template <typename G>
requires std::uniform_random_bit_generator<G>
void generate_random_bits(G& gen) {
    for (int i = 0; i < 10; ++i) {
        std::cout << gen() << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::mt19937 gen;
    generate_random_bits(gen); // 合法：std::mt19937 满足概念
    return 0;
}
```


## std::linear_congruential_engine
std::linear_congruential_engine 是 C++ 标准库中提供的一种线性同余伪随机数生成器，基于以下递推公式生成伪随机数序列：

X_{n+1} = (a \times X_n + c) \mod mX 
n+1
​
 =(a×X 
n
​
 +c)modm

其中：

X_nX 
n
​
  是当前状态值。
aa 是乘数（multiplier）。
cc 是增量（increment）。
mm 是模数（modulus），且 m > 0m>0。

核心特性
简单高效
计算速度快，适合对性能要求高但对随机性要求不严苛的场景。
可预测性
生成的序列完全由初始种子（X_0X 
0
​
 ）和参数 a, c, ma,c,m 决定，适合需要复现随机结果的场景（如测试）。
周期性
序列周期长度取决于参数选择（最大周期为 mm，需满足 Hull-Dobell 定理的条件）。

```cpp
#include <iostream>
#include <random>

int main() {
    // 使用预定义的线性同余引擎 minstd_rand
    std::minstd_rand engine(42); // 种子为 42

    // 生成 5 个随机数
    for (int i = 0; i < 5; ++i) {
        std::cout << engine() << " ";
    }
    // 输出示例：705894 1126542225  ...
    return 0;
}
```

```cpp
#include <iostream>
#include <random>

int main() {
    std::minstd_rand engine(std::random_device{}()); // 随机种子
    std::uniform_int_distribution<int> dist(1, 6);    // 生成 1~6 的整数

    // 模拟掷骰子
    for (int i = 0; i < 5; ++i) {
        std::cout << dist(engine) << " ";
    }
    // 输出示例：3 5 2 6 1
    return 0;
}
```