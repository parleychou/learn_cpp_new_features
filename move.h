#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
 
// struct A
// {
//     std::string s;
//     int k;
 
//     A() : s("test"), k(-1) {
//         std::cout<<"construct function called,current address:"<<this<<"\n";
//     }
//     A(const A& o) : s(o.s), k(o.k) { std::cout << "copy function called! move failed!\n"; }
//     A(A&& o) noexcept :
//         s(std::move(o.s)),       // explicit move of a member of class type
//         k(std::exchange(o.k, 0)) // explicit move of a member of non-class type
//     {
//         std::cout<<"move function called\n";
//     }
// };
 
// A f(A a)
// {
//     std::cout<<"address of a:"<<&a<<"\n";
//     return a;
// }
 
// struct B : A
// {
//     std::string s2;
//     int n;
//     // implicit move constructor B::(B&&)
//     // calls A's move constructor
//     // calls s2's move constructor
//     // and makes a bitwise copy of n
// };
 
// struct C : B
// {
//     ~C() {} // destructor prevents implicit move constructor C::(C&&)
// };
 
// struct D : B
// {
//     D() {}
//     ~D() {}           // destructor would prevent implicit move constructor D::(D&&)
//     D(D&&) = default; // forces a move constructor anyway
// };
 
// int testmove()
// {
//     std::cout << "Trying to move A\n";
//     A a1 = f(A()); // return by value move-constructs the target
//                    // from the function parameter
//     std::cout<<"address of a1:"<<&a1<<"\n";
 
//     std::cout << "Before move, a1.s = " << std::quoted(a1.s)
//         << " a1.k = " << a1.k << '\n';
 
//     A a2 = std::move(a1); // move-constructs from xvalue
//     std::cout << "After move, a1.s = " << std::quoted(a1.s)
//         << " a1.k = " << a1.k << '\n';
 
 
//     std::cout << "\nTrying to move B\n";
//     B b1;
 
//     std::cout << "Before move, b1.s = " << std::quoted(b1.s) << "\n";
 
//     B b2 = std::move(b1); // calls implicit move constructor
//     std::cout << "After move, b1.s = " << std::quoted(b1.s) << "\n";
 
 
//     std::cout << "\nTrying to move C\n";
//     C c1;
//     C c2 = std::move(c1); // calls copy constructor
 
//     std::cout << "\nTrying to move D\n";
//     D d1;
//     D d2 = std::move(d1);
//     return 0;
// }