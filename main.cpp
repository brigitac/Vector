#include <iostream>
#include <vector>
#include "vector.h"

int main()
{
    std::vector<int> v {};
    Vector<int>V;
    Timer t;
    for (int i = 1; i <= 10000; ++i)
    {
        v.push_back(i);
    }
    std::cout << "std::vector užpildyti užtruko: "
    << t.elapsed() << " s\n";
    t.reset();
    for (int i = 1; i <= 10000; ++i)
    {
        V.push_back(i);
    }
    std::cout << "class vector užpildyti užtruko: "
    << t.elapsed() << " s\n";
    std::cout << "std::vector size yra " << v.size()
    << ", capacity yra " << v.capacity() << "\n";
    std::cout << "Vector size yra " << V.size()
    << ", capacity yra " << V.capacity() << "\n";
}
