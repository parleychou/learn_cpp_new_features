#include "function_declaration.h"



void Foo() { std::cout << __func__ << ' '; }
int testfunc()
{
    Foo();
    Bar bar;
    Bar::Pub pub;
    return 0;
}
