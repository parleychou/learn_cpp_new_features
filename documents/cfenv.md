
在 C/C++ 中，<cfenv>（或 C 中的 <fenv.h>）是用于控制浮点运算环境的头文件，提供对浮点异常（如除零、溢出）和舍入模式（如四舍五入、向上取整）的底层操作。以下是核心内容：

核心功能
浮点异常处理
检测和清除浮点运算中触发的异常（如除零、无效操作、溢出等）。
舍入模式控制
动态设置浮点运算的舍入方向（如向零舍入、向正无穷舍入等）。
浮点环境状态保存/恢复
通过 fegetenv/fesetenv 保存和恢复当前浮点环境状态。
常用宏与函数
1. 浮点异常宏

定义在 <cfenv> 中的异常类型（通过按位或组合使用）：

FE_DIVBYZERO：除零异常。
FE_INVALID：无效操作（如对负数开平方）。
FE_OVERFLOW：上溢（结果超出浮点范围）。
FE_UNDERFLOW：下溢（结果精度丢失）。
FE_ALL_EXCEPT：所有支持的异常。
2. 舍入模式宏
FE_TONEAREST：四舍五入（默认模式）。
FE_UPWARD：向正无穷方向舍入。
FE_DOWNWARD：向负无穷方向舍入。
FE_TOWARDZERO：向零舍入。
3. 关键函数
异常操作：
```cpp
int feclearexcept(int excepts);  // 清除指定异常标志
int fetestexcept(int excepts);   // 检测是否发生指定异常
int feraiseexcept(int excepts);  // 手动触发异常（用于测试）
```
舍入模式控制：
```cpp
int fegetround();        // 获取当前舍入模式
int fesetround(int mode); // 设置舍入模式（返回0表示成功）
```
环境状态管理：
```cpp
int fegetenv(fenv_t *envp);  // 保存当前环境到 envp
int fesetenv(const fenv_t *envp); // 从 envp 恢复环境
```
代码示例
1. 检测浮点异常
```cpp
#include <cfenv>
#include <cmath>
#include <iostream>

int main() {
    std::feclearexcept(FE_ALL_EXCEPT); // 清除所有异常标志

    double x = 0.0;
    double y = 1.0 / x; // 触发 FE_DIVBYZERO

    if (std::fetestexcept(FE_DIVBYZERO)) {
        std::cout << "除零异常被触发！" << std::endl;
    }

    return 0;
}
```
2. 设置舍入模式
```cpp
#include <cfenv>
#include <iostream>

int main() {
    std::fesetround(FE_UPWARD); // 设置为向上舍入

    double x = 1.5;
    double y = nearbyint(x);    // 结果为 2.0

    std::cout << y << std::endl; // 输出 2
    return 0;
}
```
注意事项
编译器依赖
需启用浮点环境支持（如 GCC 使用 -frounding-math 编译选项）。
线程安全
浮点环境是全局状态，多线程中需避免竞争（需加锁或隔离操作）。
异常默认不触发信号
默认仅设置异常标志，不会中断程序。若需触发信号（如 SIGFPE），需调用 feenableexcept（非标准扩展，仅限某些系统如 Linux）。
适用场景
科学计算中需要精确控制舍入误差。
调试浮点运算中的隐蔽错误（如无效操作）。
实现高精度数值算法时管理浮点环境。