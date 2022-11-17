#pragma once

#include <initializer_list>
#include <exception>
#include <iterator>

template<class T>
class DoubleList
{
    struct Node
    {
        T _data;
        Node* _prev;
        Node* _next;
        Node(Node* prev, Node* next, const T& data);
    };

    Node* _head;
    Node* _tail;
    size_t _size;

    void swap(DoubleList<T>& other);

public:
	DoubleList();
    DoubleList(size_t size, const T& value);
    DoubleList(size_t size);
    DoubleList(const std::initializer_list<T>& list);
    DoubleList(const DoubleList<T>& other);
    DoubleList(DoubleList<T>&& other);
	~DoubleList();
    DoubleList<T>& operator=(const DoubleList<T>& other);
    DoubleList<T>& operator=(DoubleList<T>&& other);

    void push_back(const T& value);
    void push_front(const T& value);
    void pop_back();
    void pop_front();
    

    size_t size() const;

    struct ListIterator : private std::iterator<std::bidirectional_iterator_tag, T>
    {
        Node* _itPtr;

        ListIterator(Node* ptr = nullptr) :_itPtr{ ptr } {}
        ListIterator(const ListIterator& other) = default;
        ListIterator(ListIterator&& other) = default; 
        ListIterator& operator=(const ListIterator& other) = default; 
        ListIterator& operator=(ListIterator&& other) = default;
        ~ListIterator() = default;

        T& operator*();
        ListIterator& operator++();
        ListIterator& operator--();
        ListIterator operator++(int);
        ListIterator operator--(int);
        bool operator==(const ListIterator& other);
        bool operator!=(const ListIterator& other);

    };

    ListIterator begin() const;
    ListIterator end() const;
    ListIterator find(const T& value);
    void erase(const ListIterator& iter);
    void insert(const ListIterator& iter, const T& value);
};
template<class T>
void DoubleList<T>::swap(DoubleList<T>& other)
{
    std::swap(_size, other._size);
    std::swap(_head, other._head);
    std::swap(_tail, other._tail);
}

template<class T>
DoubleList<T>::DoubleList():_head { nullptr }, _tail{ nullptr }, _size{ 0 }{}

template<class T>
DoubleList<T>::DoubleList(size_t size, const T& value) : DoubleList()
{
    while (_size < size)
    {
        push_back(value);

    }
}

template<class T>
DoubleList<T>::DoubleList(size_t size) : DoubleList(size, T()){}

template<class T>
DoubleList<T>::DoubleList(const std::initializer_list<T>& list) : DoubleList()
{
    for (auto& element : list)
    {
        push_back(element);
    }
}

template<class T>
DoubleList<T>::DoubleList(const DoubleList<T>& other)
{
    Node* current = other._head;
    for (size_t i = 0; i < other.size(); i++)
    {
        push_back(current->_data);
        current = current->_next;
    }
}

template<class T>
DoubleList<T>::DoubleList(DoubleList<T>&& other)
{
    swap(other);
}

template<class T>
DoubleList<T>::~DoubleList()
{
    Node* current = _head;
    while (current != nullptr)
    {
        _head = _head->_next;
        delete current;
        current = _head;
    }
    _tail = nullptr;
}

template<class T>
DoubleList<T>& DoubleList<T>::operator=(const DoubleList<T>& other)
{
    DoubleList<T> copy(other);
    swap(copy);
    return *this;
}

template<class T>
DoubleList<T>& DoubleList<T>::operator=(DoubleList<T>&& other)
{
    swap(other);
    return *this;
}

template<class T>
void DoubleList<T>::push_back(const T& value)
{
    if (_head == nullptr)
    {
        _head = _tail = new Node(nullptr, nullptr, value);
    }
    else
    {
        _tail = _tail->_next = new Node(_tail, nullptr, value);
    }
    ++_size;
}

template<class T>
void DoubleList<T>::push_front(const T& value)
{
    if (_head == nullptr)
    {
        _head = _tail = new Node(nullptr, nullptr, value);
    }
    else
    {
        _head = _head->_prev = new Node(nullptr, _head, value);
    }
    ++_size;
}

template<class T>
void DoubleList<T>::pop_back()
{
    if (_size == 0)
    {
        throw std::exception("list is empty");
    }
    else if (_size == 1)
    {
        delete _tail;
        _head = _tail = nullptr;
        --_size;
    }
    else
    {
        _tail = _tail->_prev;
        delete _tail->_next;
        _tail->_next = nullptr;
        --_size;
    }

}

template<class T>
void DoubleList<T>::pop_front()
{
    if (_size <= 1)
    {
        pop_back();
    }
    else
    {
        _head = _head->_next;
        delete _head->_prev;
        _head->_prev = nullptr;
        --_size;
    }
}

template<class T>
size_t DoubleList<T>::size() const
{
    return _size;
}

template<class T>
typename DoubleList<T>::ListIterator DoubleList<T>::begin() const
{
    return ListIterator(_head);
}

template<class T>
typename DoubleList<T>::ListIterator DoubleList<T>::end() const
{
    return ListIterator(nullptr);
}

template<class T>
typename DoubleList<T>::ListIterator DoubleList<T>::find(const T& value)
{
    for (auto i = begin(); i != end(); i++)
    {
        if (*i == value)
        {
            return i;
        }

    }
    return end();
}

template<class T>
void DoubleList<T>::erase(const ListIterator& iter)
{
    if (iter._itPtr == _head || _size == 0)
    {
        pop_front();
    }
    else if (iter._itPtr == _tail)
    {
        pop_back();
    }
    else
    {
        iter._itPtr->_prev->_next = iter._itPtr->_next;
        iter._itPtr->_next->_prev = iter._itPtr->_prev;
        delete iter._itPtr;
        --_size;
    }
}

template<class T>
void DoubleList<T>::insert(const ListIterator& iter, const T& value)
{
    if (iter._itPtr == _head || _size == 0)
    {
        push_front(value);
    }
    else if (iter._itPtr == _tail)
    {
        push_back(value);
    }
    else
    {
        iter._itPtr->_prev = iter._itPtr->_prev->_next = new Node(iter._itPtr->_prev, iter._itPtr, value);
        ++_size;
    }

}


template<class T>
DoubleList<T>::Node::Node(Node* prev, Node* next, const T& data):_prev{ prev }, _next{ next }, _data{ data } {}

template<class T>
T& DoubleList<T>::ListIterator::operator*()
{
    return _itPtr->_data;
}

template<class T>
typename DoubleList<T>::ListIterator& DoubleList<T>::ListIterator::operator++()
{
    _itPtr = _itPtr->_next;
    return *this;
}

template<class T>
typename DoubleList<T>::ListIterator& DoubleList<T>::ListIterator::operator--()
{
    _itPtr = _itPtr->_prev;
    return *this;
}

template<class T>
typename DoubleList<T>::ListIterator DoubleList<T>::ListIterator::operator++(int)
{
    ListIterator copy(_itPtr);
    _itPtr = _itPtr->_next;
    return copy;
}

template<class T>
typename DoubleList<T>::ListIterator DoubleList<T>::ListIterator::operator--(int)
{
    ListIterator copy(_itPtr);
    _itPtr = _itPtr->_prev;
    return copy;
}

template<class T>
bool DoubleList<T>::ListIterator::operator==(const ListIterator& other)
{
    return _itPtr == other._itPtr;
}

template<class T>
bool DoubleList<T>::ListIterator::operator!=(const ListIterator& other)
{
    return _itPtr != other._itPtr;
}
