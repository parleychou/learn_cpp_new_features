#include <charconv>
#include <iomanip>
#include <iostream>
#include <optional>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
 
std::optional<int> to_int(std::string_view sv)
{
    int r{};
    auto [ptr, ec]{std::from_chars(sv.data(), sv.data() + sv.size(), r)};
    if (ec == std::errc())
        return r;
    else
        return std::nullopt;
}
 
int test_optional()
{
    using namespace std::literals;
 
    const std::vector<std::optional<std::string>> v
    {
        "1234", "15 foo", "bar", "42", "5000000000", " 5", std::nullopt, "-43"
    };

    /*v | std::views::transform(...)：
使用 C++20 的 范围适配器（Range Adaptor） 语法，将 vector<optional<string>> v 中的每个元素通过管道操作符 | 传递给 std::views::transform，对每个元素进行转换。

auto&& x：
使用通用引用（万能引用）遍历转换后的范围，x 的类型是转换后的结果（此处为 std::string）。

*/
 
    for (auto&& x : v | std::views::transform(
        [](auto&& o)
        {
            // debug print the content of input optional<string>
            std::cout << std::left << std::setw(13)
                      << std::quoted(o.value_or("nullopt")) << " -> ";
 
            return o
                // if optional is nullopt convert it to optional with "" string
                .or_else([]{ return std::optional{""s}; })
                // flatmap from strings to ints (making empty optionals where it fails)
                .and_then(to_int)
                // map int to int + 1
                .transform([](int n) { return n + 1; })
                // convert back to strings
                .transform([](int n) { return std::to_string(n); })
                // replace all empty optionals that were left by
                // and_then and ignored by transforms with "NaN"
                .value_or("NaN"s);
        }))
        {
            
            std::cout << x << '\n';
        }
    return 0;
}