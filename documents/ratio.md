<ratio> 是 C++11 引入的标准库头文件，用于在编译时处理有理数（分数）及其运算。它通过模板元编程实现，支持分数的精确计算、约分、比较和单位转换等操作，常用于需要高精度比例计算的场景（如时间单位转换、物理量纲处理等）。
```cpp
#include <ratio>
using MyRatio = std::ratio<分子, 分母>; // 分母默认为1
using Half = std::ratio<1, 2>;   // 1/2
using Kilo = std::ratio<1000>;   // 1000/1 = 1000
```
