#include <ratio>
#include <iostream>

// 定义秒和毫秒的比例关系
using Seconds = std::ratio<1>;
using Milliseconds = std::ratio<1, 1000>;

// 转换因子：1秒 = 1000毫秒
using ConversionFactor = std::ratio_divide<Seconds, Milliseconds>;

int testratio() {
    // std::kilo
    std::cout << "1秒 = " << ConversionFactor::num << "/" << ConversionFactor::den
              << " 毫秒" << std::endl; // 输出：1秒 = 1000/1 毫秒
    return 0;
}