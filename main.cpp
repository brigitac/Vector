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
//        std::vector<int> v2 {1,2,3};
//        Vector<int> V2{1,2,3};
//    v={1,2,3};
//    V={1,2,3};
//        v2=v;
//        V2=V;

   //      std::vector<int>v2(v);
   //      Vector<int> V2(V);
   //         std::cout << "std::vector size yra " << v2.size()
   // << ", capacity yra " << v2.capacity() << "\n";
   // std::cout << "Vector size yra " << V2.size()
   // << ", capacity yra " << V2.capacity() << "\n";
       //        std::cout<<v2[0]<<" "<<v2[1]<<" "<<v2[2]<<" "<<v2[3]<<" "<<v2[4]<<" "<<v2[5]<<" "<<v2[6]<<" "<<std::endl;
       // std::cout<<V2[0]<<" "<<V2[1]<<" "<<V2[2]<<" "<<V2[3]<<" "<<V2[4]<<" "<<V2[5]<<" "<<V2[6]<<" ";


        // std::vector<int>v3(v2.begin(),v2.end());
        // Vector<int> V3(V2);
        // std::cout<<v3[0]<<" "<<v3[1]<<" "<<v3[2]<<" "<<v3[3]<<" "<<v3[4]<<" "<<v3[5]<<" "<<v3[6]<<" "<<std::endl;
       // std::cout<<V3[0]<<" "<<V3[1]<<" "<<V3[2]<<" "<<V3[3]<<" "<<V3[4]<<" "<<V3[5]<<" "<<V3[6]<<" ";

}
