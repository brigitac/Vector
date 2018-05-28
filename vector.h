#ifndef VECTOR_H
#define VECTOR_H

#include <chrono>
#include <exception>
#include <algorithm>
#include <cstring>
#include <iterator>
#include <memory>
#include "timer.h"

template<class T>
class Vector
{
public:

    typedef T value_type;
    typedef T & reference;
    typedef const T & const_reference;
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef size_t size_type;
    typedef std::allocator<T> allocator_type;

    // Member functions
    Vector();
// Vector() noexcept(noexcept(std::allocator<T>()));
// explicit Vector(const std::allocator<T>& alloc) noexcept;
    Vector(size_type count, const_reference val);
// vector( size_t count, const T& value, const Allocator& alloc = Allocator());
    explicit Vector(size_type count);
// explicit vector(size_t count, const Allocator& alloc = Allocator() );
    template< class InputIt >
    Vector(InputIt first, InputIt last);
// template< class InputIt > vector( InputIt first, InputIt last, const Allocator& alloc = Allocator() );
    Vector(const Vector& other);
//vector(const Vector& other, const Allocator& alloc );
    Vector(Vector&& other) noexcept;
// vector(Vector&& other, const Allocator& alloc );
    Vector(std::initializer_list<T> init);
// vector(std::initializer_list<T> init, const Allocator& alloc = Allocator() );
    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other) noexcept;
    Vector& operator=( std::initializer_list<T> ilist );
    void assign(size_type count, const_reference value);
    void assign(iterator first, iterator last);
    void assign(std::initializer_list<T> ilist);
    allocator_type get_allocator() const;
    ~Vector();

    // Element Access
    reference at(size_type pos);
    const_reference at(size_type pos) const;
    reference operator[](size_type i);
    const_reference operator[](size_type i) const;
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;
    iterator data() noexcept;
    const_iterator data() const noexcept;
    // Iterators
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;
    reverse_iterator  rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crend() const noexcept;

    // Capacity
    bool empty() const noexcept;
    size_type size() const noexcept;
    size_type max_size() const noexcept;
    void reserve(size_type new_cap);
    size_type capacity() const noexcept;
    void shrink_to_fit();

    // Modifiers
    void clear() noexcept;
    iterator insert(const_iterator pos, const_reference value );
    iterator insert(const_iterator pos, T&& value );
    iterator insert(const_iterator pos, size_type count, const_reference value);
    iterator insert(const_iterator pos, T* first, T* last );
    iterator insert(const_iterator pos, std::initializer_list<T> ilist);
    template <class Args> T* emplace(const_iterator, Args &&);
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    void push_back(const_reference value);
    void push_back(T&& value);
    template <class Args> void emplace_back(Args && args); //dar yra kazkoks since 17
    void pop_back();
    void resize(size_type count);
    void resize(size_type count, const_reference value );
    void swap(Vector& other);

private:
    std::allocator<T> allocator;
    size_t size_;
    size_t capacity_;
    T* elem;
};

// Member functions
template<class T>
Vector<T>::Vector() : size_{0}, capacity_{0}, elem{allocator.allocate(capacity_)} {}

// template<class T>
// Vector<T>::Vector() noexcept(noexcept(std::allocator<T>())) : size_{0}, capacity_{0}, elem{allocator.allocate(capacity_)} {}

// template<class T>
// Vector<T>::Vector(const std::allocator<T>& alloc) noexcept : size_{0}, capacity_{0}
// {
//     allocator=alloc;
//     elem=allocator.allocate(capacity_); 
// } 

template <typename T>
Vector<T>::Vector(size_type count, const_reference value) : size_{count}, capacity_{count}, elem{allocator.allocate(capacity_)} 
{std::fill_n(elem,count,value);}

template <typename T>
Vector<T>::Vector(size_type count) : size_{count}, capacity_{count}, elem{allocator.allocate(capacity_)} 
{std::fill_n(elem,count,T());}

template <typename T>
template< class InputIt >
Vector<T>::Vector(InputIt first, InputIt last) 
{
    auto count = last - first;
    capacity_ = count;
    elem=allocator.allocate(capacity_);
    for (auto i = 0; i < count; ++i, ++first) elem[i] = *first;
    size_ = count;
}

template<class T>
Vector<T>::Vector(const Vector& other) : elem{allocator.allocate(other.size_)}, size_{other.size_}, capacity_{other.capacity_}
{for (int i=0; i!=size_; ++i) elem[i] = other.elem[i];}

template <typename T>
Vector<T>::Vector(Vector&& other) noexcept : elem{other.elem}, size_{other.size_}, capacity_{other.capacity_} 
{
    other.elem = nullptr;  
    other.size_ = 0;   
    other.capacity_=0;
}

template<class T>
Vector<T>::Vector(std::initializer_list<T>init) : size_{static_cast<size_t>(init.size())}, elem{allocator.allocate(init.size())}, capacity_{static_cast<size_t>(init.size())}
{std::copy(init.begin(),init.end(),elem);}

template<class T>
Vector<T>& Vector<T>::operator=(const Vector& other)
{
    if (&other == this) return *this;
    if (capacity_<other.size_) capacity_=other.size_;
    T* p = allocator.allocate(other.size_);
    for (int i=0; i!=other.size_; ++i) p[i] = other.elem[i];
    for (size_t i = 0; i != size_; i++) {allocator.destroy(elem + i);}
    allocator.deallocate(elem, capacity_);
    elem = p;
    size_ = other.size_;
    capacity_=other.capacity_;
    return *this;
}

template<class T>
Vector<T>& Vector<T>::operator=(Vector&& other) noexcept
{
    if (&other == this) return *this;
    for (size_t i = 0; i != size_; i++) {allocator.destroy(elem + i);}
    allocator.deallocate(elem, capacity_);
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
    T* p = allocator.allocate(ilist.size());
    auto i=0;
    for (auto &item:ilist)  p[i++] = item;
    size_=ilist.size();
    for (i = 0; i != size_; i++) {allocator.destroy(elem + i);}
    allocator.deallocate(elem, capacity_);
    elem = p;
    return *this;
}

template <typename T>
void Vector<T>::assign(size_type count, const_reference value)
{
    for (size_t i = 0; i != size_; i++) {allocator.destroy(elem + i);}
    allocator.deallocate(elem, capacity_);
    if (count > capacity_)
    {capacity_ = count;}
    elem = allocator.allocate(capacity_);
    std::fill_n(elem,count,value);
    size_ = count;
}

template <typename T>
void Vector<T>::assign(iterator first, iterator last)
{
    for (size_t i = 0; i != size_; i++) {allocator.destroy(elem + i);}
    allocator.deallocate(elem, capacity_);
    auto count = last - first;
    if (count > capacity_)
    {capacity_ = count;}
    elem = allocator.allocate(capacity_);
    for (auto i = 0; i < count; ++i, ++first) elem[i] = *first;
    size_ = count;
}

template <typename T>
void Vector<T>::assign(std::initializer_list<T> ilist)
{
    for (size_t i = 0; i != size_; i++) {allocator.destroy(elem + i);}
    allocator.deallocate(elem, capacity_);
    if (ilist.size() > capacity_)
    {capacity_ = ilist.size();}
    size_=ilist.size();
    auto i = 0;
    elem = allocator.allocate(capacity_);
    for (auto &item: ilist) elem[i++] = item;
}

template<class T>
typename Vector<T>::allocator_type Vector<T>::get_allocator() const
{return allocator;}

template<class T>
Vector<T>::~Vector() 
{
    for (size_t i = 0; i != size_; i++) {allocator.destroy(elem + i);}
    allocator.deallocate(elem, capacity_);
    size_=0;
    capacity_=0;
}

// Element Access
template<class T>
typename Vector<T>::reference Vector<T>::at(size_type pos)
{
    if (pos < 0 || size() <= pos) throw std::out_of_range{"vector"};;
    return elem[pos];
}

template<class T>
typename Vector<T>::const_reference Vector<T>::at(size_type pos) const
{
    if (pos < 0 || size() <= pos) throw std::out_of_range{"vector"};;
    return elem[pos];
}

template<class T>
typename Vector<T>::reference Vector<T>::operator[](size_type i)
{return elem[i];}

template<class T>
typename Vector<T>::const_reference Vector<T>::operator[](size_type i) const
{return elem[i];}

template<class T>
typename Vector<T>::reference Vector<T>::front()
{return elem[0];}

template<class T>
typename Vector<T>::const_reference Vector<T>::front() const
{return elem[0];}

template<class T>
typename Vector<T>::reference Vector<T>::back()
{return elem[size_ - 1];}

template<class T>
typename Vector<T>::const_reference Vector<T>::back() const
{return elem[size_ - 1];}

template<typename T>
typename Vector<T>::iterator Vector<T>::data() noexcept
{return elem;}

template<typename T>
typename Vector<T>::const_iterator Vector<T>::data() const noexcept
{return elem;}

// Iterators
template<class T>
typename Vector<T>::iterator Vector<T>::begin() noexcept
{return elem;}

template<class T>
typename Vector<T>::const_iterator Vector<T>::begin() const noexcept
{return elem;}

template<class T>
typename Vector<T>::const_iterator Vector<T>::cbegin() const noexcept
{return elem;}

template<class T>
typename Vector<T>::iterator Vector<T>::end() noexcept
{return elem + size();}

template<class T>
typename Vector<T>::const_iterator Vector<T>::end() const noexcept
{return elem + size();}

template<class T>
typename Vector<T>::const_iterator Vector<T>::cend() const noexcept
{return elem + size();}

template<class T>
typename Vector<T>::reverse_iterator Vector<T>:: rbegin() noexcept
{return std::reverse_iterator<T*>(elem + size_);}

template<class T>
typename Vector<T>::const_reverse_iterator Vector<T>:: rbegin() const noexcept
{return std::reverse_iterator<T*>(elem + size_);}

template<class T>
typename Vector<T>::const_reverse_iterator Vector<T>:: crbegin() const noexcept
{return std::reverse_iterator<T*>(elem + size_);}

template <typename T>
typename Vector<T>::reverse_iterator  Vector<T>::rend() noexcept
{return std::reverse_iterator<T*>(elem);}

template <typename T>
typename Vector<T>::const_reverse_iterator Vector<T>::rend() const noexcept
{return std::reverse_iterator<T*>(elem);}

template <typename T>
typename Vector<T>::const_reverse_iterator Vector<T>::crend() const noexcept
{return std::reverse_iterator<T*>(elem);}

// Capacity
template<class T>
bool Vector<T>::empty() const noexcept
{return begin() == end();}

template<class T>
typename Vector<T>::size_type Vector<T>::size() const noexcept
{return size_;}

template <typename T>
typename Vector<T>::size_type Vector<T>::max_size() const noexcept
{return allocator.max_size();}

template<class T>
void Vector<T>::reserve(size_type new_cap)
{
    if (new_cap>size_)
    {
        T* elem2=allocator.allocate(new_cap);
        std::copy(elem,elem+size_,elem2);
        for (size_t i = 0; i != size_; i++) {allocator.destroy(elem + i);}
        allocator.deallocate(elem, capacity_);
        elem=elem2;
        capacity_ = new_cap;
        elem2 = nullptr;
    }
}

template<class T>
typename Vector<T>::size_type Vector<T>::capacity() const noexcept
{return capacity_;}

template <typename T>
void Vector<T>::shrink_to_fit()
{
    T* elem2=allocator.allocate(size_);
    std::copy(elem,elem+size_,elem2);
    for (size_t i = 0; i != size_; i++) {allocator.destroy(elem + i);}
    allocator.deallocate(elem, capacity_);
    elem=elem2;
    elem2 = nullptr;
    capacity_ = size_;
}

// Modifiers
template <typename T>
void Vector<T>::clear() noexcept
{
    for (size_t i = 0; i != size_; i++) {allocator.destroy(elem + i);}
    size_ = 0;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::insert(const_iterator pos, const_reference value)
{
    T* pos2 = &elem[pos - elem];
    if (size_==capacity_)
    {
        capacity_=capacity_*2;
        reserve(capacity_);
    }
    std::copy(pos2-1, pos2+(size_-(pos-elem)), pos2);
    (*pos2) = value;
    ++size_;
    return pos2;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::insert(const_iterator pos, T&& value )
{
    T* pos2 = &elem[pos - elem];
    if (size_==capacity_)
    {
        capacity_=capacity_*2;
        reserve(capacity_);
    }
    std::copy(pos2-1, pos2+(size_-(pos-elem)), pos2);
    (*pos2) = std::move(value);
    ++size_;
    return pos2;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::insert(const_iterator pos, size_type count,const_reference value)
{
    T* pos2 = &elem[pos - elem];
    if (count==0) return pos2;
    if (size_ + count > capacity_)
    {
        capacity_ = (size_ + count)* 2;
        reserve(capacity_);
    }
    std::copy(pos2-count, pos2+(size_-(pos-elem)), pos2);
    size_ += count;
    for (T* it = pos2; count--; ++it) (*it) = value;
    return pos2;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::insert(const_iterator pos, iterator first, iterator last)
{
    T* pos2 = &elem[pos - elem];
    size_t count = last - first;
    if (count==0) return pos2;
    if (size_ + count > capacity_)
    {
        capacity_ = (size_ + count) * 2;
        reserve(capacity_);
    }
    std::copy(pos2-count, pos2+(size_-(pos-elem)), pos2);
    for (T* it = pos2; first != last; ++pos, ++first) (*pos) = *first;
    size_ += count;
    return pos2;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::insert(const_iterator pos, std::initializer_list<T> ilist)
{
    T* pos2 = &elem[pos - elem];
    size_t count = ilist.size();
    if (count==0) return pos2;
    if (size_ + count > capacity_)
    {
        capacity_ = (size_ + count) *2;
        reserve(capacity_);
    }
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
template <class Args>
typename Vector<T>::iterator Vector<T>::emplace(const_iterator  pos, Args && args)
{
    T* pos2 = &elem[pos - elem];
    if (size_ == capacity_) 
    {
        capacity_=capacity_ *2;
        reserve(capacity_);
    }
    std::copy(pos2-1, pos2+(size_-(pos-elem)), pos2);
    allocator.construct(pos2,args);
    ++size_;
    return pos2;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::erase(const_iterator pos)
{
    T* pos2 = &elem[pos - elem];
    allocator.destroy(elem + (pos-elem));
    std::copy(pos2+1, pos2+(size_-(pos-elem)), pos2);
    --size_;
    return pos2;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::erase(const_iterator first, const_iterator last)
{ 
    T* pos = &elem[first - elem];
    if (first == last) return pos;
    for (;first != last; ++first) allocator.destroy(elem + (first-elem));
    std::copy(last, last+(size_-(last-elem)), pos);
    size_ -= last - first;
    return pos;
}

template<class T>
void Vector<T>::push_back(const_reference value)
{
    if (capacity_==0)
    {reserve(1);}
    if (size_==capacity_)
    {reserve(2*capacity_);}
    size_++;
    elem[size_-1]=value;
}

template<class T>
void Vector<T>::push_back(T &&value)
{
    if (capacity_==0)
    {reserve(1);}
    if (size_==capacity_)
    {reserve(2*capacity_);}
    size_++;
    elem[size_-1]=std::move(value);
}

template <typename T>
template <class Args>
void Vector<T>::emplace_back(Args && args)
{
    allocator.construct(elem+size_,args);
    ++size_;
}

template<class T>
void Vector<T>::pop_back()
{
    allocator.destroy(elem + size_);
    size_--;
}

template<class T>
void Vector<T>::resize(size_type count)
{
    if (count > size_)
    {
        if (count > capacity_)
        {
            T* elem2=allocator.allocate(count);
            std::copy(elem,elem+size_,elem2);
            for (size_t i = 0; i != size_; i++) {allocator.destroy(elem + i);}
            allocator.deallocate(elem, capacity_);
            elem=elem2;
            elem2 = nullptr;
            capacity_ = count;
        }
    }
    else {for (size_t i = size_; i<count; i++) allocator.destroy(elem + i);}
    size_ = count;
}

template<class T>
void Vector<T>::resize(size_type count, const_reference value )
{
    if (count > size_)
    {
        if (count > capacity_)
        {
            T* elem2=allocator.allocate(count);
            std::copy(elem,elem+size_,elem2);
            for (size_t i = 0; i != size_; i++) {allocator.destroy(elem + i);}
            allocator.deallocate(elem, capacity_);
            elem=elem2;
            elem2 = nullptr;
            capacity_ = count;
        }
        for (auto i = size_; i < count; ++i) elem[i] = value;
    }
    else {for (size_t i = size_; i<count; i++) allocator.destroy(elem + i);}
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
Vector<T> operator+(const Vector<T>& a, const Vector<T>& b)
{
    if (a.size() != b.size())
        throw std::runtime_error("Vektorių dydžio neatitikimas!");
    auto size = a.size();
    Vector<T> c(size);
    for (auto i = 0; i != a.size(); ++i) c[i] = a[i] + b[i];
    return c;
}

// template<typename T>
// void swap(T& x, T& y) 
// {
//     T temp {std::move(x)};
//     x = std::move(y);
//     y = std::move(temp);
// }
#endif