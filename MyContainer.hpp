#pragma once

#include <memory>
#include <iterator>
#include <cstddef>
#include "MyAllocator.hpp" 

template <typename T, typename Allocator = std::allocator<T>>
class MyContainer {
private:
    struct Node {
        T value_;
        Node* next_;
        Node(const T& val) : value_(val), next_(nullptr) {}
    };

    using NodeAllocator = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;

    NodeAllocator nodeAlloc_;
    Node* head_;
    Node* tail_;
    size_t size_;

public:
    class iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        iterator(Node* ptr) : _nodePtr(ptr) {}

        reference operator*() const { return _nodePtr->value_; }
        pointer operator->() const { return &(_nodePtr->value_); }

        iterator& operator++() {
            _nodePtr = _nodePtr->next_;
            return *this;
        }
        
        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool operator==(const iterator& a, const iterator& b) {
            return a._nodePtr == b._nodePtr;
        }
        friend bool operator!=(const iterator& a, const iterator& b) {
            return a._nodePtr != b._nodePtr;
        }

    private:
        Node* _nodePtr;
    };

    explicit MyContainer(const Allocator& alloc = Allocator());
    ~MyContainer();

    void push_back(const T& value);

    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    iterator begin() { return iterator(head_); }
    iterator end() { return iterator(nullptr); }
};

#include "MyContainer.tpp"