#pragma once // NOLINT
#include <iterator>


namespace task {


template<class T>
class list {
 public:
    struct node {
        T* _value;
        node* _next;
        node* _prev;

        void hook(node& other) {
            node* prev_node = _prev;
            prev_node->_next = &other;
            other._prev = prev_node;

            _prev = &other;
            other._next = this;
        }

        node* unhook() {
            node* prev_node = _prev;
            node* next_node = _next;
            prev_node->_next = next_node;
            next_node->_prev = prev_node;
            _prev = nullptr;
            _next = nullptr;
            return this;
        }

        ~node() {
            delete _value;
        }
    };

    class iterator {
    public:
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::bidirectional_iterator_tag;

        iterator() : _node() { }
        explicit iterator(node& _x) : _node(&_x) { }
        iterator(const iterator& other) : _node(other._node) { }
        iterator& operator=(const iterator& other) { _node = other._node; return *this; }

        iterator& operator++() { _node = _node->_next; return *this; }
        iterator operator++(int) { iterator temp = *this; _node = _node->_next; return temp; }
        reference operator*() const { return *_node->_value; }
        pointer operator->() const { return _node->_value; }
        iterator& operator--() { _node = _node->_prev; return *this; }
        iterator operator--(int) { iterator temp = *this; _node = _node->_next; return temp; }

        bool operator==(iterator other) const { return this->_node == other._node; }
        bool operator!=(iterator other) const { return this->_node != other._node; }

        node* _node;

     private:
        //
    };

    class const_iterator {
     public:
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = const T*;
        using reference = const T&;
        using iterator_category = std::bidirectional_iterator_tag;

        const_iterator() : _node() { }
        const_iterator(const const_iterator& other) : _node(other._node) { }
        explicit const_iterator(const iterator& other) : _node(other._node) {  }
        const_iterator& operator=(const const_iterator& other) { _node = other._node; return *this; }

        iterator _const_cast() const { return iterator(*const_cast<node*>(_node)); }

        const_iterator& operator++() { _node = _node->_next; return *this; }
        const_iterator operator++(int) { const_iterator temp = *this; _node = _node->_next; return temp; }
        reference operator*() const { return *_node->_value; }
        pointer operator->() const { return _node->_value; }
        const_iterator& operator--() { _node = _node->_prev; return *this; }
        const_iterator operator--(int) { const_iterator temp = *this; _node = _node->_prev; return temp; }

        bool operator==(const_iterator other) const { return this->_node == other._node; }
        bool operator!=(const_iterator other) const { return this->_node != other._node; }

     private:
        const node* _node;
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
    const T& back() const { return *std::prev(end()); }


    iterator begin() const { return iterator(*_first); }
    iterator end() const { return iterator(*_first->_prev); }

    const_iterator cbegin() const { return const_iterator(begin()); }
    const_iterator cend() const { return const_iterator(end()); }

    reverse_iterator rbegin() const { return std::make_reverse_iterator(end()); }
    reverse_iterator rend() const { return std::make_reverse_iterator(begin()); }

    const_reverse_iterator crbegin() const { return std::make_reverse_iterator(end()); }
    const_reverse_iterator crend() const { return std::make_reverse_iterator(begin()); }


    bool empty() const { return _size == 0; }
    size_t size() const { return _size; }
    size_t max_size() const { return _alloc.max_size(); }
    void clear();

    iterator insert(const_iterator pos, const T& value = T());
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

    node* create_node(const T& value = T()) {
        node* temp = new node;
        temp->_value = new T;
        *temp->_value = value;
        return temp;
    }

    size_t _size;
    std::allocator<node> _alloc;
    node* _first;

};

    template<class T>
    list<T>::list() {
        _size = 0;
        _first = create_node();
        _first->_next = _first;
        _first->_prev = _first;
    }

    template<class T>
    list<T>::list(size_t count) : list() {
        for (; count; --count)
            this->insert(cend());
    }

    template<class T>
    list<T>::list(size_t count, const T &value) : list(count) {
        if (count != 0) std::fill_n(begin(), count, value);
    }

    template<class T>
    list<T>::list(const list &other) : list(other.size()) {
        if (&other != this) {
            list<T> temp(other.size());
            std::copy_n(other.begin(), size(), temp.begin());
            this->swap(temp);
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
        clear();
        delete _first;
    }

    template<class T>
    class list<T>::iterator list<T>::insert(list::const_iterator pos, const T &value) {
        ++_size;

        node* temp = create_node(value);
        pos._const_cast()._node->hook(*temp);  // Вставить узел temp перед узлом по итератору pos

        if (pos == cbegin())
            _first = temp;
        return iterator(*temp);
    }

    template<class T>
    class list<T>::iterator list<T>::insert(list::const_iterator pos, size_t count, const T &value) {
        if (count == 0)
            return pos._const_cast();
        for (; count > 1; --count)
            insert(pos, value);
        return insert(pos, value);
    }

    template<class T>
    class list<T>::iterator list<T>::erase(list::const_iterator pos) {
        if (_size == 0) return begin();
        --_size;
        node* next_node = std::next(pos)._const_cast()._node;
        node* curr_node = pos._const_cast()._node->unhook();
        delete curr_node;
        if (pos == cbegin())
            _first = next_node;
        return iterator(*next_node);
    }

    template<class T>
    class list<T>::iterator list<T>::erase(list::const_iterator first, list::const_iterator last) {
        if (first == last)
            return first._const_cast();
        const_iterator result = first;
        while (result != last)
            result = const_iterator(erase(result));
        return result._const_cast();
    }

    template<class T>
    void list<T>::push_back(const T &value) {
        insert(cend(), value);
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
    void list<T>::sort() {
        iterator first_it = begin();
        iterator last_it = end();
        // Bubble sort
        for (iterator it1 = first_it; it1 != last_it; ++it1) {
            for (iterator it2 = first_it; it2 != std::prev(last_it); ++it2) {
                if (*it2 > *std::next(it2)) {
                    std::iter_swap(it2, std::next(it2));
                }
            }
        }
    }

    template<class T>
    void list<T>::swap(list &other) {
        size_t temp_size = other.size();
        other._size = size();
        _size = temp_size;

        node* temp = _first;
        _first = other._first;
        other._first = temp;
    }

    template<class T>
    void list<T>::push_front(const T &value) {
        insert(cbegin(), value);
    }

    template<class T>
    void list<T>::clear() {
        while (begin() != end())
            erase(cbegin());
    }

    template<class T>
    void list<T>::resize(size_t count) {
        while (count < size())
            erase(cend());
        while (count > size())
            insert(cend());
    }

    template<class T>
    void list<T>::reverse() {
        std::reverse(begin(), end());
    }

    template<class T>
    void list<T>::unique() {
        iterator new_end = std::unique(begin(), end());
        resize(std::distance(begin(), new_end));
    }

    template<class T>
    void list<T>::splice(list::const_iterator pos, list &other) {
        node* pos_prev = pos._const_cast()._node->_prev;

        pos_prev->_next = other.begin()._node;

        node* other_end_prev = other.end()._node->_prev;
        node* other_end = other.end()._node;
        delete other_end;

        pos._const_cast()._node->_prev = other_end_prev;
        other_end_prev->_next = pos._const_cast()._node;
        other.begin()._node->_prev = pos_prev;

        _size += other.size();

        other._first = create_node();
        other._first->_next = other._first;
        other._first->_prev = other._first;
        other._size = 0;
    }

    template<class T>
    void list<T>::remove(const T &value) {
        //size_t count = 0;
        //for (iterator it = --end(); it != --begin(); --it) {
        //    if (*it == value) {
        //        std::move(std::next(it), end(), it);
        //        count++;
        //    }
        //}
        //resize(size() - count);
    }

    template<class T>
    void list<T>::merge(list &other) {
        //T* temp = new T[size() + other.size()];
        //std::merge(begin(), end(), other.begin(), other.end(), temp);
        //delete[] m_data;
        //m_data = temp;
        //_size += other.size();
        //other.clear();
    }


}  // namespace task
