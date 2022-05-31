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
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::bidirectional_iterator_tag;

        explicit const_iterator(pointer ptr) : node(ptr) { };
        explicit const_iterator(const iterator& it) : node(it.operator->()) { };
        const_iterator(const const_iterator& it) : node(it.operator->()) { };
        const_iterator& operator=(const const_iterator& it) { node = it.node; return *this; }

        const_iterator& operator++() { node++; return *this; }
        const_iterator operator++(int) { const_iterator tmp = *this; ++(*this); return tmp; }
        value_type operator*() const { return *node; }
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
    ~list() { delete []m_data; }

    list(const list& other);
    list& operator=(const list& other);

    T& front() { return *begin(); }
    const T& front() const { return *begin(); }

    T& back() { return *std::prev(end()); }
    const T& back() const { return *std::prev(end()); }


    iterator begin() const { return iterator(m_data); }
    iterator end() const { return iterator(m_data + _size); }

    const_iterator cbegin() const { return const_iterator(m_data); }
    const_iterator cend() const { return const_iterator(m_data + _size); }

    reverse_iterator rbegin() const;
    reverse_iterator rend() const;

    const_reverse_iterator crbegin() const;
    const_reverse_iterator crend() const;


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

    // Your code goes here?..

 private:

    size_t _size;
    T* m_data;

};

    template<class T>
    list<T>::list() {
        m_data = new T[0];
        _size = 0;
    }

    template<class T>
    list<T>::list(size_t count, const T &value) : _size(count) {
        m_data = new T[count];
        std::fill(begin(), end(), value);
    }

    template<class T>
    list<T>::list(size_t count) : _size(count) {
        m_data = new T[count];
    }

    template<class T>
    class list<T>::iterator list<T>::insert(list::const_iterator pos, const T &value) {
        T* temp = new T[_size + 1];
        std::copy(cbegin(), pos, iterator(temp));
        iterator new_pos = iterator(temp + std::distance(cbegin(), pos));
        *new_pos = value;
        std::copy(pos, cend(), std::next(new_pos, 1));
        ++_size;
        delete []m_data;
        m_data = temp;
        return iterator(m_data);
    }

    template<class T>
    void list<T>::push_back(const T &value) {
        this->insert(cend(), value);
    }

    template<class T>
    void list<T>::push_front(const T &value) {
        this->insert(cbegin(), value);
    }

    template<class T>
    class list<T>::iterator list<T>::erase(const_iterator pos) {
        T* temp = new T[_size - 1];
        std::copy(cbegin(), pos, iterator(temp));
        std::copy(std::next(pos), cend(), iterator(temp + std::distance(cbegin(), pos)));
        --_size;
        delete []m_data;
        m_data = temp;
        return end();
    }

    template<class T>
    void list<T>::pop_front() {
        this->erase(cbegin());
    }

    template<class T>
    void list<T>::pop_back() {
        this->erase(std::prev(cend()));
    }

    template<class T>
    void list<T>::clear() {
        m_data = new T[0];
        _size = 0;
    }

    template<class T>
    void list<T>::resize(size_t count) {
        T* temp = new T[count];
        std::copy(begin(), end(), iterator(temp));
        _size = count;
        delete []m_data;
        m_data = temp;
    }

    template<class T>
    class list<T>::iterator list<T>::insert(list::const_iterator pos, size_t count, const T &value) {
        T* temp = new T[_size + count];
        std::copy(cbegin(), pos, iterator(temp));
        iterator new_pos = iterator(temp + std::distance(cbegin(), pos));
        std::fill(new_pos, std::next(new_pos, count), value);
        std::copy(pos, cend(), std::next(new_pos, count));
        _size += count;
        delete []m_data;
        m_data = temp;
        return iterator(m_data);
    }

    template<class T>
    class list<T>::iterator list<T>::erase(list::const_iterator first, list::const_iterator last) {
        T* temp = new T[_size - std::distance(first, last)];
        std::copy(cbegin(), first, iterator(temp));
        std::copy(last, cend(), iterator(temp + std::distance(cbegin(), first)));
        _size -= std::distance(first, last);
        delete []m_data;
        m_data = temp;
        return end();
    }

}  // namespace task
