#ifndef PRINT_RANGE_H_
#define PRINT_RANGE_H_

#include <iostream>

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

#endif