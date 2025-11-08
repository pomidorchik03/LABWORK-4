template <typename T, size_t N>
MyAllocator<T, N>::MyAllocator() noexcept 
    : pool_(nullptr), free_list_head_(nullptr), pool_size_bytes_(N * BLOCK_SIZE) {
    
    if (N > 0) {
        pool_ = static_cast<char*>(::operator new(pool_size_bytes_));
        
        free_list_head_ = reinterpret_cast<Slot*>(pool_);
        Slot* current = free_list_head_;
        
        for (size_t i = 0; i < N - 1; ++i) {
            Slot* next = reinterpret_cast<Slot*>(reinterpret_cast<char*>(current) + BLOCK_SIZE);
            current->next = next;
            current = next;
        }
        current->next = nullptr;
    }
}

template <typename T, size_t N>
MyAllocator<T, N>::MyAllocator(MyAllocator&& other) noexcept
    : pool_(other.pool_), 
      free_list_head_(other.free_list_head_), 
      pool_size_bytes_(other.pool_size_bytes_) {
    
    other.pool_ = nullptr;
    other.free_list_head_ = nullptr;
    other.pool_size_bytes_ = 0;
}

template <typename T, size_t N>
MyAllocator<T, N>& MyAllocator<T, N>::operator=(MyAllocator&& other) noexcept {
    if (this != &other) {
        ::operator delete(pool_);

        pool_ = other.pool_;
        free_list_head_ = other.free_list_head_;
        pool_size_bytes_ = other.pool_size_bytes_;
        
        other.pool_ = nullptr;
        other.free_list_head_ = nullptr;
        other.pool_size_bytes_ = 0;
    }
    return *this;
}

template <typename T, size_t N>
MyAllocator<T, N>::~MyAllocator() noexcept {
    if (pool_ != nullptr) { 
        ::operator delete(pool_);
    }
}

template <typename T, size_t N>
template <class U>
MyAllocator<T, N>::MyAllocator(const MyAllocator<U, N>&) noexcept 
    : pool_(nullptr), free_list_head_(nullptr), pool_size_bytes_(0) {} 

template <typename T, size_t N>
bool MyAllocator<T, N>::isInPool(T* p) const {
    return pool_ && (reinterpret_cast<char*>(p) >= pool_) && 
           (reinterpret_cast<char*>(p) < (pool_ + pool_size_bytes_));
}

template <typename T, size_t N>
T* MyAllocator<T, N>::allocate(std::size_t n) {
    if (n == 1 && free_list_head_ != nullptr) {
        T* p = reinterpret_cast<T*>(free_list_head_);
        free_list_head_ = free_list_head_->next;
        return p;
    }
    return static_cast<T*>(::operator new(n * sizeof(T)));
}

template <typename T, size_t N>
void MyAllocator<T, N>::deallocate(T* p, std::size_t n) {
    if (n == 1 && isInPool(p)) {
        Slot* slot = reinterpret_cast<Slot*>(p);
        
        slot->next = free_list_head_;
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