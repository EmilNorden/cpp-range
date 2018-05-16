#include <iostream>
#include "where.h"
#include <vector>
#include <functional>

using namespace std;

template <typename Container>
void print(Container& c)
{
    std::cout << "--begin--" << std::endl;

    auto b = std::begin(c);
    std::cout << "really begin" << std::endl;
    auto e = std::end(c);

    
    for(auto i = b; i != e; ++i)
    {
        std::cout << *i << std::endl;
    }
    // for(auto& foo : c)
    // {
    //     std::cout << foo << std::endl;
    // }
    std::cout << "--end--" << std::endl;
}

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};

    auto result1 = range::where(v, [](int x) { return x < 4; });
    auto result2 = range::skip(result1, 1);

    auto range = range::from(v);
    auto qwe = [](int x) { return x < 4; };
    auto range2 = range.where(qwe);

    print(range2);


    // print(result2); // prints 2, 3
    
    // v.push_back(-5);

    // print(result2); // prints 2, 3, -5

    std::cout << "done!";
}

