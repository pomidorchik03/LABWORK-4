template <typename T, typename Allocator>
MyContainer<T, Allocator>::MyContainer(const Allocator& alloc)
    : nodeAlloc_(alloc), head_(nullptr), tail_(nullptr), size_(0) {}

template <typename T, typename Allocator>
MyContainer<T, Allocator>::~MyContainer() {
    Node* current = head_;
    while (current != nullptr) {
        Node* next = current->next_;
        
        std::allocator_traits<NodeAllocator>::destroy(nodeAlloc_, current);
        nodeAlloc_.deallocate(current, 1);
        
        current = next;
    }
}

template <typename T, typename Allocator>
void MyContainer<T, Allocator>::push_back(const T& value) {
    Node* new_node = nodeAlloc_.allocate(1);
    std::allocator_traits<NodeAllocator>::construct(nodeAlloc_, new_node, value);

    if (head_ == nullptr) {
        head_ = new_node;
        tail_ = new_node;
    } else {
        tail_->next_ = new_node;
        tail_ = new_node;
    }
    size_++;
}