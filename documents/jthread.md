在 C++20 中引入的 std::jthread（全称 joining thread）是 std::thread 的改进版本，它在简化线程管理和安全性方面有显著提升。以下是其核心特性和与 std::thread 的对比：

核心特性
自动资源管理（RAII 机制）

析构时自动 join()：当 jthread 对象被销毁时，若关联的线程仍在运行，会自动调用 join()，等待线程结束，避免资源泄漏。
对比 std::thread：传统的 std::thread 在析构时若未显式调用 join() 或 detach()，会导致程序终止（调用 std::terminate()）。
协作式线程取消（支持停止请求）

内置 std::stop_token：jthread 内部维护一个停止令牌（stop_token），允许外部请求线程终止。
示例：线程函数可通过检查 stop_token 是否被请求停止，实现优雅退出。
基本用法
1. 创建并运行线程
cpp
#include <iostream>
#include <thread>

int main() {
    std::jthread t([] {
        std::cout << "Thread is running\n";
    });

    // 析构时自动 join()
}
2. 使用停止令牌（stop_token）
cpp
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    std::jthread t([](std::stop_token stoken) {
        while (!stoken.stop_requested()) {
            std::cout << "Working...\n";
            std::this_thread::sleep_for(1s);
        }
        std::cout << "Thread stopped\n";
    });

    std::this_thread::sleep_for(3s);
    t.request_stop();  // 请求线程停止
}
与 std::thread 的对比
特性	std::jthread	std::thread
析构行为	自动 join()	未 join() 或 detach() 时终止程序
停止机制	支持 stop_token	无内置停止机制
资源管理	更安全（RAII）	需手动管理
兼容性	C++20 及以上	C++11 及以上
适用场景
需要自动管理线程生命周期：避免忘记 join() 或 detach() 导致的错误。
需要协作式线程终止：例如，后台任务需响应外部停止请求。
简化多线程代码：减少手动资源管理代码，提升安全性。
总结
std::jthread 通过以下改进成为更安全的线程管理工具：

自动析构行为：避免资源泄漏。
内置停止机制：实现线程的优雅终止。
与标准库无缝集成：支持 stop_token 和 stop_source。
若使用 C++20 或更高版本，优先选择 std::jthread 替代 std::thread，尤其在需要安全性和协作式终止的场景中。