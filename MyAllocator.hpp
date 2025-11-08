#pragma once

#include <cstddef> 
#include <new> 
#include <limits> 
#include <iostream>
#include <algorithm> 

template <typename T, size_t N>
class MyAllocator {
private:
    struct Slot {
        char element[sizeof(T)]; 
        Slot* next; 
    };

    size_t BLOCK_SIZE = sizeof(T) > sizeof(Slot*) ? sizeof(T) : sizeof(Slot*);

    char* pool_;
    Slot* free_list_head_; 
    size_t pool_size_bytes_;

    bool isInPool(T* p) const;

public:
    using value_type = T;

    template <class U>
    struct rebind {
        using other = MyAllocator<U, N>;
    };

    MyAllocator() noexcept;
    ~MyAllocator() noexcept;

    MyAllocator(const MyAllocator&) = delete;
    MyAllocator& operator=(const MyAllocator&) = delete;

    MyAllocator(MyAllocator&& other) noexcept;
    MyAllocator& operator=(MyAllocator&& other) noexcept;

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