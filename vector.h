#ifndef VECTOR_H
#define VECTOR_H

#include <chrono>
#include <exception>
#include <algorithm>
#include <cstring>
#include <iterator>
#include "timer.h"

#define MAX_SIZE 4611686018427387903

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
    Vector(const Vector& v);
    Vector& operator=(const Vector& v);
    Vector(std::initializer_list<T> il);
    ~Vector() {delete[] elem;}
    void push_back(const T& value);
    void push_back(T&& value);
    void pop_back();
    bool empty() const;
    void reserve(size_t new_cap );
    void resize(size_t count);
    void resize( size_t count, const T& value );
    void clear();
    void reallocate();
    size_t max_size() const;
    void shrink_to_fit();
    void swap(Vector& other);
    // Element Access
    T& at(size_t pos);
    const T& at(size_t pos) const;
    T& operator[](size_t i);
    const T& operator[](size_t i) const;
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;
    T * data() noexcept;
    const T * data() const noexcept;
    // Iterators
    T* begin() noexcept;
    const T* begin() const noexcept;
    const T* cbegin() const noexcept;
    T* end() noexcept;
    const T* end() const noexcept;
    const T* cend() const noexcept;
    std::reverse_iterator<T*>   rbegin() noexcept;
    std::reverse_iterator<const T*> rbegin() const noexcept;
    std::reverse_iterator<const T*>  crbegin() const noexcept;
    std::reverse_iterator<T*> rend() noexcept;
    std::reverse_iterator<const T*> rend() const noexcept;
    std::reverse_iterator<const T*> crend() const noexcept;
};

template<class T>
Vector<T>& Vector<T>::operator=(const Vector& v)
{
    T* p = new T[v.size_];
    for (int i=0; i!=v.size_; ++i) // nukopijuojame v elementus
        p[i] = v.elem[i];
    delete[] elem; // atlaisviname seną atmintį!
    elem = p;      // elem point'ina į naują atmintį
    size_ = v.size_;     // atnaujiname size
    return *this;  // grąžiname objektą
}

template<class T>
Vector<T> operator+(const Vector<T>& a, const Vector<T>& b)
{
    if (a.size() != b.size())
        throw std::runtime_error("Vektorių dydžio neatitikimas!");
    auto size = a.size();
    Vector<T> c(size);
    for (auto i = 0; i != a.size(); ++i)
        c[i] = a[i] + b[i];
    return c;
}

template<class T>
Vector<T>::Vector(std::initializer_list<T> il)
: size_{static_cast<int>(il.size())},
elem{new T[il.size()]}
{std::copy(il.begin(),il.end(),elem);}

template<class T>
Vector<T>::Vector(const Vector& v) :elem{new T[v.size_]}, size_{v.size_}
{
    for (int i=0; i!=size_; ++i)
    elem[i] = v.elem[i];
}

template<class T>
void Vector<T>::push_back(const T& value)
{
    if (capacity_==0) {capacity_=1;}
    if (size_==capacity_) {capacity_=2*capacity_;}
    size_++;
    reallocate();
    elem[size_-1]=value;
}

template<class T>
void Vector<T>::push_back(T &&value)
{
    if (capacity_==0) {capacity_=1;}
    if (size_==capacity_) {capacity_=2*capacity_;}
    size_++;
    reallocate();
    elem[size_-1]=value;
}

template<class T>
bool Vector<T>::empty() const
{return begin() == end();}

template<class T>
void Vector<T>::pop_back()
{size_--;}

template<class T>
void Vector<T>::reserve(size_t new_cap)
{
    T* elem2 = new T [new_cap];
    std::copy (elem, elem + size_, elem2);
    capacity_ = new_cap;
    delete[] elem;
    elem = elem2;
}

template<class T>
void Vector<T>::resize(size_t count)
{
    size_=count;
    reallocate();
}

template<class T>
void Vector<T>::resize( size_t count, const T& value )
{
    if (count>size_)
    {
        auto difference=count-size_;
        size_=count;
        reallocate();
        for (auto i=size_-difference; i < size_; ++i) elem[i] = value;
        capacity_=size_;
    }
    else
    {
        size_=count;
        reallocate();
    }
}

template <typename T>
void Vector<T>::clear()
{
    for (auto i = 0; i < size_; ++i)
        elem[i].~T();
//  delete[] elem; //kodel neleidzia istrinti?
    size_ = 0;
}

template<typename T>
void Vector<T>::reallocate()
{
    T* elem2=new T[size_];
    std::memcpy(elem2,elem,size_*sizeof(T));
    delete[] elem;
    elem=elem2;
}

template <typename T>
size_t Vector<T>::max_size() const
{return MAX_SIZE;}

template <typename T>
void Vector<T>::shrink_to_fit()
{capacity_ = size_;}

template <typename T>

void Vector<T>::swap(Vector<T> &v2)
{
    auto size_2 = size_,
    capacity_2 = capacity_;
    T *telem = elem;
    
    size_ = v2.size_;
    capacity_ = v2.capacity_;
    elem = v2.elem;
    
    v2.size_ = size_2;
    v2.capacity_ = capacity_2;
    v2.elem = telem;
}

// Element Access
template<class T>
T& Vector<T>::at(size_t pos)
{
    if (pos < 0 || size() <= pos) throw std::out_of_range{"vector"};;
    return elem[pos];
}

template<class T>
const T& Vector<T>::at(size_t pos) const
{
    if (pos < 0 || size() <= pos) throw std::out_of_range{"vector"};;
    return elem[pos];
}

template<class T>
T& Vector<T>::operator[](size_t i)
{return elem[i];}

template<class T>
const T& Vector<T>::operator[](size_t i) const
{return elem[i];}

template<class T>
T& Vector<T>::front()
{return elem[0];}

template<class T>
const T& Vector<T>::front() const
{return elem[0];}

template<class T>
T& Vector<T>::back()
{return elem[size_ - 1];}

template<class T>
const T& Vector<T>::back() const
{return elem[size_ - 1];}

template<typename T>
T* Vector<T>::data() noexcept
{return elem;}

template<typename T>
const T* Vector<T>::data() const noexcept
{return elem;}

// Iterators
template<class T>
T* Vector<T>::begin() noexcept
{return elem;}

template<class T>
const T* Vector<T>::begin() const noexcept
{return elem;}

template<class T>
const T* Vector<T>::cbegin() const noexcept
{return elem;}

template<class T>
T* Vector<T>::end() noexcept
{return elem + size();}

template<class T>
const T* Vector<T>::end() const noexcept
{return elem + size();}

template<class T>
const T* Vector<T>::cend() const noexcept
{return elem + size();}

template<class T>
std::reverse_iterator<T*> Vector<T>:: rbegin() noexcept
{return std::reverse_iterator<T*>(elem + size_);}

template<class T>
std::reverse_iterator<const T*> Vector<T>:: rbegin() const noexcept
{return std::reverse_iterator<T *>(elem + size_);}

template<class T>
std::reverse_iterator<const T*> Vector<T>:: crbegin() const noexcept
{return std::reverse_iterator<T *>(elem + size_);}

template <typename T>
std::reverse_iterator<T*> Vector<T>::rend() noexcept
{return std::reverse_iterator<T*>(elem);}

template <typename T>
std::reverse_iterator<const T*> Vector<T>::rend() const noexcept
{return std::reverse_iterator<T*>(elem);}

template <typename T>
std::reverse_iterator<const T*> Vector<T>::crend() const noexcept
{return std::reverse_iterator<T*>(elem);}

#endif
