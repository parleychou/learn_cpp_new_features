#ifndef __function__declaration__
#define __function__declaration__
#include <string>
#include <iostream>


void Foo();
 
struct Bar
{
    Bar() { std::cout << __func__ << ' '; }
    ~Bar() { std::cout << __func__ << ' '; }
    struct Pub { Pub() { std::cout << __func__ << ' '; } };
};

int testfunc();

int testrvalue();

#endif