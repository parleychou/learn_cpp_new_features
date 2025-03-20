std::atomic 是 C++11 引入的原子操作模板类，用于在多线程环境中安全地操作共享数据，避免数据竞争（data race）。它通过硬件级别的原子指令（如 CAS、LL/SC）保证操作的不可分割性，无需显式使用互斥锁（mutex），从而提升并发性能。

核心特性
原子性
对 std::atomic 对象的读写操作是原子的（线程安全），不会被其他线程的操作中断。
内存顺序控制
支持指定内存顺序（如 memory_order_relaxed），优化性能或强制同步。
支持多种数据类型
可包装基础类型（如 int, bool）或满足特定条件的自定义类型（需为 trivially copyable）。
基本用法
1. 头文件与声明
cpp
#include <atomic>

// 声明原子变量
std::atomic<int> counter{0};   // 原子整型变量，初始值为0
std::atomic<bool> flag{false}; // 原子布尔变量
2. 常用操作
cpp
// 原子写（store）
counter.store(42, std::memory_order_release);

// 原子读（load）
int value = counter.load(std::memory_order_acquire);

// 原子自增（仅限整数类型）
counter.fetch_add(1, std::memory_order_relaxed);

// 原子交换（exchange）
int old = counter.exchange(100, std::memory_order_seq_cst);

// 原子比较交换（CAS）
bool success = counter.compare_exchange_strong(old, new_value, memory_order_acq_rel);
3. 内存顺序（Memory Order）
通过参数控制操作的内存可见性和顺序，常用选项：

memory_order_relaxed：仅保证原子性，无同步（性能最高）。
memory_order_acquire：当前线程的后续读操作必须在此操作之后执行。
memory_order_release：当前线程的前置写操作必须在此操作之前完成。
memory_order_seq_cst：全局顺序一致（默认，性能最低但最安全）。
适用场景
轻量级计数器
例如统计并发任务的数量：
cpp
std::atomic<int> task_count{0};
void worker() {
    task_count.fetch_add(1, std::memory_order_relaxed); // 原子自增
}
标志位控制
例如通知线程退出：
cpp
std::atomic<bool> stop_flag{false};
void thread_func() {
    while (!stop_flag.load(std::memory_order_acquire)) { // 原子读
        // 执行任务
    }
}
void stop() {
    stop_flag.store(true, std::memory_order_release); // 原子写
}
无锁数据结构
如无锁队列、栈的实现（依赖 CAS 操作）。
与互斥锁（Mutex）对比
特性	std::atomic	互斥锁（Mutex）
性能	高（无阻塞，硬件指令直接支持）	低（需上下文切换）
适用粒度	单个变量或简单操作	复杂操作或代码块
内存顺序控制	支持精细调整	无（默认全局同步）
编程复杂度	高（需理解内存模型）	低（直接加锁）
注意事项
非原子操作仍需同步
std::atomic 仅保证自身操作的原子性。若共享数据需多个操作联合生效（如先读后写），仍需用互斥锁。
cpp
// 错误示例：两个原子操作之间可能被其他线程打断
if (counter.load() > 0) {
    counter.fetch_sub(1); // 此时 counter 可能已被其他线程修改
}
自定义类型的限制
自定义类型必须满足 trivially copyable（无自定义析构函数、虚函数等）才能使用 std::atomic。
避免过度依赖宽松内存序
memory_order_relaxed 可能导致反直觉的执行顺序，仅在明确需求时使用。
示例代码
```cpp
#include <atomic>
#include <thread>
#include <vector>
#include <iostream>

std::atomic<int> sum{0};

void add(int n) {
    for (int i = 0; i < n; ++i) {
        sum.fetch_add(1, std::memory_order_relaxed); // 原子自增
    }
}

int main() {
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(add, 100000); // 4个线程各累加10万次
    }
    for (auto& t : threads) {
        t.join();
    }
    std::cout << sum.load() << std::endl; // 正确输出 400000
    return 0;
}
```
总结
std::atomic 是实现高效线程安全的关键工具，适用于简单共享变量的原子操作。但对复杂逻辑（如多变量协作），仍需结合互斥锁、条件变量等机制。理解内存顺序模型是正确使用它的核心。