#include <iostream>
 
struct Foo
{
    int   i;
    float f;
    char  c;
};
 
// Note: alignas(alignof(long double)) below can be
// simplified to alignas(long double) if desired.
struct alignas(alignof(long double)) Foo2
{
    // put your definition here
}; 
 
struct Empty {};
 
struct alignas(64) Empty64 {};
 
#define SHOW(expr) std::cout << #expr << " = " << (expr) << '\n'
 
int testalignof()
{
    SHOW(alignof(char));
    SHOW(alignof(int*));
    // SHOW(alignof(Foo));
    std::cout<<sizeof(long double)<<'\n';
    SHOW(alignof(Foo2));
    SHOW(alignof(Empty));
    SHOW(alignof(Empty64));
    return 0;
}