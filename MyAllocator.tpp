template <typename T, size_t N>
MyAllocator<T, N>::MyAllocator() noexcept 
    : pool_(nullptr), free_list_head_(nullptr), pool_size_bytes_(N * sizeof(T)) {
    if (pool_size_bytes_ > 0) {
        pool_ = static_cast<char*>(::operator new(pool_size_bytes_));
        free_list_head_ = pool_;
        
        Slot* current = reinterpret_cast<Slot*>(pool_);
        for (size_t i = 0; i < N - 1; ++i) {
            Slot* next = reinterpret_cast<Slot*>(reinterpret_cast<char*>(current) + sizeof(T));
            current->next = next;
            current = next;
        }
        current->next = nullptr;
    }
}

template <typename T, size_t N>
MyAllocator<T, N>::~MyAllocator() noexcept {
    ::operator delete(pool_);
}

template <typename T, size_t N>
template <class U>
MyAllocator<T, N>::MyAllocator(const MyAllocator<U, N>& other) noexcept: MyAllocator() {}

template <typename T, size_t N>
bool MyAllocator<T, N>::isInPool(T* p) const {
    return pool_ && (char*)p >= pool_ && (char*)p < (pool_ + pool_size_bytes_);
}

template <typename T, size_t N>
T* MyAllocator<T, N>::allocate(std::size_t n) {
    if (n == 1 && free_list_head_ != nullptr) {
        T* p = static_cast<T*>(free_list_head_);
        free_list_head_ = static_cast<Slot*>(free_list_head_)->next;
        return p;
    }
    return static_cast<T*>(::operator new(n * sizeof(T)));
}

template <typename T, size_t N>
void MyAllocator<T, N>::deallocate(T* p, std::size_t n) {
    if (n == 1 && isInPool(p)) {
        Slot* slot = static_cast<Slot*>(static_cast<void*>(p));
        slot->next = static_cast<Slot*>(free_list_head_);
        free_list_head_ = slot;
    }
    else {
        ::operator delete(p);
    }
}

template <class T, size_t N1, class U, size_t N2>
bool operator==(const MyAllocator<T, N1>& lhs, const MyAllocator<U, N2>& rhs) noexcept {
    return false;
}

template <class T, size_t N1, class U, size_t N2>
bool operator!=(const MyAllocator<T, N1>& lhs, const MyAllocator<U, N2>& rhs) noexcept {
    return true;
}