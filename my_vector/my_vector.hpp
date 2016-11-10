#pragma once
#define _SCL_SECURE_NO_WARNINGS
#include <stdexcept>
#include <iterator>
#include <limits>
#include <xmemory>

namespace my {

template <class T, class Allocator = std::allocator<T>> class vector {
public:
    typedef T value_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef T* pointer;
    typedef Allocator allocator_type;

    class iterator : public std::iterator<
        std::random_access_iterator_tag, T, ptrdiff_t, T*, T&>
    {
    public:
        iterator() : pos_(nullptr), container_(nullptr) {};
        iterator(const iterator& other) : pos_(other.pos_), container_(other.container_) {};
        iterator(vector<T, Allocator>* container, pointer pos) : pos_(pos), container_(container) {}

        iterator& operator= (const iterator&);
        bool operator== (const iterator& other) { return pos_ == other.pos_; };
        bool operator!= (const iterator& other) { return pos_ != other.pos_; };
        bool operator> (const iterator& other) { return pos_ > other.pos_; };
        bool operator< (const iterator& other) { return pos_ < other.pos_; };
        bool operator>= (const iterator& other) { return pos_ >= other.pos_; };
        bool operator<= (const iterator& other) { return pos_ <= other.pos_; };

        iterator& operator++() { add(1); return *this; };
        iterator operator++(int) { iterator it(*this); add(1); return it; };
        iterator& operator--() { substract(1); return *this; };
        iterator operator--(int) { iterator it(*this); substract(1); return it; };
        iterator& operator+=(size_type n) { add(n); return *this; };
        iterator operator+(size_type n) const { iterator it(*this); it.add(n); return it; };
        friend iterator operator+(size_type n, const iterator& that) { iterator it(that); it.add(n); return it; };
        iterator& operator-=(size_type n) { substract(n); return *this; };
        iterator operator-(size_type n) const { iterator it(*this); it.substract(n); return it; };
        difference_type operator-(iterator other) const { return pos_ - other.pos_; }

        reference operator*() const { return *pos_; };
        pointer operator->() const { return pos_; };
        reference operator[](size_type n) const { return *(pos_ + n); };

        pointer pos_;
    private:
        vector<T, Allocator>* container_;

        void add(size_type);
        void substract(size_type);
    };

    vector() : elements_(nullptr), size_(0), capacity_(0), allocator_() {};
    vector(size_type n);
    vector(size_type n, const T& value);
    template <class ForwardIterator>
        vector(ForwardIterator first, ForwardIterator last);
    vector(const vector& x);
    vector(vector&&) noexcept;
    vector(std::initializer_list<T>);
    ~vector();

    vector& operator=(const vector& x);
    vector& operator=(vector&& x) noexcept;
    vector& operator=(std::initializer_list<T>);

    template <class ForwardIterator> void assign(ForwardIterator first, ForwardIterator last);
    void assign(size_type n, const T& u);
    void assign(std::initializer_list<T>);

    iterator begin() noexcept { return iterator(this, elements_); }
    iterator end() noexcept { return iterator(this, elements_ + size_); }

    size_type size() const noexcept { return size_; }
    size_type max_size() const noexcept { return allocator_.max_size(); }
    void resize(size_type sz);
    void resize(size_type sz, const T& c);
    size_type capacity() const noexcept { return capacity_; }
    bool empty() const noexcept { return size_ == 0; }
    void reserve(size_type n) { if (n > capacity_) allocate(n); }
    void shrink_to_fit() { allocate(size_); }

    reference operator[](size_type n) { return elements_[n]; }
    const_reference operator[](size_type n) const { return elements_[n]; }
    const_reference at(size_type n) const;
    reference at(size_type n);
    reference front() { return elements_[0]; }
    const_reference front() const { return elements_[0]; }
    reference back() { return elements_[size_ - 1]; }
    const_reference back() const { return elements_[size_ - 1]; }
    pointer data() noexcept { return elements_; }
    const pointer data() const noexcept { return elements_; }

    void push_back(const T& x);
    void push_back(T&& x);
    void pop_back();
    iterator insert(iterator position, const T& x);
    iterator insert(iterator position, T&& x);
    iterator insert(iterator position, size_type n, const T& x);
    template <class ForwardIterator> iterator insert(iterator position, ForwardIterator first, ForwardIterator last);
    iterator insert(iterator position, std::initializer_list<T> il);
    iterator erase(iterator position);
    iterator erase(iterator first, iterator last);

    template<class... Args> iterator emplace(iterator pos, Args&&... args);
    template<class... Args> void emplace_back(Args&&... args);

    void swap(vector&) noexcept;
    void clear() noexcept;
    allocator_type get_allocator() const;
private:
    pointer elements_;
    size_type size_;
    size_type capacity_;
    allocator_type allocator_;

    void allocate(size_type n);
};

template <class T, class Allocator>
void vector<T, Allocator>::allocate(size_type n) {
    size_type new_capacity = static_cast<size_type>(std::floor(pow(2, std::floor(std::log2(n)) + 1)));
    T* new_elements = allocator_.allocate(new_capacity);
    if (elements_ != nullptr && n > size_) {
        std::uninitialized_copy(elements_, elements_ + size_, new_elements);
    }
    for (auto i = elements_; i < elements_ + size_; ++i) {
        allocator_.destroy(i);
    }
    allocator_.deallocate(elements_, capacity_);
    capacity_ = new_capacity;
    elements_ = new_elements;
}

template <class T, class Allocator>
typename vector<T, Allocator>::iterator& vector<T, Allocator>::iterator::operator=(const iterator& other) {
    auto tmp(other);
    std::swap(pos_, tmp.pos_);
    std::swap(container_, tmp.container_);
    return *this;
}

template <class T, class Allocator>
void vector<T, Allocator>::iterator::add(size_type n) {
    pos_ += n;
}

template <class T, class Allocator>
void vector<T, Allocator>::iterator::substract(size_type n) {
    pos_ -= n;
}

template <class T, class Allocator>
vector<T, Allocator>::vector(size_type n): elements_(nullptr), size_(0), capacity_(0), allocator_() {
    allocate(n);
    size_ = n;
}

template <class T, class Allocator>
vector<T, Allocator>::vector(size_type n, const T& value): elements_(nullptr), size_(0), capacity_(0), allocator_() {
    allocate(n);
    std::uninitialized_fill(elements_, elements_ + n, value);
    size_ = n;
}

template <class T, class Allocator>
template <class ForwardIterator>
vector<T, Allocator>::vector(ForwardIterator first, ForwardIterator last) : elements_(nullptr), size_(0), capacity_(0), allocator_() {
    allocate(std::distance(first, last));
    std::uninitialized_copy(first, last, elements_);
    size_ = std::distance(first, last);
}

template <class T, class Allocator>
vector<T, Allocator>::vector(const vector& x) : elements_(nullptr), size_(0), capacity_(0), allocator_() {
    allocate(x.capacity_);
    std::uninitialized_copy(x.elements_, x.elements_ + x.size_, elements_);
    size_ = x.size_;
}

template <class T, class Allocator>
vector<T, Allocator>::vector(vector&& x) noexcept : elements_(nullptr), size_(0), capacity_(0), allocator_() {
    swap(x);
}

template <class T, class Allocator>
vector<T, Allocator>::vector(std::initializer_list<T> l) : elements_(nullptr), size_(0), capacity_(0), allocator_() {
    allocate(l.size());
    std::uninitialized_copy(l.begin(), l.end(), elements_);
    size_ = l.size();
}

template <class T, class Allocator>
vector<T, Allocator>::~vector() {
    for (auto i = elements_; i < elements_ + size_; ++i) {
        allocator_.destroy(i);
    }
    allocator_.deallocate(elements_, capacity_);
}

template <class T, class Allocator>
vector<T, Allocator>& vector<T, Allocator>::operator=(const vector& x) {
    vector<T, Allocator> tmp(x);
    swap(tmp);
    return *this;
}

template <class T, class Allocator>
vector<T, Allocator>& vector<T, Allocator>::operator=(vector&& x) noexcept {
    swap(x);
    return *this;
}

template <class T, class Allocator>
vector<T, Allocator>& vector<T, Allocator>::operator=(std::initializer_list<T> l) {
    for (auto i = elements_; i < elements_ + size_; ++i) {
        allocator_.destroy(i);
    }
    size_ = 0;
    if (l.size() > capacity_) {
        allocate(l.size());
    }
    std::uninitialized_copy(std::make_move_iterator(l.begin()), std::make_move_iterator(l.end()), elements_);
    size_ = l.size();
    return *this;
}

template <class T, class Allocator>
template <class ForwardIterator>
void vector<T, Allocator>::assign(ForwardIterator first, ForwardIterator last) {
    for (auto i = elements_; i < elements_ + size_; ++i) {
        allocator_.destroy(i);
    }
    size_ = 0;
    size_type new_size = std::distance(first, last);
    if (new_size > capacity_) {
        allocate(new_size);
    }
    std::uninitialized_copy(first, last, elements_);
    size_ = new_size;
}

template <class T, class Allocator>
void vector<T, Allocator>::assign(size_type n, const T& u) {
    for (auto i = elements_; i < elements_ + size_; ++i) {
        allocator_.destroy(i);
    }
    size_ = 0;
    if (n > capacity_) {
        allocate(n);
    }
    std::uninitialized_fill(elements_, elements_ + n, u);
    size_ = n;
}

template <class T, class Allocator>
void vector<T, Allocator>::assign(std::initializer_list<T> l) {
    for (auto i = elements_; i < elements_ + size_; ++i) {
        allocator_.destroy(i);
    }
    size_ = 0;
    size_type new_size = l.size();
    if (new_size > capacity_) {
        allocate(new_size);
    }
    std::uninitialized_copy(l.begin(), l.end(), elements_);
    size_ = new_size;
}

template <class T, class Allocator>
void vector<T, Allocator>::resize(size_type sz) {
    if (sz <= size_) {
        for (auto i = elements_ + sz; i < elements_ + size_; ++i) {
            allocator_.destroy(i);
        }
    }
    else {
        if (sz > capacity_) {
            allocate(sz);
        }
        std::uninitialized_fill(elements_ + size_, elements_ + sz, T());
    }
    size_ = sz;
}

template <class T, class Allocator>
void vector<T, Allocator>::resize(size_type sz, const T& c) {
    if (sz <= size_) {
        for (auto i = elements_ + sz; i < elements_ + size_; ++i) {
            allocator_.destroy(i);
        }
    }
    else {
        if (sz > capacity_) {
            allocate(sz);
        }
        std::uninitialized_fill(elements_ + size_, elements_ + sz, c);
    }
    size_ = sz;
}

template <class T, class Allocator>
typename vector<T, Allocator>::const_reference vector<T, Allocator>::at(size_type n) const {
    if (n >= size_) {
        throw std::out_of_range("Vector subscript out of range");
    }
    return elements_[n];
}

template <class T, class Allocator>
typename vector<T, Allocator>::reference vector<T, Allocator>::at(size_type n) {
    if (n >= size_) {
        throw std::out_of_range("Vector subscript out of range");
    }
    return elements_[n];
}

template <class T, class Allocator>
void vector<T, Allocator>::push_back(const T& x) {
    if (size_ + 1 > capacity_) {
        allocate(size_ + 1);
    }
    allocator_.construct(elements_ + (size_++), x);
}

template <class T, class Allocator>
void vector<T, Allocator>::push_back(T&& x) {
    if (size_ + 1 > capacity_) {
        allocate(size_ + 1);
    }
    allocator_.construct(elements_ + (size_++), x);
}

template <class T, class Allocator>
void vector<T, Allocator>::pop_back() {
    allocator_.destroy(elements_ + (size_--));
}

template <class T, class Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(iterator position, const T& x) {
    difference_type p = position - begin();
    if (size_ + 1 > capacity_) {
        allocate(size_ + 1);
    }
    if (elements_ + p < elements_ + size_) {
        std::move_backward(elements_ + p, elements_ + size_, elements_ + size_ + 1);
    }
    allocator_.construct(elements_ + p, x);
    ++size_;
    return begin() + p;
}

template <class T, class Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(iterator position, T&& x) {
    difference_type p = position - begin();
    if (size_ + 1 > capacity_) {
        allocate(size_ + 1);
    }
    if (elements_ + p < elements_ + size_) {
        std::move_backward(elements_ + p, elements_ + size_, elements_ + size_ + 1);
    }
    allocator_.construct(elements_ + p, x);
    ++size_;
    return begin() + p;
}

template <class T, class Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(iterator position, size_type n, const T& x) {
    difference_type p = position - begin();
    if (size_ + n > capacity_) {
        allocate(size_ + n);
    }
    if (elements_ + p < elements_ + size_) {
        std::move_backward(elements_ + p, elements_ + size_, elements_ + size_ + n);
    }
    std::uninitialized_fill(elements_ + p, elements_ + p + n, x);
    size_ += n;
    return begin() + p;
}

template <class T, class Allocator>
template <class ForwardIterator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(iterator position, ForwardIterator first, ForwardIterator last) {
    size_type n = std::distance(first, last);
    difference_type p = position - begin();
    if (size_ + n > capacity_) {
        allocate(size_ + n);
    }
    if (elements_ + p < elements_ + size_ + 1) {
        std::move_backward(elements_ + p, elements_ + size_, elements_ + size_ + n);
    }
    std::uninitialized_copy(first, last, elements_ + p);
    size_ += n;
    return begin() + p;
}

template <class T, class Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(iterator position, std::initializer_list<T> il) {
    size_type n = il.size();
    difference_type p = position - begin();
    if (size_ + n > capacity_) {
        allocate(size_ + n);
    }
    if (elements_ + p < elements_ + size_) {
        std::move_backward(elements_ + p, elements_ + size_, elements_ + size_ + n);
    }
    std::uninitialized_copy(il.begin(), il.end(), elements_ + p);
    size_ += n;
    return begin() + p;
}

template <class T, class Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::erase(iterator position) {
    allocator_.destroy(position.pos_);
    std::move(position + 1, end(), position);
    --size_;
    return position;
}

template <class T, class Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::erase(iterator first, iterator last) {
    for (auto i = first.pos_; i != last.pos_; ++i) {
        allocator_.destroy(i);
    }
    auto d = last - first;
    std::move(last, end(), first);
    size_ -= d;
    return first;
}

template <class T, class Allocator>
template <class ... Args>
typename vector<T, Allocator>::iterator vector<T, Allocator>::emplace(iterator pos, Args&&... args) {
    difference_type p = pos - begin();
    if (size_ + 1 > capacity_) {
        allocate(size_ + 1);
    }
    if (elements_ + p < elements_ + size_) {
        std::move_backward(elements_ + p, elements_ + size_, elements_ + size_ + 1);
    }
    allocator_.construct(elements_ + p, std::forward<Args>(args)...);
    ++size_;
    return begin() + p;
}

template <class T, class Allocator>
template <class ... Args>
void vector<T, Allocator>::emplace_back(Args&&... args) {
    if (size_ + 1 > capacity_) {
        allocate(size_ + 1);
    }
    allocator_.construct(elements_ + (size_++), std::forward<Args>(args)...);
}

template <class T, class Allocator>
void vector<T, Allocator>::swap(vector& other) noexcept {
    std::swap(elements_, other.elements_);
    std::swap(capacity_, other.capacity_);
    std::swap(size_, other.size_);
}

template <class T, class Allocator>
void vector<T, Allocator>::clear() noexcept {
    for (auto i = elements_; i < elements_ + size_; ++i) {
        allocator_.destroy(i);
    }
    allocator_.deallocate(elements_, capacity_);
    elements_ = nullptr;
    size_ = 0;
    capacity_ = 0;
}

template <class T, class Allocator>
typename vector<T, Allocator>::allocator_type vector<T, Allocator>::get_allocator() const {
    return allocator_;
}
}
