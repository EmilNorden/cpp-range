#include <iostream>
#include "where.h"
#include <vector>
#include <functional>

using namespace std;

template <typename Container>
void print(Container& c)
{
    std::cout << "--begin--" << std::endl;
    for(auto& foo : c)
    {
        std::cout << foo << std::endl;
    }
    std::cout << "--end--" << std::endl;
}

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 29, 100};

    auto result1 = range::where(v, [](int x) { return x % 3 == 0; });

    print(result1);

    auto result2 = range::where(result1, [](int x) { return x % 2 == 0; });

    print(result2);

    std::cout << "done!";
}

