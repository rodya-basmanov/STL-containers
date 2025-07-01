#ifndef SKIP_LIST_H
#define SKIP_LIST_H

#include <algorithm>
#include <random>
#include <memory>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <type_traits>
#include <initializer_list>

namespace stl_containers {

template <typename T, typename Compare = std::less<T>, typename Allocator = std::allocator<T>>
class SkipList {
private:
    static constexpr size_t MAX_LEVEL = 6;
    static constexpr float P = 0.5;

    struct Node {
        T value;
        std::vector<Node*> forward;

        Node(const T& val, size_t level) 
            : value(val), forward(level, nullptr) {}
        
        Node(T&& val, size_t level) 
            : value(std::move(val)), forward(level, nullptr) {}
    };
    
    using NodeAllocator = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
    using NodeAllocatorTraits = std::allocator_traits<NodeAllocator>;

    NodeAllocator node_allocator_;
    Compare compare_;
    
    Node* head_;
    size_t max_level_;
    size_t size_;
    
    std::mt19937 generator_;
    std::uniform_real_distribution<float> distribution_;

    size_t randomLevel() {
        size_t level = 1;
        while (distribution_(generator_) < P && level < MAX_LEVEL) {
            level++;
        }
        return level;
    }

    Node* createNode(const T& value, size_t level) {
        Node* node = NodeAllocatorTraits::allocate(node_allocator_, 1);
        NodeAllocatorTraits::construct(node_allocator_, node, value, level);
        return node;
    }

    Node* createNode(T&& value, size_t level) {
        Node* node = NodeAllocatorTraits::allocate(node_allocator_, 1);
        NodeAllocatorTraits::construct(node_allocator_, node, std::move(value), level);
        return node;
    }

    void destroyNode(Node* node) {
        NodeAllocatorTraits::destroy(node_allocator_, node);
        NodeAllocatorTraits::deallocate(node_allocator_, node, 1);
    }

public:
    template <bool IsConst>
    class Iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using reference = typename std::conditional<IsConst, const T&, T&>::type;
        using pointer = typename std::conditional<IsConst, const T*, T*>::type;
        
    private:
        using NodePtr = typename std::conditional<IsConst, const Node*, Node*>::type;
        NodePtr current_;
        
    public:
        Iterator() noexcept : current_(nullptr) {}
        explicit Iterator(NodePtr node) noexcept : current_(node) {}
        
        template <bool OtherIsConst, typename = typename std::enable_if<IsConst && !OtherIsConst>::type>
        Iterator(const Iterator<OtherIsConst>& other) noexcept : current_(other.current_) {}
        
        reference operator*() const {
            return current_->value;
        }
        
        pointer operator->() const {
            return &(current_->value);
        }
        
        Iterator& operator++() {
            current_ = current_->forward[0];
            return *this;
        }
        
        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        
        bool operator==(const Iterator& other) const {
            return current_ == other.current_;
        }
        
        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }
        
        friend class SkipList;
    };

    using value_type = T;
    using allocator_type = Allocator;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
    using iterator = Iterator<false>;
    using const_iterator = Iterator<true>;

    SkipList() 
        : node_allocator_(), 
          compare_(),
          max_level_(1), 
          size_(0), 
          generator_(std::random_device{}()),
          distribution_(0.0, 1.0) {
        head_ = createNode(T(), MAX_LEVEL);
    }

    SkipList(std::initializer_list<T> init)
        : node_allocator_(), 
          compare_(),
          max_level_(1), 
          size_(0), 
          generator_(std::random_device{}()),
          distribution_(0.0, 1.0) {
        head_ = createNode(T(), MAX_LEVEL);
        
        for (const auto& value : init) {
            insert(value);
        }
    }

    SkipList(const SkipList& other)
        : node_allocator_(NodeAllocatorTraits::select_on_container_copy_construction(other.node_allocator_)),
          compare_(other.compare_),
          max_level_(1),
          size_(0),
          generator_(std::random_device{}()),
          distribution_(0.0, 1.0) {
        head_ = createNode(T(), MAX_LEVEL);
        
        for (auto it = other.begin(); it != other.end(); ++it) {
            insert(*it);
        }
    }

    SkipList(SkipList&& other) noexcept
        : node_allocator_(std::move(other.node_allocator_)),
          compare_(std::move(other.compare_)),
          head_(other.head_),
          max_level_(other.max_level_),
          size_(other.size_),
          generator_(std::move(other.generator_)),
          distribution_(std::move(other.distribution_)) {
        other.head_ = nullptr;
        other.size_ = 0;
        other.max_level_ = 1;
    }

    ~SkipList() {
        clear();
        if (head_) destroyNode(head_);
    }

    SkipList& operator=(const SkipList& other) {
        if (this != &other) {
            SkipList tmp(other);
            swap(tmp);
        }
        return *this;
    }

    SkipList& operator=(SkipList&& other) noexcept {
        if (this != &other) {
            clear();
            
            if (NodeAllocatorTraits::propagate_on_container_move_assignment::value) {
                node_allocator_ = std::move(other.node_allocator_);
            }
            
            compare_ = std::move(other.compare_);
            
            destroyNode(head_);
            
            head_ = other.head_;
            max_level_ = other.max_level_;
            size_ = other.size_;
            
            other.head_ = nullptr;
            other.size_ = 0;
            other.max_level_ = 1;
            
            generator_ = std::move(other.generator_);
            distribution_ = std::move(other.distribution_);
        }
        return *this;
    }

    reference front() {
        if (empty()) throw std::out_of_range("SkipList::front called on empty container");
        return head_->forward[0]->value;
    }

    const_reference front() const {
        if (empty()) throw std::out_of_range("SkipList::front called on empty container");
        return head_->forward[0]->value;
    }

    iterator begin() noexcept {
        return iterator(head_->forward[0]);
    }

    const_iterator begin() const noexcept {
        return const_iterator(head_->forward[0]);
    }

    const_iterator cbegin() const noexcept {
        return const_iterator(head_->forward[0]);
    }

    iterator end() noexcept {
        return iterator(nullptr);
    }

    const_iterator end() const noexcept {
        return const_iterator(nullptr);
    }

    const_iterator cend() const noexcept {
        return const_iterator(nullptr);
    }

    bool empty() const noexcept {
        return size_ == 0;
    }

    size_type size() const noexcept {
        return size_;
    }

    void clear() noexcept {
        Node* current = head_->forward[0];
        
        while (current != nullptr) {
            Node* next = current->forward[0];
            destroyNode(current);
            current = next;
        }

        for (size_t i = 0; i < MAX_LEVEL; ++i) {
            head_->forward[i] = nullptr;
        }

        max_level_ = 1;
        size_ = 0;
    }

    std::pair<iterator, bool> insert(const T& value) {
        std::vector<Node*> update(MAX_LEVEL, nullptr);
        
        Node* current = head_;
        
        for (int i = static_cast<int>(max_level_) - 1; i >= 0; --i) {
            while (current->forward[i] != nullptr && 
                   compare_(current->forward[i]->value, value)) {
                current = current->forward[i];
            }
            update[i] = current;
        }
        
        Node* next = current->forward[0];
        
        if (next != nullptr && !compare_(value, next->value) && !compare_(next->value, value)) {
            return {iterator(next), false};
        }
        
        size_t new_level = randomLevel();
        if (new_level > max_level_) {
            for (size_t i = max_level_; i < new_level; ++i) {
                update[i] = head_;
            }
            max_level_ = new_level;
        }
        
        Node* new_node = createNode(value, new_level);
        
        for (size_t i = 0; i < new_level; ++i) {
            new_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = new_node;
        }
        
        ++size_;
        return {iterator(new_node), true};
    }

    std::pair<iterator, bool> insert(T&& value) {
        std::vector<Node*> update(MAX_LEVEL, nullptr);
        
        Node* current = head_;
        
        for (int i = static_cast<int>(max_level_) - 1; i >= 0; --i) {
            while (current->forward[i] != nullptr && 
                   compare_(current->forward[i]->value, value)) {
                current = current->forward[i];
            }
            update[i] = current;
        }
        
        Node* next = current->forward[0];
        
        if (next != nullptr && !compare_(value, next->value) && !compare_(next->value, value)) {
            return {iterator(next), false};
        }
        
        size_t new_level = randomLevel();
        if (new_level > max_level_) {
            for (size_t i = max_level_; i < new_level; ++i) {
                update[i] = head_;
            }
            max_level_ = new_level;
        }
        
        Node* new_node = createNode(std::move(value), new_level);
        
        for (size_t i = 0; i < new_level; ++i) {
            new_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = new_node;
        }
        
        ++size_;
        return {iterator(new_node), true};
    }

    template <typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args) {
        return insert(T(std::forward<Args>(args)...));
    }

    size_t erase(const_iterator pos) {
        if (pos == cend()) {
            return 0;
        }
        
        return erase(*pos);
    }

    size_t erase(const T& value) {
        std::vector<Node*> update(MAX_LEVEL, nullptr);
        
        Node* current = head_;
        
        for (int i = static_cast<int>(max_level_) - 1; i >= 0; --i) {
            while (current->forward[i] != nullptr && 
                   compare_(current->forward[i]->value, value)) {
                current = current->forward[i];
            }
            update[i] = current;
        }
        
        Node* target = current->forward[0];
        
        if (target != nullptr && !compare_(value, target->value) && !compare_(target->value, value)) {
            for (size_t i = 0; i < max_level_; ++i) {
                if (update[i]->forward[i] != target) {
                    break;
                }
                update[i]->forward[i] = target->forward[i];
            }
            
            destroyNode(target);
            --size_;
            
            while (max_level_ > 1 && head_->forward[max_level_ - 1] == nullptr) {
                --max_level_;
            }
            
            return 1;
        }
        
        return 0;
    }

    void swap(SkipList& other) noexcept {
        if (this != &other) {
            using std::swap;
            
            swap(node_allocator_, other.node_allocator_);
            swap(compare_, other.compare_);
            swap(head_, other.head_);
            swap(max_level_, other.max_level_);
            swap(size_, other.size_);
            swap(generator_, other.generator_);
            swap(distribution_, other.distribution_);
        }
    }

    iterator find(const T& value) {
        Node* current = head_;
        
        for (int i = static_cast<int>(max_level_) - 1; i >= 0; --i) {
            while (current->forward[i] != nullptr && 
                   compare_(current->forward[i]->value, value)) {
                current = current->forward[i];
            }
        }
        
        Node* result = current->forward[0];
        
        if (result != nullptr && !compare_(value, result->value) && !compare_(result->value, value)) {
            return iterator(result);
        }
        
        return end();
    }

    const_iterator find(const T& value) const {
        const Node* current = head_;
        
        for (int i = static_cast<int>(max_level_) - 1; i >= 0; --i) {
            while (current->forward[i] != nullptr && 
                   compare_(current->forward[i]->value, value)) {
                current = current->forward[i];
            }
        }
        
        const Node* result = current->forward[0];
        
        if (result != nullptr && !compare_(value, result->value) && !compare_(result->value, value)) {
            return const_iterator(result);
        }
        
        return cend();
    }

    bool contains(const T& value) const {
        return find(value) != cend();
    }

    friend bool operator==(const SkipList& lhs, const SkipList& rhs) {
        if (lhs.size_ != rhs.size_) {
            return false;
        }
        
        return std::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    friend bool operator!=(const SkipList& lhs, const SkipList& rhs) {
        return !(lhs == rhs);
    }
};

template <typename T, typename Compare, typename Alloc>
void swap(SkipList<T, Compare, Alloc>& lhs, SkipList<T, Compare, Alloc>& rhs) noexcept {
    lhs.swap(rhs);
}

} // namespace stl_containers

#endif // SKIP_LIST_H 