#pragma once // NOLINT
#include <iterator>
#include <algorithm>


namespace task {


template<class T>
class list {
 public:
    struct node {
        T* _value;
        node* _next;
        node* _prev;

        void hook(node* other) {
            if (this == other) {
                _next = this;
                _prev = this;
            }
            node* prev_node = _prev;
            prev_node->_next = other;
            other->_prev = prev_node;

            _prev = other;
            other->_next = this;
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

    class const_iterator;

    class iterator {
     public:
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::bidirectional_iterator_tag;

        iterator() = default;
        explicit iterator(node* _x) : _node(_x) { }
        iterator(const iterator& other) : _node(other._node) { }
        iterator& operator=(const iterator& other) {
            if (&other != this) {
                _node = other._node;
            }
            return *this;
        }

        iterator& operator++() { _node = _node->_next; return *this; }
        iterator operator++(int) { iterator temp = *this; _node = _node->_next; return temp; }
        reference operator*() const { return *_node->_value; }
        pointer operator->() const { return _node->_value; }
        iterator& operator--() { _node = _node->_prev; return *this; }
        iterator operator--(int) { iterator temp = *this; _node = _node->_next; return temp; }

        operator const_iterator() const { return const_iterator(*this); }

        bool operator==(iterator other) const { return this->_node == other._node; }
        bool operator!=(iterator other) const { return this->_node != other._node; }

     private:
        friend list;
        node* _node{};
    };

    class const_iterator {
     public:
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = const T*;
        using reference = const T&;
        using iterator_category = std::bidirectional_iterator_tag;

        const_iterator() = default;
        explicit const_iterator(node* _x) : _node(_x) { }
        const_iterator(const const_iterator& other) : _node(other._node) { }
        explicit const_iterator(const iterator& other) : _node(other._node) {  }
        const_iterator& operator=(const const_iterator& other) {
            if (&other != this) {
                _node = other._node;
            }
            return *this;
        }

        iterator _const_cast() const { return iterator(const_cast<node*>(_node)); }

        const_iterator& operator++() { _node = _node->_next; return *this; }
        const_iterator operator++(int) { const_iterator temp = *this; _node = _node->_next; return temp; }
        reference operator*() const { return *_node->_value; }
        pointer operator->() const { return _node->_value; }
        const_iterator& operator--() { _node = _node->_prev; return *this; }
        const_iterator operator--(int) { const_iterator temp = *this; _node = _node->_prev; return temp; }

        bool operator==(const_iterator other) const { return this->_node == other._node; }
        bool operator!=(const_iterator other) const { return this->_node != other._node; }

     private:
        friend list;
        const node* _node{};
    };

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;


    list() = default;
    list(size_t count, const T& value);
    explicit list(size_t count);
    ~list();

    list(const list& other);
    list& operator=(const list& other);

    T& front() { return *begin(); }
    const T& front() const { return *begin(); }

    T& back() { return *std::prev(end()); }
    const T& back() const { return *std::prev(end()); }


    iterator begin() const { return iterator(_first); }
    iterator end() const { return (_first != nullptr) ? iterator(_first->_prev) : (begin()); }

    const_iterator cbegin() const { return const_iterator(begin()); }
    const_iterator cend() const { return const_iterator(end()); }

    reverse_iterator rbegin() const { return std::make_reverse_iterator(end()); }
    reverse_iterator rend() const { return std::make_reverse_iterator(begin()); }

    const_reverse_iterator crbegin() const { return std::make_reverse_iterator(end()); }
    const_reverse_iterator crend() const { return std::make_reverse_iterator(begin()); }


    bool empty() const { return _size == 0; }
    size_t size() const { return _size; }
    static size_t max_size() { return std::numeric_limits<typename iterator::difference_type>::max(); }
    void clear();

    iterator insert(const_iterator pos);
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
    void reverse() const;
    void unique();
    void sort() const;

 private:
    node* create_node(const T& value = T()) {
        node* temp = new node;
        temp->_value = new T;
        *temp->_value = std::move(value);
        return temp;
    }

    size_t _size{0};
    node* _first{};
};

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
    class list<T>::iterator list<T>::insert(list::const_iterator pos) {
        if constexpr (!std::is_default_constructible<T>::value) {
            throw std::invalid_argument("Type has no default constructor");
        } else {
            return insert(pos, T());
        }
    }

    template<class T>
    class list<T>::iterator list<T>::insert(list::const_iterator pos, const T &value) {
        node* temp = create_node(value);
        ++_size;
        if (pos._node == nullptr) {
            _first = std::move(temp);
            node* _last = create_node(value);
            _first->hook(_first);
            _first->hook(_last);
            return iterator(_first);
        }

        pos._const_cast()._node->hook(temp);  // Вставить узел temp перед узлом по итератору pos

        if (pos == cbegin())
            _first = temp;  // Вставленный элемент становится первым
        return iterator(temp);
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
        if (empty())
            return begin();
        node* next_node = pos._const_cast()._node->_next;
        node* curr_node = pos._const_cast()._node->unhook();
        delete curr_node;
        --_size;
        if (pos == cbegin())
            _first = next_node;
        if (empty()) {
            delete next_node;
            _first = nullptr;
            next_node = _first;
        }
        return iterator(next_node);
    }

    template<class T>
    class list<T>::iterator list<T>::erase(list::const_iterator first, list::const_iterator last) {
        if (empty())
            return begin();
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
    void list<T>::sort() const {
        iterator first_it = begin();
        iterator last_it = end();
        iterator pos = last_it;
        iterator it1 = first_it;
        iterator it2 = first_it;
        // Bubble sort
        for (; it1 != last_it; ++it1) {
            --pos;
            for (it2 = first_it; it2 != pos; ++it2) {
                // Very slow (~0.5x) because of implicit temp values in std::next()
                // if (*it2 > *std::next(it2))
                if (*it2 > *it2._node->_next->_value) {
                    std::swap(it2._node->_value, it2._node->_next->_value);
                }
            }
        }
    }

    template<class T>
    void list<T>::swap(list &other) {
        size_t temp_size = other.size();
        other._size = size();
        _size = temp_size;

        node* temp = std::move(_first);
        _first = std::move(other._first);
        other._first = std::move(temp);
    }

    template<class T>
    void list<T>::push_front(const T &value) {
        insert(cbegin(), value);
    }

    template<class T>
    void list<T>::clear() {
        while (!empty())
            pop_front();
    }

    template<class T>
    void list<T>::resize(size_t count) {
        while (count < size())
            erase(cend());
        while (count > size())
            insert(cend());
    }

    template<class T>
    void list<T>::reverse() const {
        std::reverse(begin(), end());
    }

    template<class T>
    void list<T>::unique() {
        iterator new_end = std::unique(begin(), end());
        resize(std::distance(begin(), new_end));
    }

    template<class T>
    void list<T>::splice(list::const_iterator pos, list &other) {
        if (!other.empty()) {
            node *pos_prev = pos._const_cast()._node->_prev;

            pos_prev->_next = other.begin()._node;

            node *other_end_prev = other.end()._node->_prev;  // Узел последнего значащего элемента
            node *other_end = std::move(other.end()._node);   // Последний "псевдоузел"
            delete other_end;

            pos._const_cast()._node->_prev = other_end_prev;
            other_end_prev->_next = pos._const_cast()._node;
            other.begin()._node->_prev = pos_prev;

            _size += other.size();

            other._first = nullptr;
            other._size = 0;
        }
    }

    template<class T>
    void list<T>::remove(const T &value) {
        const_iterator it = cbegin();
        // Копируем value, т.к. ссылка может указывать на стёртый элемент в дальнейшем
        T copied_value = value;
        while (it != cend()) {
            if (*it == copied_value)
                it = erase(it);
            else
                ++it;
        }
    }

    template<class T>
    void list<T>::merge(list &other) {
        auto dist = std::distance(begin(), end());
        this->splice(cend(), other);
        std::inplace_merge(begin(), std::next(begin(), dist), end());
    }


}  // namespace task
