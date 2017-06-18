// Copyright (c) 2017, Danilo Peixoto. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef EXPRESSIO_QUEUE_H
#define EXPRESSIO_QUEUE_H

#include "global.h"
#include "types.h"
#include <initializer_list>
#include <sstream>
#include <string>

EXPRESSIO_NAMESPACE_BEGIN

template<typename T>
class Queue {
public:
    struct Node {
        T data;
        Node * prev, *next;

        Node();
        Node(const T &, Node * = EXPRESSIO_NULL, Node * = EXPRESSIO_NULL);
        ~Node();
    };

    typedef Node * NodePointer;

    class Iterator {
    public:
        NodePointer pointer;

        Iterator();
        Iterator(const Iterator &);
        Iterator(NodePointer);
        ~Iterator();

        T & operator *();
        T * operator ->();
        Iterator & operator ++();
        Iterator operator ++(int);
        Iterator & operator --();
        Iterator operator --(int);
        Bool operator ==(const Iterator &);
        Bool operator !=(const Iterator &);
    };

    class ConstIterator {
    public:
        NodePointer pointer;

        ConstIterator();
        ConstIterator(const ConstIterator &);
        ConstIterator(NodePointer);
        ~ConstIterator();

        const T & operator *();
        const T * operator ->();
        ConstIterator & operator ++();
        ConstIterator operator ++(int);
        ConstIterator & operator --();
        ConstIterator operator --(int);
        Bool operator ==(const ConstIterator &);
        Bool operator !=(const ConstIterator &);
    };

    Queue();
    Queue(const Queue &);
    Queue(const std::initializer_list<T> &);
    Queue(UInt, const T *);
    ~Queue();

    UInt getSize() const;
    Iterator getBegin();
    Iterator getEnd();
    ConstIterator getBegin() const;
    ConstIterator getEnd() const;
    Bool isEmpty() const;
    std::string toString() const;

    Queue & insert(const T &);
    Bool remove();
    Queue & clear();

private:
    UInt size;
    NodePointer begin, end;
};

template<typename T>
Queue<T>::Node::Node() : prev(EXPRESSIO_NULL), next(EXPRESSIO_NULL) {}
template<typename T>
Queue<T>::Node::Node(const T & data, Node * prev, Node * next)
    : data(data), prev(prev), next(next) {}
template<typename T>
Queue<T>::Node::~Node() {}

template<typename T>
Queue<T>::Iterator::Iterator() : pointer(EXPRESSIO_NULL) {};
template<typename T>
Queue<T>::Iterator::Iterator(const Iterator & iterator) : pointer(iterator.pointer) {};
template<typename T>
Queue<T>::Iterator::Iterator(NodePointer pointer) : pointer(pointer) {};
template<typename T>
Queue<T>::Iterator::~Iterator() {};

template<typename T>
T & Queue<T>::Iterator::operator *() {
    return pointer->data;
}
template<typename T>
T * Queue<T>::Iterator::operator ->() {
    return (T *)pointer;
}
template<typename T>
typename Queue<T>::Iterator & Queue<T>::Iterator::operator ++() {
    pointer = pointer->next;

    return *this;
}
template<typename T>
typename Queue<T>::Iterator Queue<T>::Iterator::operator ++(int) {
    Iterator it(*this);

    pointer = pointer->next;

    return it;
}
template<typename T>
typename Queue<T>::Iterator & Queue<T>::Iterator::operator --() {
    pointer = pointer->prev;

    return *this;
}
template<typename T>
typename Queue<T>::Iterator Queue<T>::Iterator::operator --(int) {
    Iterator it(*this);

    pointer = pointer->prev;

    return it;
}
template<typename T>
Bool Queue<T>::Iterator::operator ==(const Iterator & rhs) {
    return pointer == rhs.pointer;
}
template<typename T>
Bool Queue<T>::Iterator::operator !=(const Iterator & rhs) {
    return !(*this == rhs);
}

template<typename T>
Queue<T>::ConstIterator::ConstIterator() : pointer(EXPRESSIO_NULL) {};
template<typename T>
Queue<T>::ConstIterator::ConstIterator(const ConstIterator & constIterator)
    : pointer(constIterator.pointer) {};
template<typename T>
Queue<T>::ConstIterator::ConstIterator(NodePointer pointer) : pointer(pointer) {};
template<typename T>
Queue<T>::ConstIterator::~ConstIterator() {};

template<typename T>
const T & Queue<T>::ConstIterator::operator *() {
    return pointer->data;
}
template<typename T>
const T * Queue<T>::ConstIterator::operator ->() {
    return (const T *)pointer;
}
template<typename T>
typename Queue<T>::ConstIterator & Queue<T>::ConstIterator::operator ++() {
    pointer = pointer->next;

    return *this;
}
template<typename T>
typename Queue<T>::ConstIterator Queue<T>::ConstIterator::operator ++(int) {
    ConstIterator it(*this);

    pointer = pointer->next;

    return it;
}
template<typename T>
typename Queue<T>::ConstIterator & Queue<T>::ConstIterator::operator --() {
    pointer = pointer->prev;

    return *this;
}
template<typename T>
typename Queue<T>::ConstIterator Queue<T>::ConstIterator::operator --(int) {
    ConstIterator it(*this);

    pointer = pointer->prev;

    return it;
}
template<typename T>
Bool Queue<T>::ConstIterator::operator ==(const ConstIterator & rhs) {
    return pointer == rhs.pointer;
}
template<typename T>
Bool Queue<T>::ConstIterator::operator !=(const ConstIterator & rhs) {
    return !(*this == rhs);
}

template<typename T>
Queue<T>::Queue() : size(0), begin(EXPRESSIO_NULL), end(EXPRESSIO_NULL) {};
template<typename T>
Queue<T>::Queue(const Queue & queue) {
    size = 0;
    begin = EXPRESSIO_NULL;
    end = EXPRESSIO_NULL;

    ConstIterator it(queue.getBegin());

    while (it != queue.getEnd())
        insert(*it++);
}
template<typename T>
Queue<T>::Queue(const std::initializer_list<T> & list) {
    this->size = 0;
    begin = EXPRESSIO_NULL;
    end = EXPRESSIO_NULL;

    const T * it = list.begin();

    while (it != list.end())
        insert(*it++);
}
template<typename T>
Queue<T>::Queue(UInt size, const T * elements) {
    this->size = 0;
    begin = EXPRESSIO_NULL;
    end = EXPRESSIO_NULL;

    for (UInt i = 0; i < size; i++)
        insert(elements[i]);
}
template<typename T>
Queue<T>::~Queue() {
    clear();
}

template<typename T>
UInt Queue<T>::getSize() const {
    return size;
}
template<typename T>
typename Queue<T>::Iterator Queue<T>::getBegin() {
    return Iterator(begin);
}
template<typename T>
typename Queue<T>::Iterator Queue<T>::getEnd() {
    return Iterator(EXPRESSIO_NULL);
}
template<typename T>
typename Queue<T>::ConstIterator Queue<T>::getBegin() const {
    return ConstIterator(begin);
}
template<typename T>
typename Queue<T>::ConstIterator Queue<T>::getEnd() const {
    return ConstIterator(EXPRESSIO_NULL);
}
template<typename T>
Bool Queue<T>::isEmpty() const {
    return begin == EXPRESSIO_NULL;
}
template<typename T>
std::string Queue<T>::toString() const {
    std::stringstream sstream;

    if (begin != EXPRESSIO_NULL) {
        ConstIterator it(begin);

        sstream << "[";

        while (it != getEnd()) {
            sstream << *it++;

            if (it != getEnd())
                sstream << ", ";
        }

        sstream << "]";
    }

    return sstream.str();
}

template<typename T>
Queue<T> & Queue<T>::insert(const T & element) {
    NodePointer node = new Node;

    node->data = element;
    node->prev = end;
    node->next = EXPRESSIO_NULL;

    if (end != EXPRESSIO_NULL)
        end->next = node;
    else
        begin = node;

    end = node;

    size++;

    return *this;
}
template<typename T>
Bool Queue<T>::remove() {
    if (begin == EXPRESSIO_NULL)
        return false;

    NodePointer node = begin;
    begin = node->next;

    if (begin != EXPRESSIO_NULL)
        begin->prev = EXPRESSIO_NULL;
    else
        end = EXPRESSIO_NULL;

    delete node;

    size--;

    return true;
}
template<typename T>
Queue<T> & Queue<T>::clear() {
    while (remove());

    return *this;
}

EXPRESSIO_NAMESPACE_END

#endif