#pragma once
#define _SCL_SECURE_NO_WARNINGS
#include <stdexcept>
#include <iterator>
#include <limits>
#include <algorithm>

namespace my {

template <class T> class vector { //TODO: use allocator to allocate memory not initializing it, use iterators
public:
    typedef T value_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef T* pointer;

    class iterator : public std::iterator<
        std::random_access_iterator_tag, T, ptrdiff_t, T*, T&>
    {
    public:
        iterator() : pos_(nullptr), container_(nullptr) {};
        iterator(const iterator& other) : pos_(other.pos_), container_(other.container_) {};
        iterator(vector<T>* container, pointer pos) : pos_(pos), container_(container) {}

        iterator& operator= (const iterator&);
        bool operator== (const iterator& other) { return real_pos() == other.real_pos(); };
        bool operator!= (const iterator& other) { return real_pos() != other.real_pos(); };
        bool operator> (const iterator& other) { return real_pos() > other.real_pos(); };
        bool operator< (const iterator& other) { return real_pos() < other.real_pos(); };
        bool operator>= (const iterator& other) { return real_pos() >= other.real_pos(); };
        bool operator<= (const iterator& other) { return real_pos() <= other.real_pos(); };

        iterator& operator++() { add(1); return *this; };
        iterator operator++(int) { iterator it(*this); add(1); return it; };
        iterator& operator--() { substract(1); return *this; };
        iterator operator--(int) { iterator it(*this); substract(1); return it; };
        iterator& operator+=(size_type n) { add(n); return *this; };
        iterator operator+(size_type n) const { iterator it(*this); it.add(n); return it; };
        friend iterator operator+(size_type n, const iterator& that) { iterator it(that); it.add(n); return it; };
        iterator& operator-=(size_type n) { substract(n); return *this; };
        iterator operator-(size_type n) const { iterator it(*this); it.substract(n); return it; };
        difference_type operator-(iterator other) const { return real_pos() - other.real_pos(); }

        reference operator*() const { return *pos_; };
        pointer operator->() const { return pos_; };
        reference operator[](size_type n) const { return *(pos_ + n); };

    private:
        pointer pos_;
        vector<T>* container_;

        void add(size_type);
        void substract(size_type);
        pointer real_pos() const { return pos_ != nullptr ? pos_ : container_->elements_ + container_->size_; }
    };

    vector() : elements_(nullptr), size_(0), capacity_(0) {};
    vector(size_type n);
    vector(size_type n, const T& value);
    //TODO: get into template constructors
    //template <class ForwardIterator, typename std::enable_if<
    //    std::is_convertible<typename std::iterator_traits<ForwardIterator>::iterator_category,
    //    std::forward_iterator_tag>::value, bool>::type>
    //    vector(ForwardIterator first, ForwardIterator last);
    //template <class ForwardIterator, typename std::enable_if<
    //    std::is_base_of<std::forward_iterator_tag, typename std::iterator_traits<ForwardIterator>::iterator_category>::value>::type>
    //    vector(ForwardIterator first, ForwardIterator last);
    template <class ForwardIterator>
        vector(ForwardIterator first, ForwardIterator last);
    vector(const vector& x);
    vector(vector&&) noexcept;
    vector(std::initializer_list<T>);
    ~vector();

    vector& operator=(const vector& x);
    vector& operator=(vector&& x) noexcept;
    vector& operator=(std::initializer_list<T>);

    //TODO: assign must be rewritten not to allocate more memory
    template <class ForwardIterator> void assign(ForwardIterator first, ForwardIterator last);
    void assign(size_type n, const T& u);
    void assign(std::initializer_list<T>);

    iterator begin() noexcept { return iterator(this, size_ > 0 ? elements_ : nullptr); }
    iterator end() noexcept { return iterator(this, nullptr); }

    size_type size() const noexcept { return size_; }
    size_type max_size() const noexcept { return std::numeric_limits<size_type>::max(); }
    void resize(size_type sz);
    void resize(size_type sz, const T& c);
    size_type capacity() const noexcept { return capacity_; }
    bool empty() const noexcept { return size_ == 0; }
    void reserve(size_type n) { if (n > capacity_) allocate_more(n); }
    void shrink_to_fit() { allocate_more(size_); }

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

    void swap(vector&) noexcept;
    void clear() noexcept;
private:
    pointer elements_;
    size_type size_;
    size_type capacity_;

    void allocate(size_type n);
    void allocate_more(size_type n);
};

template<class T> 
void vector<T>::allocate(size_type n) {
    size_type new_capacity = static_cast<size_type>(std::floor(pow(2, std::floor(std::log2(n)) + 1)));
    elements_ = new T[new_capacity];
    capacity_ = new_capacity;
}

template<class T>
void vector<T>::allocate_more(size_type n) {
    T* old = elements_;
    size_type new_capacity = static_cast<size_type>(std::floor(pow(2, std::floor(std::log2(n)) + 1)));
    elements_ = new T[new_capacity];
    capacity_ = new_capacity;
    std::copy(old, old + size_, elements_);
    delete[] old;
}

template <class T>
typename vector<T>::iterator& vector<T>::iterator::operator=(const iterator& other) {
    auto tmp(other);
    std::swap(pos_, tmp.pos_);
    std::swap(container_, tmp.container_);
    return *this;
}

template <class T>
void vector<T>::iterator::add(size_type n) {
    if (pos_ == nullptr) {
        return;
    }
    pos_ += n;
    if (pos_ >= container_->elements_ + container_->size_) {
        pos_ = nullptr;
    }
}

template <class T>
void vector<T>::iterator::substract(size_type n) {
    if (pos_ == nullptr && n > 0) {
        pos_ = container_->elements_ + container_->size_;
    }
    pos_ -= n;
    if (pos_ < container_->elements_) {
        pos_ = container_->elements_;
    }
}

template <class T>
vector<T>::vector(size_type n): size_(n) {
    allocate(n);
}

template <class T>
vector<T>::vector(size_type n, const T& value): size_(n) {
    allocate(n);
    std::fill(elements_, elements_ + size_, value);
}

template <class T>
template <class ForwardIterator>
vector<T>::vector(ForwardIterator first, ForwardIterator last) : size_(std::distance(first, last)) {
    allocate(size_);
    std::copy(first, last, elements_);
}

//template <class T>
//template <class ForwardIterator, typename std::enable_if<
//    std::is_convertible<typename std::iterator_traits<ForwardIterator>::iterator_category,
//    std::forward_iterator_tag>::value, bool>::type>
//vector<T>::vector(ForwardIterator first, ForwardIterator last): size_(std::distance(first, last)) {
//    allocate(size_);
//    std::copy(first, last, elements_);
//}

//template <class T>
//template <class ForwardIterator, typename std::enable_if<
//    std::is_base_of<std::forward_iterator_tag, typename std::iterator_traits<ForwardIterator>::iterator_category>::value>::type>
//vector<T>::vector(ForwardIterator first, ForwardIterator last) : size_(std::distance(first, last)) {
//    allocate(size_);
//    std::copy(first, last, elements_);
//}

template <class T>
vector<T>::vector(const vector& x) : size_(x.size_) {
    allocate(x.capacity_);
    std::copy(x.elements_, x.elements_ + x.size_, elements_);
}

template <class T>
vector<T>::vector(vector&& x) noexcept : elements_(nullptr), size_(0), capacity_(0) {
    std::swap(elements_, x.elements_);
    std::swap(size_, x.size_);
    std::swap(capacity_, x.capacity_);
}

template <class T>
vector<T>::vector(std::initializer_list<T> l) : size_(l.size()) {
    allocate(size_);
    std::copy(l.begin(), l.end(), elements_);
}

template <class T>
vector<T>::~vector() {
    delete[] elements_;
}

template <class T>
vector<T>& vector<T>::operator=(const vector& x) {
    vector<T> tmp(x);
    swap(tmp);
    return *this;
}

template <class T>
vector<T>& vector<T>::operator=(vector&& x) noexcept {
    swap(x);
    return *this;
}

template <class T>
vector<T>& vector<T>::operator=(std::initializer_list<T> l) {
    size_ = l.size();
    allocate(size_);
    std::copy(l.begin(), l.end(), elements_);
    return *this;
}

template <class T>
template <class ForwardIterator>
void vector<T>::assign(ForwardIterator first, ForwardIterator last) {
    size_type new_size = std::distance(first, last);
    pointer old_elements = elements_;
    allocate(new_size);
    std::copy(first, last, elements_);
    size_ = new_size;
    delete[] old_elements;
}

template <class T>
void vector<T>::assign(size_type n, const T& u) {
    pointer old_elements = elements_;
    allocate(n);
    std::fill(elements_, elements_ + n, u);
    size_ = n;
    delete[] old_elements;
}

template <class T>
void vector<T>::assign(std::initializer_list<T> l) {
    size_type new_size = l.size();
    pointer old_elements = elements_;
    allocate(new_size);
    std::copy(l.begin(), l.end(), elements_);
    size_ = new_size;
    delete[] old_elements;
}

template <class T>
void vector<T>::resize(size_type sz) {
    if (sz <= capacity_) {
        std::fill(begin() + sz, end(), T());
    }
    else {
        allocate_more(sz);
    }
    size_ = sz;
}

template <class T>
void vector<T>::resize(size_type sz, const T& c) {
    if (sz <= capacity_) {
        std::fill(begin() + sz, end(), T());
    }
    else {
        allocate_more(sz);
    }
    if (sz > size_) {
        std::fill(elements_ + size_, elements_ + sz, T(c));
    }
    size_ = sz;
}

template <class T>
typename vector<T>::const_reference vector<T>::at(size_type n) const {
    if (n >= size_) {
        throw std::out_of_range("Vector subscript out of range");
    }
    return elements_[n];
}

template <class T>
typename vector<T>::reference vector<T>::at(size_type n) {
    if (n >= size_) {
        throw std::out_of_range("Vector subscript out of range");
    }
    return elements_[n];
}

template <class T>
void vector<T>::push_back(const T& x) {
    if (size_ >= capacity_) {
        allocate_more(size_ + 1);
    }
    elements_[size_++] = T(x);
}

template <class T>
void vector<T>::push_back(T&& x) {
    if (size_ >= capacity_) {
        allocate_more(size_ + 1);
    }
    elements_[size_++] = T(x);
}

template <class T>
void vector<T>::pop_back() {
    size_ = std::max(size_ - 1, 0U);
    elements_[size_] = T();
}

template <class T>
typename vector<T>::iterator vector<T>::insert(iterator position, const T& x) {
    difference_type p = position - begin();
    if (size_ >= capacity_) {
        allocate_more(size_ + 1);
    }
    std::move_backward(begin() + p, end(), elements_ + size_ + 1);
    elements_[p] = T(x);
    ++size_;
    return begin() + p;
}

template <class T>
typename vector<T>::iterator vector<T>::insert(iterator position, T&& x) {
    difference_type p = position - begin();
    if (size_ >= capacity_) {
        allocate_more(size_ + 1);
    }
    std::move_backward(begin() + p, end(), elements_ + size_ + 1);
    elements_[p] = T(x);
    ++size_;
    return begin() + p;
}

template <class T>
typename vector<T>::iterator vector<T>::insert(iterator position, size_type n, const T& x) {
    difference_type p = position - begin();
    if (size_ + n > capacity_) {
        allocate_more(size_ + n);
    }
    std::move_backward(begin() + p, end(), elements_ + size_ + n);
    std::fill(elements_ + p, elements_ + p + n, T(x));
    size_ += n;
    return begin() + p;
}

template <class T>
template <class ForwardIterator>
typename vector<T>::iterator vector<T>::insert(iterator position, ForwardIterator first, ForwardIterator last) {
    size_type n = std::distance(first, last);
    difference_type p = position - begin();
    if (size_ + n > capacity_) {
        allocate_more(size_ + n);
    }
    std::move_backward(begin() + p, end(), elements_ + size_ + n);
    std::copy(first, last, elements_ + p);
    size_ += n;
    return begin() + p;
}

template <class T>
typename vector<T>::iterator vector<T>::insert(iterator position, std::initializer_list<T> il) {
    size_type n = il.size();
    difference_type p = position - begin();
    if (size_ + n > capacity_) {
        allocate_more(size_ + n);
    }
    std::move_backward(begin() + p, end(), elements_ + size_ + n);
    std::copy(il.begin(), il.end(), elements_ + p);
    size_ += n;
    return begin() + p;
}

template <class T>
typename vector<T>::iterator vector<T>::erase(iterator position) {
    std::move(position + 1, end(), position);
    --size_;
    return position;
}

template <class T>
typename vector<T>::iterator vector<T>::erase(iterator first, iterator last) {
    auto d = last - first;
    std::move(last, end(), first);
    size_ -= d;
    return last != end() ? first : end();
}

template<class T>
void vector<T>::swap(vector& other) noexcept {
    std::swap(elements_, other.elements_);
    std::swap(capacity_, other.capacity_);
    std::swap(size_, other.size_);
}

template <class T>
void vector<T>::clear() noexcept {
    delete[] elements_;
    elements_ = nullptr;
    size_ = 0;
    capacity_ = 0;
}

}
