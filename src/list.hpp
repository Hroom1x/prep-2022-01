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

        iterator(pointer ptr) : node(ptr) { };
        iterator(const iterator& it) : node(it.node) { };
        iterator& operator=(const iterator& it) { node = it.node; return *this; }

        iterator& operator++() { node++; return *this; }
        iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }
        reference operator*() const { return *node; }
        pointer operator->() const { return node; }
        iterator& operator--() { node--; return *this; }
        iterator operator--(int) { iterator tmp = *this; --(*this); return tmp; }

        bool operator==(iterator other) const { return this->node == other.node; }
        bool operator!=(iterator other) const { return this->node != other.node; }

     private:
        pointer node;
    };

    class const_iterator : iterator {
        // Your code goes here...
    };

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;


    list() = default;
    list(size_t count, const T& value);
    explicit list(size_t count);
    ~list();

    list(const list& other);
    list& operator=(const list& other);

    T& front();
    const T& front() const;

    T& back();
    const T& back() const;


    iterator begin() const { return iterator(m_data); }
    iterator end() const { return iterator(m_data + _size); }

    const_iterator cbegin() const;
    const_iterator cend() const;

    reverse_iterator rbegin() const;
    reverse_iterator rend() const;

    const_reverse_iterator crbegin() const;
    const_reverse_iterator crend() const;


    bool empty() const { return _size == 0; }
    size_t size() const { return _size; }
    size_t max_size() const { return _size; }
    void clear();

    iterator insert(iterator pos, const T& value);
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
    list<T>::list(size_t count, const T &value) : _size(count) {
        m_data = new T[count];
        std::fill(begin(), end(), value);
    }

    template<class T>
    list<T>::list(size_t count) : _size(count) {
        m_data = new T[count];
    }

    template<class T>
    class list<T>::iterator list<T>::insert(list::iterator pos, const T &value) {
        T* temp = new T[++_size];
        std::move(begin(), pos, iterator(temp));
        iterator new_pos = iterator(temp + std::distance(begin(), pos));
        *new_pos = value;
        std::move(pos, end(), std::next(new_pos, 1));
        delete []m_data;
        m_data = temp;
        return new_pos;
    }

    template<class T>
    void list<T>::push_back(const T &value) {
        this->insert(end(), value);
    }

}  // namespace task
