#pragma once // NOLINT
#include <iterator>


namespace task {


template<class T>
class list {
 public:
    class iterator {
     public:
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::bidirectional_iterator_tag;

        explicit iterator(pointer ptr) : node(ptr) { };
        iterator(const iterator& it) : node(it.node) { };
        iterator& operator=(const iterator& it) { node = it.node; return *this; }

        virtual iterator& operator++() { node++; return *this; }
        virtual iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }
        virtual reference operator*() const { return *node; }
        virtual pointer operator->() const { return node; }
        virtual iterator& operator--() { node--; return *this; }
        virtual iterator operator--(int) { iterator tmp = *this; --(*this); return tmp; }

        virtual bool operator==(iterator other) const { return this->node == other.node; }
        virtual bool operator!=(iterator other) const { return this->node != other.node; }

     private:
        pointer node;
    };

    class const_iterator {
     public:
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = const T*;
        using reference = const T&;
        using iterator_category = std::bidirectional_iterator_tag;

        explicit const_iterator(pointer ptr) : node(ptr) { };
        explicit const_iterator(const iterator& it) : node(it.operator->()) { };
        const_iterator(const const_iterator& it) : node(it.operator->()) { };
        const_iterator& operator=(const const_iterator& it) { node = it.node; return *this; }

        const_iterator& operator++() { node++; return *this; }
        const_iterator operator++(int) { const_iterator tmp = *this; ++(*this); return tmp; }
        reference operator*() const { return *node; }
        pointer operator->() const { return node; }
        const_iterator& operator--() { node--; return *this; }
        const_iterator operator--(int) { const_iterator tmp = *this; --(*this); return tmp; }

        bool operator==(const_iterator other) const { return this->node == other.node; }
        bool operator!=(const_iterator other) const { return this->node != other.node; }
     private:
        pointer node;
    };

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;


    list();
    list(size_t count, const T& value);
    explicit list(size_t count);
    ~list() { free(m_data); }

    list(const list& other);
    list& operator=(const list& other);

    T& front() { return *begin(); }
    const T& front() const { return *begin(); }

    T& back() { return *std::prev(end()); }
    const T& back() const { return *std::prev(cend()); }


    iterator begin() const { return iterator(m_data); }
    iterator end() const { return iterator(m_data + _size); }

    const_iterator cbegin() const { return const_iterator(begin()); }
    const_iterator cend() const { return const_iterator(end()); }

    reverse_iterator rbegin() const { return std::make_reverse_iterator(end()); }
    reverse_iterator rend() const { return std::make_reverse_iterator(begin()); }

    const_reverse_iterator crbegin() const { return std::make_reverse_iterator(end()); }
    const_reverse_iterator crend() const { return std::make_reverse_iterator(begin()); }


    bool empty() const { return _size == 0; }
    size_t size() const { return _size; }
    size_t max_size() const { return _size; }
    void clear();

    iterator insert(const_iterator pos, const T& value);
    iterator insert(const_iterator pos, size_t count, const T& value);

    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);


    void push_back(const T& value);
    void pop_back();

    void push_front(const T& value);
    void pop_front();

    void resize(size_t count);
    void swap(list& other);


    void merge(list& other);
    void splice(const_iterator pos, list& other);
    void remove(const T& value);
    void reverse();
    void unique();
    void sort();

 private:

    size_t _size;
    T* m_data;

};

    template<class T>
    list<T>::list() {
        m_data = (T*) calloc(0, sizeof(T));
        _size = 0;
    }

    template<class T>
    list<T>::list(size_t count, const T &value) : _size(count) {
        m_data = (T*) calloc(count, sizeof(T));
        std::fill(begin(), end(), value);
    }

    template<class T>
    list<T>::list(size_t count) : _size(count) {
        m_data = (T*) calloc(count, sizeof(T));
    }

    template<class T>
    class list<T>::iterator list<T>::insert(list::const_iterator pos, const T &value) {
        T* temp = (T*) calloc(_size + 1, sizeof(T));
        std::copy(cbegin(), pos, iterator(temp));
        iterator new_pos = iterator(temp + std::distance(cbegin(), pos));
        *new_pos = value;
        std::copy(pos, cend(), std::next(new_pos, 1));
        ++_size;
        free(m_data);
        m_data = temp;
        return iterator(m_data);
    }

    template<class T>
    void list<T>::push_back(const T &value) {
        insert(cend(), value);
    }

    template<class T>
    void list<T>::push_front(const T &value) {
        insert(cbegin(), value);
    }

    template<class T>
    class list<T>::iterator list<T>::erase(const_iterator pos) {
        T* temp = (T*) calloc(_size - 1, sizeof(T));
        std::copy(cbegin(), pos, iterator(temp));
        std::copy(std::next(pos), cend(), iterator(temp + std::distance(cbegin(), pos)));
        --_size;
        free(m_data);
        m_data = temp;
        return end();
    }

    template<class T>
    void list<T>::pop_front() {
        erase(cbegin());
    }

    template<class T>
    void list<T>::pop_back() {
        erase(std::prev(cend()));
    }

    template<class T>
    void list<T>::clear() {
        free(m_data);
        m_data = (T*) calloc(0, sizeof(T));
        _size = 0;
    }

    template<class T>
    void list<T>::resize(size_t count) {
        T* temp = (T*) calloc(count, sizeof(T));
        if (count >= _size) {
            std::copy(begin(), end(), iterator(temp));
        } else {
            std::copy_n(begin(), count, iterator(temp));
        }
        _size = count;
        free(m_data);
        m_data = temp;
    }

    template<class T>
    class list<T>::iterator list<T>::insert(list::const_iterator pos, size_t count, const T &value) {
        T* temp = (T*) calloc(_size + count, sizeof(T));
        std::copy(cbegin(), pos, iterator(temp));
        iterator new_pos = iterator(temp + std::distance(cbegin(), pos));
        std::fill(new_pos, std::next(new_pos, count), value);
        std::copy(pos, cend(), std::next(new_pos, count));
        _size += count;
        free(m_data);
        m_data = temp;
        return iterator(m_data);
    }

    template<class T>
    class list<T>::iterator list<T>::erase(list::const_iterator first, list::const_iterator last) {
        T* temp = (T*) calloc(_size - std::distance(first, last), sizeof(T));
        std::copy(cbegin(), first, iterator(temp));
        std::copy(last, cend(), iterator(temp + std::distance(cbegin(), first)));
        _size -= std::distance(first, last);
        free(m_data);
        m_data = temp;
        return end();
    }

    template<class T>
    void list<T>::sort() {
        // Bubble sort
        for (iterator it1 = begin(); it1 != end(); ++it1) {
            for (iterator it2 = begin(); it2 != std::prev(end()); ++it2) {
                if (*it2 > *std::next(it2)) {
                    std::iter_swap(it2, std::next(it2));
                }
            }
        }
    }

    template<class T>
    void list<T>::reverse() {
        std::reverse(begin(), end());
    }

    template<class T>
    list<T>::list(const list &other) {
        if (&other != this) {
            _size = other._size;
            m_data = (T*) calloc(_size, sizeof(T));
            std::copy_n(other.m_data, _size, m_data);
        }
    }

    template<class T>
    class list<T>& list<T>::operator=(const list &other)  {
        if (&other != this) {
            free(m_data);
            _size = other._size;
            m_data = (T*) calloc(_size, sizeof(T));
            std::copy(other.begin(), other.end(), iterator(m_data));
        }
        return *this;
    }

    template<class T>
    void list<T>::splice(list::const_iterator pos, list &other) {
        //
    }

    template<class T>
    void list<T>::unique() {
        //
    }

    template<class T>
    void list<T>::remove(const T &value) {
        //
    }

    template<class T>
    void list<T>::merge(list &other) {
        //
    }

    template<class T>
    void list<T>::swap(list &other) {
        //
    }


}  // namespace task
