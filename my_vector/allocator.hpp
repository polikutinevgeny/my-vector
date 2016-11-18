#pragma once
#include <list>

namespace my {

enum class block_status {
    ALLOCATED, 
    FREE
};

template <class T>
class allocator;

template<class T>
struct memory_pool {
    memory_pool() {
        max_size = 0xFFFFF;
        pool = static_cast<typename allocator<T>::pointer>(::operator new(max_size * sizeof(T)));
        blocks = { typename allocator<T>::block(pool, max_size - 1, block_status::FREE) };
    };
    ~memory_pool() {
        ::operator delete(pool, max_size);
    }
    typename allocator<T>::size_type max_size;
    typename allocator<T>::pointer pool;
    std::list<typename allocator<T>::block> blocks;
};

template<class T>
memory_pool<T>& pool() {
    static memory_pool<T> pool;
    return pool;
}

template <class T>
class allocator {
public:
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef const T* const_pointer;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    template<class U> struct rebind { typedef allocator<U> other; };

    allocator() noexcept { }
    allocator(const allocator<T>&) noexcept { }
    template<class U> allocator(const allocator<U>&) noexcept {}

    pointer allocate(size_type n);
    void deallocate(pointer p, size_type n);

    template <class... Args> void construct(pointer p, Args&&... args) {
        ::new (static_cast<void*>(p)) T(std::forward<Args>(args)...);
    };
    void destroy(pointer p) { p->~value_type(); };

    size_type max_size() const noexcept { return pool<T>().max_size; }

    struct block {
        block(pointer start, size_type length, block_status status) noexcept
            : start(start), length(length), status(status) {}
        pointer start;
        size_type length;
        block_status status;
    };

};

template <class T>
typename allocator<T>::pointer allocator<T>::allocate(size_type n) {
    for (auto a = pool<T>().blocks.begin(); a != pool<T>().blocks.end(); ++a) {
        if (a->length > n && a->status == block_status::FREE) {
            auto res = pool<T>().blocks.emplace(a, a->start, n, block_status::ALLOCATED);
            a->start = a->start + n;
            a->length = a->length - n;
            return res->start;
        }
        else if (a->length == n && a->status == block_status::FREE) {
            a->status = block_status::ALLOCATED;
            return a->start;
        }
    }
    throw std::bad_alloc();
}

template <class T>
void allocator<T>::deallocate(pointer p, size_type n) {
    if (p == nullptr) {
        return;
    }
    for (auto a = pool<T>().blocks.begin(); a != pool<T>().blocks.end(); ++a) {
        if (a->start == p && a->length == n) {
            a->status = block_status::FREE;
            if (a != pool<T>().blocks.begin()) {
                auto i = a;
                for (--i; i != pool<T>().blocks.begin();) {
                    if (i->status == block_status::FREE) {
                        a->start = i->start;
                        a->length += i->length;
                        i = pool<T>().blocks.erase(i);
                        --i;
                    }
                    else {
                        break;
                    }
                }
            }
            auto i = a;
            for (++i; i != pool<T>().blocks.end();) {
                if (i->status == block_status::FREE) {
                    a->length += i->length;
                    i = pool<T>().blocks.erase(i);
                }
                else {
                    break;
                }
            }
        }
    }
}

template <class T, class U>
constexpr bool operator== (const allocator<T>&, const allocator<U>&) noexcept {return true;}

template <class T, class U>
constexpr bool operator!= (const allocator<T>&, const allocator<U>&) noexcept {return false;}


}
