在 C++ 中，inline 是一个关键字，主要用于优化函数调用和避免链接错误。以下是其核心作用和用法：

1. 内联函数（优化函数调用）
作用：建议编译器将函数代码直接插入调用处，减少函数调用的开销（如栈帧创建）。
示例：
cpp
inline int add(int a, int b) {
    return a + b;
}
注意：
编译器可能忽略 inline 建议（如函数体过大或包含递归）。
现代编译器通常自动决定是否内联，手动指定 inline 的优化作用有限。
2. 头文件中的函数定义（避免链接错误）
问题：在头文件中定义非内联函数，多个源文件包含该头文件会导致重复定义错误。
解决：使用 inline 允许头文件中定义函数，避免链接冲突。
cpp
// header.h
inline void print() {
    std::cout << "Hello";
}
适用场景：小型工具函数、模板函数（C++17 后模板函数默认内联）。
3. 内联变量（C++17 起支持）
作用：允许在头文件中定义全局变量或静态成员变量，避免重复定义。
示例：
cpp
// header.h
inline int global_value = 42;  // 全局变量

class MyClass {
public:
    inline static int count = 0; // 静态成员变量
};
4. 与 static 的区别
特性	inline	static
作用域	跨文件共享（链接时不冲突）	文件内私有（链接时隔离）
内存占用	单一定义（所有文件共享同一实体）	每个文件有独立副本
适用场景	头文件中定义函数/变量	文件内辅助函数或变量
5. 实际应用建议
优先用于头文件函数：确保跨文件共享时无链接错误。
谨慎用于性能优化：依赖编译器决策，避免强制内联复杂函数。
结合 C++17 特性：使用 inline 变量简化全局/静态成员变量的管理。
总结
inline 的核心价值在于：

头文件安全：允许在头文件中定义函数和变量。
性能提示：为编译器提供内联优化建议（但非强制）。
简化代码（C++17+）：通过内联变量减少冗余定义。
合理使用 inline 可以提升代码可维护性，但需避免过度依赖其优化作用。


Explanation
An inline function or inline variable(since C++17) has the following properties:

The definition of an inline function or variable(since C++17) must be reachable in the translation unit where it is accessed (not necessarily before the point of access).
An inline function or variable(since C++17) with external linkage (e.g. not declared static) has the following additional properties:
There may be more than one definition of an inline function or variable(since C++17) in the program as long as each definition appears in a different translation unit and (for non-static inline functions and variables(since C++17)) all definitions are identical. For example, an inline function or an inline variable(since C++17) may be defined in a header file that is included in multiple source files.
It must be declared inline in every translation unit.
It has the same address in every translation unit.
In an inline function,

Function-local static objects in all function definitions are shared across all translation units (they all refer to the same object defined in one translation unit).
Types defined in all function definitions are also the same in all translation units.
Inline const variables at namespace scope have external linkage by default (unlike the non-inline non-volatile const-qualified variables).

(since C++17)
The original intent of the inline keyword was to serve as an indicator to the optimizer that inline substitution of a function is preferred over function call, that is, instead of executing the function call CPU instruction to transfer control to the function body, a copy of the function body is executed without generating the call. This avoids overhead created by the function call (passing the arguments and retrieving the result) but it may result in a larger executable as the code for the function has to be repeated multiple times.

Since inline substitution is unobservable in the standard semantics, compilers are free to use inline substitution for any function that's not marked inline, and are free to generate function calls to any function marked inline. Those optimization choices do not change the rules regarding multiple definitions and shared statics listed above.

Because the meaning of the keyword inline for functions came to mean "multiple definitions are permitted" rather than "inlining is preferred" since C++98, that meaning was extended to variables.