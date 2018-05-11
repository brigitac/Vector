#ifndef VECTOR_H
#define VECTOR_H
#include <chrono>
#include <exception>
#include "timer.h"

template<class T>
class Vector
{
    int size_;
    int capacity_;
    T* elem;
public:
    Vector() : size_{0}, capacity_{0}, elem{new T [size_]} {}
    Vector(int s) : size_{s}, capacity_{s}, elem{new T [size_]} {std::fill_n(elem,s,0.0);}
    Vector(int s, int val) : size_(s), capacity_(s), elem(new T [size_]) {std::fill_n(elem,s,val);}
    inline int size() const {return size_;}
    inline int capacity() const {return capacity_;}
    void push_back(int skaicius);
    Vector(const Vector& v);
    Vector& operator=(const Vector& v);
    T& operator[](int i);
    const T& operator[](int i) const;
    Vector(std::initializer_list<T> il);
    ~Vector() {delete[] elem;}
};

template<class T>
Vector<T>& Vector<T>::operator=(const Vector& v) { // priskyrimo operatorius
    T* p = new T[v.size_];
    for (int i=0; i!=v.size_; ++i) // nukopijuojame v elementus
        p[i] = v.elem[i];
    delete[] elem; // atlaisviname seną atmintį!
    elem = p;      // elem point'ina į naują atmintį
    size_ = v.size_;     // atnaujiname size
    return *this;  // grąžiname objektą
}

template<class T>
Vector<T> operator+(const Vector<T>& a, const Vector<T>& b) {
    if (a.size() != b.size())
        throw std::runtime_error("Vektorių dydžio neatitikimas!");
    auto size = a.size();
    Vector<T> c(size);
    for (auto i = 0; i != a.size(); ++i)
        c[i] = a[i] + b[i];
    return c;
}

template<class T>
T& Vector<T>::operator[](int i)
{
    if (i < 0 || size() <= i) throw std::out_of_range {"Vector::operator[]"};
    return elem[i];
}

template<class T>
const T& Vector<T>::operator[](int i) const
{
    if (i < 0 || size() <= i) throw std::out_of_range {"Vector::operator[]"};
    return elem[i];
}

template<class T>
Vector<T>::Vector(std::initializer_list<T> il)
: size_{static_cast<int>(il.size())},
elem{new T[il.size()]}
{
    std::copy(il.begin(),il.end(),elem);
}

template<class T>
Vector<T>::Vector(const Vector& v) :elem{new T[v.size_]}, size_{v.size_}
{
    for (int i=0; i!=size_; ++i)
    elem[i] = v.elem[i];
}

template<class T>
void Vector<T>::push_back(int skaicius)
{
    if (capacity_==0) {capacity_=1;}
    if (size_==capacity_) {capacity_=2*capacity_;}
    size_++;
    int *mas2=new int[size_];
    for(int i=0;i<size_;i++) mas2[i]=elem[i]; //nukopinu viena masyva i kita
    mas2[size_-1]=skaicius;
    delete[] elem;
    elem=mas2;
}

#endif
