## timed_mutex
std::timed_mutex 是 C++11 引入的互斥量类型，它在普通互斥量（std::mutex）的基础上增加了超时锁定功能，允许线程在尝试获取锁时设置超时时间，避免无限阻塞。以下是其核心用法和场景：

核心功能
超时尝试加锁
提供 try_lock_for() 和 try_lock_until() 方法，允许线程在指定时间内尝试获取锁，超时后返回 false，继续执行其他逻辑。
避免死锁
在需要限制等待时间的场景（如实时系统）中，防止线程因无限等待而僵死。
成员函数
try_lock_for(timeout_duration)
在 timeout_duration 时间内尝试获取锁，成功返回 true，否则超时返回 false。
try_lock_until(timeout_time)
在 timeout_time 时间点之前尝试获取锁，成功返回 true，否则超时返回 false。
其他基础方法
与 std::mutex 一致：lock()、unlock()、try_lock()。
使用示例
```cpp
#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>

std::timed_mutex tmutex;

void task(int id) {
    auto timeout = std::chrono::milliseconds(100);  // 超时时间 100ms

    // 尝试在 100ms 内获取锁
    if (tmutex.try_lock_for(timeout)) {
        std::cout << "Thread " << id << " acquired the lock.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(50));  // 模拟临界区操作
        tmutex.unlock();
    } else {
        std::cout << "Thread " << id << " failed to acquire the lock within 100ms.\n";
    }
}

int main() {
    std::thread t1(task, 1);
    std::thread t2(task, 2);
    t1.join();
    t2.join();
    return 0;
}
```
可能输出：

Thread 1 acquired the lock. Thread 2 failed to acquire the lock within 100ms.
适用场景
实时系统
需要确保线程在指定时间内响应，避免因锁竞争导致任务超时。
资源争用优化
当锁竞争激烈时，允许线程放弃等待并执行其他逻辑（如重试或记录错误）。
避免优先级反转
在高优先级线程需要快速获取资源的场景中，防止被低优先级线程阻塞。
注意事项
正确释放锁
确保每次 lock() 或成功 try_lock_xxx() 后调用 unlock()，否则会导致死锁。
时间精度
超时时间受系统调度影响，实际等待时间可能略长于指定值。
递归锁定
std::timed_mutex 不可递归加锁（同一线程多次加锁会死锁），需改用 std::recursive_timed_mutex。
总结
std::timed_mutex 适用于需要控制锁等待时间的多线程场景，通过超时机制提升程序的健壮性和响应性。使用时需结合 try_lock_for() 或 try_lock_until() 处理超时逻辑，避免资源浪费。


## recursive_mutex
std::recursive_mutex 是 C++11 引入的递归互斥量，允许同一线程多次对其加锁，避免单一线程因重复获取锁而导致的死锁问题。以下是其核心用法和场景：

核心特性
递归加锁
同一线程可以多次调用 lock() 或 try_lock()，但必须执行相同次数的 unlock() 才能释放锁。
线程独占
与其他互斥量一样，锁被某一线程持有时，其他线程无法获取。
不可超时
若需要超时功能，需使用 std::recursive_timed_mutex。
使用场景
递归函数：在递归调用的函数中需要多次加锁同一资源。
复杂调用链：多个函数通过嵌套调用访问同一共享资源，且难以保证单次加锁。
基本用法
```cpp
#include <iostream>
#include <mutex>
#include <thread>

std::recursive_mutex rmutex;

void recursive_function(int count) {
    std::lock_guard<std::recursive_mutex> lock(rmutex); // 自动加锁
    std::cout << "Enter function, count = " << count << std::endl;
    
    if (count > 0) {
        recursive_function(count - 1); // 递归调用，再次加锁
    }
    
    std::cout << "Exit function, count = " << count << std::endl;
} // 自动解锁（每次递归解锁一次）

int main() {
    std::thread t(recursive_function, 3);
    t.join();
    return 0;
}
```
输出：

Enter function, count = 3 Enter function, count = 2 Enter function, count = 1 Enter function, count = 0 Exit function, count = 0 Exit function, count = 1 Exit function, count = 2 Exit function, count = 3
对比 std::mutex
特性	std::mutex	std::recursive_mutex
同一线程重复加锁	导致死锁（未定义行为）	允许，需对应解锁次数
性能	更高（无递归计数开销）	略低（需维护递归计数）
适用场景	简单加锁逻辑	递归或嵌套调用共享资源
注意事项
解锁次数匹配
必须确保 lock() 和 unlock() 次数严格一致，否则会导致锁无法释放或未定义行为。
避免滥用
递归锁的开销较大，仅在确实需要递归加锁时使用，优先考虑重构代码避免嵌套锁。
RAII 管理锁
使用 std::lock_guard 或 std::unique_lock 自动管理锁生命周期，避免手动解锁遗漏。
总结
std::recursive_mutex 为同一线程的递归或嵌套加锁场景提供了解决方案，但需谨慎使用以确保解锁次数正确。在复杂调用链中，优先通过设计模式（如减少共享资源依赖）来避免递归锁的使用。

## std::shared_timed_mutex
std::shared_timed_mutex 是 C++14 引入的一个同步机制，属于 <shared_mutex> 头文件。它允许实现共享锁（读锁）和独占锁（写锁），从而支持更高效的多线程访问控制。

核心概念
共享锁（Shared Lock）：允许多个读者同时访问资源，但阻止写入者。
独占锁（Exclusive Lock）：只允许一个写入者访问资源，阻止其他读者和写入者。
与 std::mutex 的对比
std::mutex 只支持独占锁，适用于写多读少的场景。
std::shared_timed_mutex 支持共享锁和独占锁，适用于读多写少的场景，提升并发性能。
使用场景
读多写少：如缓存、配置文件读取。
需要高并发读取：如数据库查询、网页服务器。
基本用法
包含头文件

cpp
#include <shared_mutex>
定义互斥锁

cpp
std::shared_timed_mutex mtx;
获取共享锁（读锁）

```cpp
std::shared_lock<std::shared_timed_mutex> read_lock(mtx);
```
获取独占锁（写锁）

```cpp
std::unique_lock<std::shared_timed_mutex> write_lock(mtx);
```
示例代码
```cpp
#include <iostream>
#include <thread>
#include <shared_mutex>
#include <vector>

std::shared_timed_mutex mtx;
int counter = 0;

// 读者线程
void reader(int id) {
    for (int i = 0; i < 3; ++i) {
        std::shared_lock<std::shared_timed_mutex> lock(mtx);
        std::cout << "Reader " << id << " reads counter: " << counter << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

// 写者线程
void writer(int id) {
    for (int i = 0; i < 3; ++i) {
        std::unique_lock<std::shared_timed_mutex> lock(mtx);
        std::cout << "Writer " << id << " writes counter: " << ++counter << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

int main() {
    std::vector<std::thread> threads;

    // 启动3个读者和2个写者
    for (int i = 0; i < 3; ++i) {
        threads.emplace_back(reader, i);
    }
    for (int i = 0; i < 2; ++i) {
        threads.emplace_back(writer, i);
    }

    for (auto& th : threads) {
        th.join();
    }

    return 0;
}
```
输出示例
Reader 0 reads counter: 0 Reader 1 reads counter: 0 Reader 2 reads counter: 0 Writer 0 writes counter: 1 Reader 0 reads counter: 1 Reader 1 reads counter: 1 Writer 1 writes counter: 2 Reader 2 reads counter: 2 Reader 0 reads counter: 2 Reader 1 reads counter: 2 Writer 0 writes counter: 3 Reader 2 reads counter: 3 Reader 0 reads counter: 3 Reader 1 reads counter: 3 Writer 1 writes counter: 4 Reader 2 reads counter: 4 Reader 0 reads counter: 4 Reader 1 reads counter: 4
关键特性
读者优先：默认情况下，读者可以同时进入，提升并发效率。
写者阻塞：写者必须等待所有读者释放共享锁后才能获得独占锁。
公平性：C++20 引入了 try_lock 和 try_lock_for，允许设置锁超时，避免死锁。
注意事项
死锁风险：需确保锁的获取顺序一致，避免循环等待。
性能优化：适用于读操作远多于写操作的场景，否则可能因写者阻塞而降低性能。
总结
std::shared_timed_mutex 提供了更灵活的锁机制，适用于读多写少的场景，通过允许多个读者同时访问，显著提升了系统的并发性能。在现代多线程编程中，合理使用 shared_timed_mutex 可以有效优化资源访问效率，减少线程间的竞争和阻塞。