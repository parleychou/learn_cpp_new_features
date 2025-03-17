#include "function_declaration.h"


void Foo() { std::cout << __func__ << ' '; }
int testfunc()
{
    Foo();
    Bar bar;
    Bar::Pub pub;
    return 0;
}
int testrvalue()
{
    std::string s1 = "Test";
//  std::string&& r1 = s1;           // error: can't bind to lvalue
 
    const std::string& r2 = s1 + s1; // okay: lvalue reference to const extends lifetime
//  r2 += "Test";                    // error: can't modify through reference to const
    std::cout << s1<< '\n';
    std::cout << r2<< '\n';
 
    std::string&& r3 = s1 + s1;      // okay: rvalue reference extends lifetime
    r3 += "Test";                    // okay: can modify through reference to non-const
    std::cout << r3 << '\n';
    return 0;
}