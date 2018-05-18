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
    size_t size_;
    size_t capacity_;
    T* elem;
public:
    // Member functions
    Vector();
// // vector() noexcept(noexcept(Allocator()));
// // explicit vector(const Allocator& alloc) noexcept;
    Vector(size_t count, const T &val);
// // vector( size_t count, const T& value, const Allocator& alloc = Allocator());
    explicit Vector(size_t count);
// // explicit vector(size_t count, const Allocator& alloc = Allocator() );
    Vector(T* first, T* last);
// // template< class InputIt > vector( InputIt first, InputIt last, const Allocator& alloc = Allocator() );
    Vector(const Vector& other);
// //vector(const Vector& other, const Allocator& alloc );
    Vector(Vector&& other) noexcept;
// // vector(Vector&& other) noexcept;
// // vector(Vector&& other, const Allocator& alloc );
    Vector(std::initializer_list<T> init);
// // vector(std::initializer_list<T> init, const Allocator& alloc = Allocator() );
    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other);
// // vector& operator=( vector&& other ) noexcept(/* see below */);
    Vector& operator=( std::initializer_list<T> ilist );
    void assign(T count, const T& value);
    void assign(T* first, T* last);
    void assign(std::initializer_list<T> ilist);
    ~Vector();
    // Element Access
    T& at(size_t pos);
    const T& at(size_t pos) const;
    T& operator[](size_t i);
    const T& operator[](size_t i) const;
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;
    T* data() noexcept;
    const T* data() const noexcept;
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
    // Capacity
    bool empty() const noexcept;
    size_t size() const noexcept;
    size_t max_size() const noexcept;
    void reserve(size_t new_cap);
    size_t capacity() const noexcept;
    void shrink_to_fit();
    // Modifiers
    void clear() noexcept;
    T* insert(const T* pos, const T& value );
    T* insert(const T* pos, T&& value );
    T* insert(const T* pos, size_t count, const T& value);
    T* insert(const T* pos, T* first, T* last );
    T* insert(const T* pos, std::initializer_list<T> ilist);
    template <class ... Args> T* emplace(const T*, Args && ...);
    T* erase(const T* pos);
    T* erase(const T* first, const T* last);
    void push_back(const T& value);
    void push_back(T&& value);
    template <class ... Args> void emplace_back(Args && ... args); //dar yra kazkoks since 17
    void pop_back();
    void resize(size_t count);
    void resize( size_t count, const T& value );
    void swap(Vector& other);
    // Other
    void reallocate();
};

// Member functions
template<class T>
Vector<T>::Vector() : size_{0}, capacity_{0}, elem{new T [capacity_]} {}

template <typename T>
Vector<T>::Vector(size_t count, const T &value) : size_{count}, capacity_{count}, elem{new T [capacity_]} 
{std::fill_n(elem,count,value);}

template <typename T>
Vector<T>::Vector(size_t count) : size_{count}, capacity_{count}, elem{new T [capacity_]} 
{std::fill_n(elem,count,T());}

template <typename T>
Vector<T>::Vector(T* first, T* last) 
{
    auto count = last - first;
    capacity_ = count;
    elem = new T[capacity_];
    for (auto i = 0; i < count; ++i, ++first) elem[i] = *first;
    size_ = count;
}

template<class T>
Vector<T>::Vector(const Vector& other) :elem{new T[other.size_]}, size_{other.size_}, capacity_{other.capacity_}
{for (int i=0; i!=size_; ++i) elem[i] = other.elem[i];}

template <typename T>
Vector<T>::Vector(Vector&& other) noexcept :elem{new T[other.size_]}, size_{other.size_}, capacity_{other.capacity_} 
{for (auto i = 0; i < other.size_; ++i)  elem[i] = std::move(other.elem[i]);}

template<class T>
Vector<T>::Vector(std::initializer_list<T>init) : size_{static_cast<size_t>(init.size())}, elem{new T[init.size()]}, capacity_{static_cast<size_t>(init.size())}
{std::copy(init.begin(),init.end(),elem);}

template<class T>
Vector<T>& Vector<T>::operator=(const Vector& other)
{
    if (&other == this) return *this;
    if (capacity_<other.size_) capacity_=other.size_;
    T* p = new T[other.size_];
    for (int i=0; i!=other.size_; ++i) p[i] = other.elem[i];
    delete[] elem;
    elem = p;
    size_ = other.size_;
    return *this;
}

template<class T>
Vector<T>& Vector<T>::operator=(Vector&& other)
{
    if (&other == this) return *this;
    delete[] elem;
    elem = other.elem;
    size_ = other.size_;
    capacity_=other.capacity_;
    other.elem = nullptr;
    other.size_ = 0;
    other.capacity_=0;
    return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator = (std::initializer_list<T> ilist)
{
    if (capacity_<ilist.size()) capacity_=ilist.size();
    T* p = new T[ilist.size()];
    auto i=0;
    for (auto &item:ilist)  p[i++] = item;
    size_=ilist.size();
    delete[] elem;
    elem = p;
    return *this;
}

template <typename T>
void Vector<T>::assign(T count, const T& value)
{
    if (count > capacity_)
    {
        capacity_ = count*2;
        reallocate();
    }
    std::fill_n(elem,count,value);
    std::cout<<"as cia";
    size_ = count;
}

template <typename T>
void Vector<T>::assign(T* first, T* last)
{
    auto count = last - first;
    if (count > capacity_)
    {
        capacity_ = count*2;
        reallocate();
    }
    for (auto i = 0; i < count; ++i, ++first) elem[i] = *first;
    size_ = count;
}

template <typename T>
void Vector<T>::assign(std::initializer_list<T> ilist)
{
    auto count = ilist.size();
    if (count > capacity_)
    {
        capacity_ = count * 2;
        reallocate();
    }
    auto i = 0;
    for (auto &item: ilist) elem[i++] = item;
}

template<class T>
Vector<T>::~Vector() {delete[] elem;}

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

// Capacity
template<class T>
bool Vector<T>::empty() const noexcept
{return begin() == end();}

template<class T>
size_t Vector<T>::size() const noexcept
{return size_;}

template <typename T>
size_t Vector<T>::max_size() const noexcept
{return MAX_SIZE;}

template<class T>
void Vector<T>::reserve(size_t new_cap)
{
    if (new_cap>size_)
    {
        capacity_ = new_cap;
        reallocate();
    }
}

template<class T>
size_t Vector<T>::capacity() const noexcept
{return capacity_;}

template <typename T>
void Vector<T>::shrink_to_fit()
{
    capacity_ = size_;
    reallocate();
}

// Modifiers
template <typename T>
void Vector<T>::clear() noexcept
{
    for (auto i = 0; i < size_; ++i)
        elem[i].~T();
//      delete[] elem; //kodel neleidzia istrinti?
    size_ = 0;
}

template <typename T>
T* Vector<T>::insert(const T* pos, const T& value)
{
    T* pos2 = &elem[pos - elem];
    if (size_==capacity_)
    {
        capacity_=capacity_*2;
        reallocate();
    }
    std::copy(pos2-1, pos2+(size_-(pos-elem)), pos2);
    (*pos2) = value;
    ++size_;
    return pos2;
}

template <typename T>
T* Vector<T>::insert(const T* pos, T&& value )
{
    T* pos2 = &elem[pos - elem];
    if (size_==capacity_)
    {
        capacity_=capacity_*2;
        reallocate();
    }
    std::copy(pos2-1, pos2+(size_-(pos-elem)), pos2);
    (*pos2) = std::move(value);
    ++size_;
    return pos2;
}

template <typename T>
T* Vector<T>::insert(const T* pos, size_t count,const T& value)
{
    T* pos2 = &elem[pos - elem];
    if (count==0) return pos2;
    if (size_ + count > capacity_)
    {
        capacity_ = (size_ + count) << 2;
        reallocate();
    }
    std::copy(pos2-count, pos2+(size_-(pos-elem)), pos2);
    size_ += count;
    for (T* pos = pos2; count--; ++pos)
        (*pos) = value;
    return pos2;
}

template <typename T>
T* Vector<T>::insert(const T* pos, T* first, T* last)
{
    T* pos2 = &elem[pos - elem];
    size_t count = last - first;
    if (count==0) return pos2;
    if (size_ + count > capacity_)
    {
        capacity_ = (size_ + count) << 2;
        reallocate();
    }
    std::copy(pos2-count, pos2+(size_-(pos-elem)), pos2);
    for (T* it = pos2; first != last; ++pos, ++first)
        (*pos) = *first;
    size_ += count;
    return pos2;
}

template <typename T>
T* Vector<T>::insert(const T* pos, std::initializer_list<T> ilist)
{
    T* pos2 = &elem[pos - elem];
    size_t count = ilist.size();
    if (count==0) return pos2;
    if (size_ + count > capacity_)
    {
        capacity_ = (size_ + count) << 2;
        reallocate();
    }std::copy(pos2-count, pos2+(size_-(pos-elem)), pos2);
    std::copy(pos2-count, pos2+(size_-(pos-elem)), pos2);
    T* pos3 = pos2;
    for (auto &item: ilist)
    {
        (*pos3) = item;
        ++pos3;
    }
    size_ += count;
    return pos2;
}

template <typename T>
template <class ... Args>
T* Vector<T>::emplace(const T*  pos, Args && ... args)
{
    T* pos2 = &elem[pos - elem];
    if (size_ == capacity_) {
        capacity_=capacity_ *2;
        reallocate();
    }
    std::copy(pos2-1, pos2+(size_-(pos-elem)), pos2);
    (*pos2) = std::move( T( std::forward<Args>(args) ... ) );
    ++size_;
    return pos2;
}

template <typename T>
T* Vector<T>::erase(const T* pos)
{
    T* pos2 = &elem[pos - elem];
    (*pos2).~T();
    std::copy(pos2+1, pos2+(size_-(pos-elem)), pos2);
    --size_;
    return pos2;
}

template <typename T>
T* Vector<T>::erase(const T* first, const T* last)
{ 
    std::cout<<"as esu cia :)"<<std::endl;
    T* pos = &elem[first - elem];
    if (first == last) return pos;
    for (;first != last; ++first) (*first).~T();
    std::copy(last, last+(size_-(last-elem)), pos);
    size_ -= last - first;
    return pos;
}

template<class T>
void Vector<T>::push_back(const T& value)
{
    if (capacity_==0)
    {
        capacity_=1;
        reallocate();
    }
    if (size_==capacity_)
    {
        capacity_=2*capacity_;
        reallocate();
    }
    size_++;
    elem[size_-1]=value;
}

template<class T>
void Vector<T>::push_back(T &&value)
{
    if (capacity_==0) {capacity_=1;}
    if (size_==capacity_) {capacity_=2*capacity_;}
    size_++;
    reallocate();
    elem[size_-1]=std::move(value);
}

template <typename T>
template <class ... Args>
void Vector<T>::emplace_back(Args && ... args)
{
    if (size_ == capacity_)
    {
        capacity_ = capacity_*2;
        reallocate();
    }
    elem[size_] = std::move(T(std::forward<Args>(args) ...));
    ++size_;
}

template<class T>
void Vector<T>::pop_back()
{
    size_--;
    elem[size_].~T();
}

template<class T>
void Vector<T>::resize(size_t count)
{
    if (count > size_)
    {
        if (count > capacity_)
        {
            capacity_ = count;
            reallocate();
        }
    }
    else
    {for (auto i = size_; i < count; ++i) elem[i].~T();}
    size_ = count;
}

template<class T>
void Vector<T>::resize( size_t count, const T& value )
{
    if (count > size_)
    {
        if (count > capacity_)
        {
            capacity_ = count;
            reallocate();
        }
        for (auto i = size_; i < count; ++i) elem[i] = value;
    }
    else {for (auto i = size_; i < count; ++i) elem[i].~T();}
    size_ = count;
}

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

// Other
template<typename T>
void Vector<T>::reallocate()
{
    T* elem2=new T[capacity_];
    std::copy(elem,elem+size_,elem2);
    delete[] elem;
    elem=elem2;
}

template<typename T>
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

template<typename T>
void swap(T& x, T& y) 
{
    T temp {std::move(x)};
    x = std::move(y);
    y = std::move(temp);
}
#endif
