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

        iterator() = default;
        iterator(const iterator& other) : _node(other._node), _next(other._next), _prev(other._prev) { };
        iterator& operator=(const iterator& other) { if (this != &other) this(other); return *this; }

        iterator& operator++() { this = _next; return *this; }
        iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }
        reference operator*() const { return *_node; }
        pointer operator->() const { return _node; }
        iterator& operator--() { this = _prev; return *this; }
        iterator operator--(int) { iterator tmp = *this; --(*this); return tmp; }

        bool operator==(iterator other) const { return this->_node == other._node; }
        bool operator!=(iterator other) const { return this->_node != other._node; }

     private:
        pointer _node;
        iterator _next;
        iterator _prev;
    };

    class const_iterator {
     public:
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = const T*;
        using reference = const T&;
        using iterator_category = std::bidirectional_iterator_tag;

        const_iterator() = default;
        explicit const_iterator(const iterator& other) : _node(other._node), _next(other._next), _prev(other._prev) { };
        const_iterator(const const_iterator& other) : _node(other._node), _next(other._next), _prev(other._prev) { };
        const_iterator& operator=(const const_iterator& other) { if (this != &other) this(other); return *this; }

        const_iterator& operator++() { this = _next; return *this; }
        const_iterator operator++(int) { const_iterator tmp = *this; ++(*this); return tmp; }
        reference operator*() const { return *_node; }
        pointer operator->() const { return _node; }
        const_iterator& operator--() { this = _prev; return *this; }
        const_iterator operator--(int) { const_iterator tmp = *this; --(*this); return tmp; }

        bool operator==(const_iterator other) const { return this->_node == other._node; }
        bool operator!=(const_iterator other) const { return this->_node != other._node; }

     private:
        pointer _node;
        iterator _next;
        iterator _prev;
    };

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;


    list();
    list(size_t count, const T& value);
    explicit list(size_t count);
    ~list();

    list(const list& other);
    list& operator=(const list& other);

    T& front() { return *begin(); }
    const T& front() const { return *begin(); }

    T& back() { return *std::prev(end()); }
    const T& back() const { return *std::prev(cend()); }


    iterator begin() const { return _first; }
    iterator end() const { return std::next(_first, _size); }

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
    iterator _first;
    iterator _last;

};

    template<class T>
    list<T>::list() {
        _size = 0;
    }

    template<class T>
    list<T>::list(size_t count, const T &value) : _size(count) {
        m_data = new T[count];
        std::fill_n(begin(), count, value);
    }

    template<class T>
    list<T>::list(size_t count) : _size(count) {
        m_data = new T[count];
        std::uninitialized_default_construct_n(begin(), count);
    }

    template<class T>
    list<T>::list(const list &other) {
        if (&other != this) {
            _size = other._size;
            m_data = new T[size()];
            std::copy_n(other.m_data, size(), m_data);
        }
    }

    template<class T>
    class list<T>& list<T>::operator=(const list &other)  {
        if (&other != this) {
            list<T> temp(other);
            this->swap(temp);
        }
        return *this;
    }

    template<class T>
    list<T>::~list() {
        //
    }
/*
    template<class T>
    class list<T>::iterator list<T>::insert(list::const_iterator pos, const T &value) {
        T* temp = new T[size() + 1];
        std::copy(cbegin(), pos, iterator(temp));
        iterator new_pos = iterator(temp + std::distance(cbegin(), pos));
        *new_pos = value;
        //std::fill_n(new_pos, 1, value);
        std::copy(pos, cend(), std::next(new_pos, 1));
        ++_size;
        delete[] m_data;
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
        T* temp = new T[size() - 1];
        std::copy(cbegin(), pos, iterator(temp));
        std::copy(std::next(pos), cend(), iterator(temp + std::distance(cbegin(), pos)));
        --_size;
        delete[] m_data;
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
        delete[] m_data;
        m_data = new T[0];
        _size = 0;
    }

    template<class T>
    void list<T>::resize(size_t count) {
        T* temp = new T[count];
        if (count >= _size) {
            std::copy(begin(), end(), iterator(temp));
        } else {
            std::copy_n(begin(), count, iterator(temp));
        }
        _size = count;
        delete[] m_data;
        m_data = temp;
    }

    template<class T>
    class list<T>::iterator list<T>::insert(list::const_iterator pos, size_t count, const T &value) {
        T* temp = new T[size() + count];
        std::copy(cbegin(), pos, iterator(temp));
        iterator new_pos = iterator(temp + std::distance(cbegin(), pos));
        std::fill(new_pos, std::next(new_pos, count), value);
        std::copy(pos, cend(), std::next(new_pos, count));
        _size += count;
        delete[] m_data;
        m_data = temp;
        return iterator(m_data);
    }

    template<class T>
    class list<T>::iterator list<T>::erase(list::const_iterator first, list::const_iterator last) {
        T* temp = new T[size() - std::distance(first, last)];
        std::copy(cbegin(), first, iterator(temp));
        std::copy(last, cend(), iterator(temp + std::distance(cbegin(), first)));
        _size -= std::distance(first, last);
        delete[] m_data;
        m_data = temp;
        return end();
    }
*/
    //template<class T>
    //void list<T>::sort() {
    //    iterator _first = begin();
    //    iterator _last = end();
    //    // Bubble sort
    //    for (iterator it1 = _first; it1 != _last; ++it1) {
    //        for (iterator it2 = _first; it2 != std::prev(_last); ++it2) {
    //            if (*it2 > *std::next(it2)) {
    //                std::iter_swap(it2, std::next(it2));
    //            }
    //        }
    //    }
    //}

    //template<class T>
    //void list<T>::reverse() {
    //    std::reverse(begin(), end());
    //}


/*
    template<class T>
    void list<T>::splice(list::const_iterator pos, list &other) {
        T* temp = new T[size() + other.size()];
        std::copy(cbegin(), pos, iterator(temp));
        std::copy(other.cbegin(), other.cend(), std::next(iterator(temp), std::distance(cbegin(), pos)));
        std::copy(pos, cend(), std::next(iterator(temp), std::distance(cbegin(), pos) + other._size));
        _size += other.size();
        other.clear();
        delete[] m_data;
        m_data = temp;
    }

    template<class T>
    void list<T>::unique() {
        iterator new_end = std::unique(begin(), end());
        // resize(std::distance(begin(), new_end));
        std::destroy(new_end, end());
        _size = std::distance(begin(), new_end);
    }

    template<class T>
    void list<T>::remove(const T &value) {
        size_t count = 0;
        for (iterator it = --end(); it != --begin(); --it) {
            if (*it == value) {
                std::move(std::next(it), end(), it);
                count++;
            }
        }
        resize(size() - count);
    }

    template<class T>
    void list<T>::merge(list &other) {
        T* temp = new T[size() + other.size()];
        std::merge(begin(), end(), other.begin(), other.end(), temp);
        delete[] m_data;
        m_data = temp;
        _size += other.size();
        other.clear();
    }

    template<class T>
    void list<T>::swap(list &other) {
        size_t temp_size = other.size();
        other._size = size();
        _size = temp_size;

        T* temp_ptr = m_data;
        m_data = other.m_data;
        other.m_data = temp_ptr;
    }
*/

}  // namespace task
