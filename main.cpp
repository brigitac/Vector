#include <iostream>
#include <vector>
#include "vector.h"
int main()
{
    std::vector<int> v {};
    Vector<int>V;
    Timer t;
    for (int i = 1; i <= 100; ++i)
    {
        v.push_back(i);
    }
    std::cout << "std::vector užpildyti užtruko: "
    << t.elapsed() << " s\n";
    t.reset();
    for (int i = 1; i <= 100; ++i)
    {
        V.push_back(i);
    }
    std::cout << "class vector užpildyti užtruko: "
    << t.elapsed() << " s\n";
    std::cout << "std::vector size yra " << v.size()
    << ", capacity yra " << v.capacity() << "\n";
    std::cout << "Vector size yra " << V.size()
    << ", capacity yra " << V.capacity() << "\n";
    
//   v.clear();
//   V.clear();
//    std::cout <<v.begin()<<'\n';
//    std::cout <<V.begin()<<'\n'; //ar cia irgi turi neleisti isvesti?
//
//   std::cout << "std::vector size yra " << v.size()
//   << ", capacity yra " << v.capacity() << "\n";
//   std::cout << "Vector size yra " << V.size()
//   << ", capacity yra " << V.capacity() << "\n";
    
//    auto it = v.begin();
//    it = v.insert(it, 200);
//    auto it2 = V.begin();
//    it2 = V.insert(it2, 200);
//    std::cout<<v[0]<<" "<<V[0];
//    std::cout << "std::vector size yra " << v.size()
//    << ", capacity yra " << v.capacity() << "\n";
//    std::cout << "Vector size yra " << V.size()
//    << ", capacity yra " << V.capacity() << "\n";
//
//    v.emplace_back(777);
//    std::cout<<v[100]<<std::endl;
//    V.emplace_back(777);
//    std::cout<<v[100]<<std::endl;
//    v.pop_back();
//    std::cout<<v[100]<<std::endl;
//    V.pop_back();
//    std::cout<<v[100]<<std::endl;
//    
//    v.assign(5, 101);
//    V.assign(5, 101);
//    for (int c : v)
//    {
//        std::cout << c << '\n';
//    }
//    std::cout<<V[0]<<" "<<V[1]<<" "<<V[2]<<" "<<V[3]<<" "<<V[4]<<" "<<V[5]<<" "<<V[6]<<" ";
    
}
