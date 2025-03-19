// #include <cstdint>
// #include <iostream>
 
// union S
// {
//     std::int32_t n;     // occupies 4 bytes
//     std::uint32_t s[2]; // occupies 4 bytes
//     std::uint8_t c;     // occupies 1 byte
// };                      // the whole union occupies 4 bytes
 
// int testunion()
// {
//     S s = {0x12345678}; // initializes the first member, s.n is now the active member
//     // At this point, reading from s.s or s.c is undefined behavior,
//     // but most compilers define it.
//     std::cout << std::hex << "s.n = " << s.n << '\n';

//     std::cout<<"size of union:"<<sizeof(S)<<std::endl;
 
//     s.s[0] = 0x0011; // s.s is now the active member
//     // At this point, reading from s.n or s.c is undefined behavior,
//     // but most compilers define it.
//     std::cout << "s.c is now " << +s.c << '\n' // 11 or 00, depending on platform
//               << "s.n is now " << s.n << '\n'; // 12340011 or 00115678
//     return 0;
// }

#include <iostream>
#include <string>
#include <vector>
 
union S
{
    std::string str;
    std::vector<int> vec;
    ~S() {} // needs to know which member is active, only possible in union-like class 
};          // the whole union occupies max(sizeof(string), sizeof(vector<int>))
 
int testunion()
{
    S s = {"Hello, world"};
    // at this point, reading from s.vec is undefined behavior
    std::cout << "s.str = " << s.str << '\n';
    s.str.~basic_string();
    new (&s.vec) std::vector<int>;
    // now, s.vec is the active member of the union
    s.vec.push_back(10);
    std::cout << s.vec.size() << '\n';
    s.vec.~vector();
}