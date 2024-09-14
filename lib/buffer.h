#include <iostream>

template<typename T, typename Allocator = std::allocator<T>>
class CCircularBuffer {
public:
    class Iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using type_value = T;
        using difference_type = T;
        using pointer = T*;
        using reference = T&;
        T* data_pointer = nullptr;
        size_t head = 0;
        size_t iter = 0;
        size_t block_size = 0;

        Iterator() :
                data_pointer(nullptr), iter(0), block_size(0), head(0) {}

        Iterator(const Iterator& other) :
                data_pointer(other.data_pointer), iter(other.iter), block_size(other.block_size),
                head(other.head) {}

        Iterator& operator=(const Iterator& other) {
            if (this != &other) {
                data_pointer = other.data_pointer;
                iter = other.iter;
                block_size = other.block_size;
                head = other.head;
            }
            return *this;
        }

        Iterator(T* pointer, size_t it, size_t size, size_t head_) {
            data_pointer = pointer;
            iter = it;
            head = head_;
            block_size = size;
        }


        T& operator*() {
            return *(data_pointer + (head + iter) % block_size);
        }

        void operator++() {
            this->iter++;
        }

        void operator--() {
            this->iter--;
        }

        void operator+=(int n) {
            this->iter = this->iter + n;
        }

        void operator-=(int n) {
            this->iter = this->iter - n;
        }

        Iterator operator+(int n) {
            return *this += n;
        }

        Iterator operator-(int n) {
            return *this -= n;
        }

        bool operator==(const Iterator& rhs) {
            return (data_pointer == rhs.data_pointer) && (iter == rhs.iter);
        }

        bool operator!=(const Iterator& rhs) {
            return !(*this == rhs);
        }
    };

    Iterator begin() {
        return Iterator(data_value, 0, size_, head_);
    }

    Iterator end() {
        return Iterator(data_value, size_, size_, head_);
    }

    class KIterator : public Iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using type_value = T;
        using difference_type = T;
        using pointer = T*;
        using reference = T&;
        KIterator() : Iterator() {}

        KIterator(const T* ptr, size_t index, size_t size, size_t head) : Iterator(ptr, index, size, head) {}

        KIterator(const Iterator& it) : Iterator(it) {}

        KIterator(const KIterator& other) : Iterator(other) {}

        KIterator& operator=(const KIterator& other) {
            if (this != &other) {
                Iterator::operator=(other);
            }
            return *this;
        }

        const T& operator*() const {
            return const_cast<const T&>(Iterator::operator*());
        }

        const T* operator->() const {
            return const_cast<const T*>(Iterator::operator->());
        }
    };

    KIterator cbegin() const {
        return KIterator(data_value, 0, size_, head_);
    }

    KIterator cend() const {
        return KIterator(data_value, size_, size_, head_);
    }

    CCircularBuffer(size_t capacity, const Allocator& allocator = Allocator()) {
        allocator_ = allocator;
        data_value = allocator_.allocate(capacity);
        head_ = 0;
        capacity_ = capacity;
    }

    CCircularBuffer(const Allocator& allocator = Allocator()) :
            allocator_(allocator), data_value(nullptr), size_(0), capacity_(0), head_(0) {}

    CCircularBuffer(size_t capacity, const T& value, const Allocator& allocator = Allocator())
            : allocator_(allocator), data_value(allocator_.allocate(capacity)), size_(capacity), capacity_(capacity),
              head_(0) {
        for (size_t i = 0; i < size_; ++i) {
            data_value[i] = value;
        }
    }
    CCircularBuffer(const CCircularBuffer &other)
            : allocator_(other.allocator_), data_value(allocator_.allocate(other.capacity_)),
              size_(other.size_), capacity_(other.capacity_), head_(other.head_) {
        for (size_t i = 0; i < size_; ++i) {
            data_value[i] = other.data_value[i];
        }
    }


    ~CCircularBuffer() {
        clear();
        allocator_.deallocate(data_value, capacity_);
    }

    bool operator==(CCircularBuffer& rhs) {
        if (size_ == rhs.size_ && head_ == rhs.head_) {
            for (size_t i = 0; i < size_; i++) {
                if (data_value[i] != rhs.data_value[i]) {
                    return false;
                }
            }
            return true;
        }
        return false;
    }

    bool operator!=(CCircularBuffer& rhs) {
        return !(*this == rhs);
    }

    CCircularBuffer& operator=(CCircularBuffer& other) {
        allocator_ = other.allocator_;
        capacity_ = other.capacity_;
        size_ = other.size_;
        head_ = other.head_;
        data_value = allocator_.allocate(size_);
        for (int i = 0; i < size_; ++i) {
            data_value[i] = other.data_value[i];
        }
        return *this;
    }

    size_t size() {
        return size_;
    }

    size_t capacity() {
        return capacity_;
    }

    bool empty() {
        return (size_ == 0);
    }

    void push_back(const T& value) {
        if (size_ == capacity_) {
            throw std::runtime_error("Buffer overflow");
        }
        data_value[head_] = value;
        head_ = (head_ + 1) % capacity_;
        size_++;
    }

    void push_front(const T& value) {
        if (size_ < capacity_) {
            push_back(value);
            head_ = size_ - 1;
        } else {
            pop_back();
            push_front(value);
        }
    }

    void pop_front() {
        std::allocator_traits<Allocator>::destroy(allocator_, data_value + head_);
        head_ = (head_ + 1) % capacity_;
        --size_;
    }


    void pop_back() {
        if (empty()) {
            throw std::runtime_error("Buffer is empty");
        }
        size_--;
        std::allocator_traits<Allocator>::destroy(allocator_, data_value + size_);
    }

    void clear() {
        while (size_ > 0) {
            std::allocator_traits<Allocator>::destroy(allocator_, data_value + head_);
            head_ = (head_ + 1) % capacity_;
            --size_;
        }
    }

    void insert(Iterator iter, const T& value) {
        if (iter.index_ > size_) {
            return;
        }
        if (size_ == capacity_) {
            pop_back();
        }
        if (iter.index_ == size_) {
            push_back(value);
        } else {
            for (size_t i = size_; i > iter.index_; --i) {
                data_value[(head_ + i) % capacity_] = data_value[(head_ + i - 1) % capacity_];
            }
            data_value[(head_ + iter.index_) % capacity_] = value;
            ++size_;
        }
    }

    void reserve(size_t new_capacity) {
        if (new_capacity <= size_) {
            return;
        }
        T* new_data = allocator_.allocate(new_capacity);
        for (size_t i = 0; i < size_; ++i) {
            std::allocator_traits<Allocator>::construct(allocator_, new_data + i, data_value[head_ + i]);
        }
        std::allocator_traits<Allocator>::deallocate(allocator_, data_value, capacity_);
        data_value = new_data;
        capacity_ = new_capacity;
        head_ = 0;
    }

    void resize(const size_t new_capacity, const T& value = T()) {
        if (new_capacity <= size_) {
            return;
        } else {
            reserve(new_capacity);
            for (size_t i = size_; i < new_capacity; ++i) {
                std::allocator_traits<Allocator>::construct(allocator_, data_value + i, value);
            }
            size_ = new_capacity;
        }
    }

    T& operator[](size_t i) {
        return *(data_value + (head_ + i) % size_);
    }

    const T& operator[](size_t i) const {
        return *(data_value + (head_ + i) % size_);
    }

    Allocator allocator_;
    T* data_value = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;
    size_t head_ = 0;
};

template<typename T, typename Allocator = std::allocator<T>>
class CCircularBufferExt : public CCircularBuffer<T> {
public:
    using Cbuffer = CCircularBuffer<T, Allocator>;

    explicit CCircularBufferExt(size_t capacity, const Allocator& allocator = Allocator())
            : Cbuffer(capacity, allocator) {}

    CCircularBufferExt(size_t capacity, const T& value, const Allocator& allocator = Allocator())
            : Cbuffer(capacity, value, allocator) {}

    CCircularBufferExt(const Cbuffer& other)
            : Cbuffer(other) {
        for (size_t i = 0; i < this->size_; ++i) {
            this->data_[i] = other.data_[i];
        }
    }

    CCircularBufferExt(const CCircularBufferExt& other) : Cbuffer(other.data_value, other.size_, other.capacity_,
                                                                  other.head_, other.allocator_) {
    }

    CCircularBufferExt& operator=(const CCircularBufferExt& other) {
        if (this != other) {
            this->allocator_ = other.allocator_;
            this->data_value = this->allocator_.allocate(other.capacity_);
            this->size_ = other.size_;
            this->capacity_ = other.capacity_;
            this->head_ = other.head_;
            for (size_t i = 0; i < this->size_; ++i) {
                this->data_[i] = other.data_[i];
            }
        }
        return *this;
    }

    void push_back(const T& value) {
        if (this->size_ + 1 < this->capacity_) {
            this->data_value[(this->head_ + this->size_) % this->capacity_] = value;
            ++this->size_;
        } else {
            Cbuffer::reserve(this->capacity_ * 2);
            this->data_value[(this->head_ + this->size_) % this->capacity_] = value;
            ++this->size_;
        }
    }

    void push_front(const T& value) {
        if (this->size_ < this->capacity_) {
            push_back(value);
            this->head_ = this->size_ - 1;
        } else {
            Cbuffer::reserve(this->capacity_ * 2);
            push_front(value);
        }
    }

    void pop_front() {
        this->allocator_.destroy(this->data_value + this->head_);
        this->head_ = (this->head_ + 1) % this->capacity_;
        this->size_ = this->size_ - 1;
        if (this->size_ < this->capacity_ / 2) {
            Cbuffer::reserve(this->capacity_ / 2);
        }
    }

    void pop_back() {
        this->allocator_.destroy(this->data_value + this->size_);
        this->size_ = this->size_ - 1;
        if (this->size_ < this->capacity_ / 2) {
            Cbuffer::reserve(this->capacity_ / 2);
        }
    }
};
