#pragma once

#include <cstddef> 
#include <new>     
#include <limits>  

template <typename T, size_t N>
class MyAllocator {
private:
    char* pool_;
    void* free_list_head_;
    size_t pool_size_bytes_;

    struct Slot {
        char element[sizeof(T)];
        Slot* next;
    };

    bool isInPool(T* p) const;

public:
    using value_type = T;

    template <class U>
    struct rebind {
        using other = MyAllocator<U, N>;
    };

    MyAllocator() noexcept;
    ~MyAllocator() noexcept;

    template <class U>
    MyAllocator(const MyAllocator<U, N>& other) noexcept;

    T* allocate(std::size_t n);
    void deallocate(T* p, std::size_t n);
};

template <class T, size_t N1, class U, size_t N2>
bool operator==(const MyAllocator<T, N1>& lhs, const MyAllocator<U, N2>& rhs) noexcept;

template <class T, size_t N1, class U, size_t N2>
bool operator!=(const MyAllocator<T, N1>& lhs, const MyAllocator<U, N2>& rhs) noexcept;

#include "MyAllocator.tpp"