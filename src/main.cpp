#include <iostream>
#include "range.h"
#include <vector>
#include <functional>

#include "print.h"
#include <string>

using namespace std;

class A
{
public:
    A(int k, std::string&& n)
        : key(k), name(n)
    {
    }
int key;
std::string name;
};

int main()
{
    std::vector<int> v = {1, 2, 3, 4, 5};

    //auto range = range::from(v).where([](int x) { return x < 4; }).skip(1);
    auto range = range::from(v).skip(1).skip(1).take(5);
    print(range);

    v.push_back(10);
    print(range);

    std::vector<A> v2;
    v2.push_back(A{30, "Emil"});
    v2.push_back(A{30, "Johan"});
    v2.push_back(A{20, "Kalle"});
    v2.push_back(A{20, "Löken"});
    v2.push_back(A{35, "Leffe"});

    auto grouped = range::from(v2).group([](A& a) { return a.key; });
    
    auto beg = std::begin(grouped);

    for(auto&& group : grouped)
    {
        std::cout << "==" << group.key() << "==" << std::endl;
        for(auto ll : group)
        {
            std::cout << ll.name << std::endl;
        }
    }
    

    std::cout << "done!";
}
