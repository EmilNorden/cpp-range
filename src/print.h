#ifndef PRINT_RANGE_H_
#define PRINT_RANGE_H_

#include <iostream>

template <typename Container>
void print(Container &c)
{

    for (auto &item : c)
    {
        std::cout << item << ",";
    }

    std::cout << std::endl;
}

#endif