#include <iostream>
#include "range.h"
#include <vector>
#include <functional>

#include "print.h"

using namespace std;

int main()
{
    std::vector<int> v = {1, 2, 3, 4, 5};

    //auto range = range::from(v).where([](int x) { return x < 4; }).skip(1);
    auto range = range::from(v).skip(1).skip(1).take(5);
    print(range);

    v.push_back(10);
    print(range);

    std::cout << "done!";
}
