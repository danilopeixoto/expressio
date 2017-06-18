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

#ifndef EXPRESSIO_TREE_H
#define EXPRESSIO_TREE_H

#include "global.h"
#include "types.h"
#include "queue.h"
#include <initializer_list>
#include <sstream>
#include <string>

EXPRESSIO_NAMESPACE_BEGIN

template<typename T>
class Tree {
public:
    struct Node {
        T data;
        Node * left, *right;

        Node();
        Node(const T &, Node * = EXPRESSIO_NULL, Node * = EXPRESSIO_NULL);
        ~Node();
    };

    typedef Node * NodePointer;

    class Function {
    public:
        Function();
        virtual ~Function();

        virtual void compute(T &) = 0;
    };

    Tree();
    Tree(const Tree &);
    Tree(const Queue<T> &);
    Tree(const std::initializer_list<T> &);
    Tree(UInt, const T *);
    ~Tree();

    UInt getSize() const;
    NodePointer getRoot();
    Tree & setRoot(NodePointer);
    Bool isLeaf() const;
    Bool isEmpty() const;
    UInt height() const;
    UInt leafCount() const;
    std::string toString() const;

    Tree & insert(const T &);
    Bool remove(const T &);
    NodePointer search(const T &);
    Tree & preOrder(Function &);
    Tree & postOrder(Function &);
    Tree & inOrder(Function &);
    Tree & levelOrder(Function &);

    Tree & clear();

private:
    UInt size;
    NodePointer root;

    UInt height(NodePointer) const;
    UInt leafCount(NodePointer) const;
    void format(NodePointer, std::stringstream &,
        std::string, Bool, Bool) const;
    Tree & copy(NodePointer &);
    Tree & insert(NodePointer &, const T &);
    Bool remove(NodePointer &, const T &);
    NodePointer search(NodePointer, const T &) const;
    Tree & preOrder(NodePointer, Function &);
    Tree & postOrder(NodePointer, Function &);
    Tree & inOrder(NodePointer, Function &);
    Tree & destroy(NodePointer &);
};

template<typename T>
Tree<T>::Node::Node() : left(EXPRESSIO_NULL), right(EXPRESSIO_NULL) {}
template<typename T>
Tree<T>::Node::Node(const T & data, Node * left, Node * right)
    : data(data), left(left), right(right) {}
template<typename T>
Tree<T>::Node::~Node() {}

template<typename T>
Tree<T>::Function::Function() {}
template<typename T>
Tree<T>::Function::~Function() {}

template<typename T>
Tree<T>::Tree() : size(0), root(EXPRESSIO_NULL) {}
template<typename T>
Tree<T>::Tree(const Tree & tree) {
    size = 0;

    copy(tree.getRoot());
}
template<typename T>
Tree<T>::Tree(const Queue<T> & queue) {
    this->size = 0;
    root = EXPRESSIO_NULL;

    Queue<T>::ConstIterator it(queue.getBegin());

    while (it != queue.getEnd())
        insert(*it++);
}
template<typename T>
Tree<T>::Tree(const std::initializer_list<T> & list) {
    this->size = 0;
    root = EXPRESSIO_NULL;

    const T * it = list.begin();

    while (it != list.end())
        insert(*it++);
}
template<typename T>
Tree<T>::Tree(UInt size, const T * elements) {
    this->size = 0;
    root = EXPRESSIO_NULL;

    for (UInt i = 0; i < size; i++)
        insert(elements[i]);
}
template<typename T>
Tree<T>::~Tree() {
    destroy(root);
}

template<typename T>
UInt Tree<T>::getSize() const {
    return size;
}
template<typename T>
typename Tree<T>::NodePointer Tree<T>::getRoot() {
    return root;
}
template<typename T>
Tree<T> & Tree<T>::setRoot(NodePointer root) {
    this->root = root;

    return *this;
}
template<typename T>
Bool Tree<T>::isLeaf() const {
    return root->left == EXPRESSIO_NULL && root->right == EXPRESSIO_NULL;
}
template<typename T>
Bool Tree<T>::isEmpty() const {
    return root == EXPRESSIO_NULL;
}
template<typename T>
UInt Tree<T>::height() const {
    return height(root);
}
template<typename T>
UInt Tree<T>::leafCount() const {
    return leafCount(root);
}
template<typename T>
std::string Tree<T>::toString() const {
    std::stringstream sstream;

    format(root, sstream, "", true);

    return sstream.str();
}

template<typename T>
Tree<T> & Tree<T>::insert(const T & element) {
    size++;

    insert(root, element);

    return *this;
}
template<typename T>
Bool Tree<T>::remove(const T & element) {
    if (remove(root, element)) {
        size--;

        return true;
    }

    return false;
}
template<typename T>
typename Tree<T>::NodePointer Tree<T>::search(const T & element) {
    return search(root, element);
}
template<typename T>
Tree<T> & Tree<T>::preOrder(Function & function) {
    preOrder(root, function);

    return *this;
}
template<typename T>
Tree<T> & Tree<T>::postOrder(Function & function) {
    postOrder(root, function);

    return *this;
}
template<typename T>
Tree<T> & Tree<T>::inOrder(Function & function) {
    inOrder(root, function);

    return *this;
}
template<typename T>
Tree<T> & Tree<T>::levelOrder(Function & function) {
    if (root == EXPRESSIO_NULL)
        return *this;

    Queue<NodePointer> queue;
    queue.insert(root);

    while (!queue.isEmpty()) {
        NodePointer n = *queue.getBegin();
        queue.remove();

        function.compute(n->data);

        if (n->left != EXPRESSIO_NULL)
            queue.insert(n->left);

        if (n->right != EXPRESSIO_NULL)
            queue.insert(n->right);
    }

    return *this;
}

template<typename T>
Tree<T> & Tree<T>::clear() {
    destroy(root);

    size = 0;
    root = EXPRESSIO_NULL;

    return *this;
}

template<typename T>
void Tree<T>::format(NodePointer node, std::stringstream & sstream,
    std::string prefix, Bool left, Bool tail) const {
    if (node == EXPRESSIO_NULL)
        return;

    sstream << prefix;

    if (tail)
        sstream << (left ? "|_ _" : "|___");
    else
        sstream << "|---";

    sstream << node->data << std::endl;
    prefix += tail ? "    " : "|   ";

    format(node->left, sstream, prefix, true, node->right == EXPRESSIO_NULL);
    format(node->right, sstream, prefix, false, true);
}
template<typename T>
UInt Tree<T>::height(NodePointer node) const {
    if (node == EXPRESSIO_NULL)
        return 0;

    UInt lh = height(node->left);
    UInt rh = height(node->right);

    return lh > lh ? lh + 1 : rh + 1;
}
template<typename T>
UInt Tree<T>::leafCount(NodePointer node) const {
    if (node == EXPRESSIO_NULL)
        return 0;

    if (node->left == EXPRESSIO_NULL && node->right == EXPRESSIO_NULL)
        return 1;

    return leafCount(node->left) + leafCount(node->right);
}
template<typename T>
Tree<T> & Tree<T>::copy(NodePointer & node) {
    if (node != EXPRESSIO_NULL) {
        insert(node->data);

        copy(node->left);
        copy(node->right);
    }

    return *this;
}
template<typename T>
Tree<T> & Tree<T>::insert(NodePointer & node, const T & data) {
    if (node == EXPRESSIO_NULL) {
        NodePointer n = new Node;

        n->data = data;
        n->left = EXPRESSIO_NULL;
        n->right = EXPRESSIO_NULL;

        node = n;
    }
    else if (data < node->data)
        insert(node->left, data);
    else
        insert(node->right, data);

    return *this;
}
template<typename T>
Bool Tree<T>::remove(NodePointer & node, const T & data) {
    if (node == EXPRESSIO_NULL)
        return false;

    if (data == node->data) {
        if (node->left != EXPRESSIO_NULL && node->right != EXPRESSIO_NULL) {
            NodePointer & n = node->right;

            while (n->left != EXPRESSIO_NULL)
                n = n->left;

            node->data = n->data;

            return remove(n, n->data);
        }

        NodePointer n = node;

        if (n->left == EXPRESSIO_NULL)
            node = n->right;
        else
            node = n->left;

        delete n;

        return true;
    }

    return data < node->data ?
        remove(node->left, data) : remove(node->right, data);
}
template<typename T>
typename Tree<T>::NodePointer Tree<T>::search(NodePointer node, const T & data) const {
    if (node == EXPRESSIO_NULL)
        return node;

    if (data < node->data)
        node = search(node->left, data);

    if (data > node->data)
        node = search(node->right, data);

    return node;
}
template<typename T>
Tree<T> & Tree<T>::preOrder(NodePointer node, Function & function) {
    if (node == EXPRESSIO_NULL)
        return *this;

    function.compute(node->data);

    preOrder(node->left, function);
    preOrder(node->right, function);

    return *this;
}
template<typename T>
Tree<T> & Tree<T>::postOrder(NodePointer node, Function & function) {
    if (node == EXPRESSIO_NULL)
        return *this;

    postOrder(node->left, function);
    postOrder(node->right, function);

    function.compute(node->data);

    return *this;
}
template<typename T>
Tree<T> & Tree<T>::inOrder(NodePointer node, Function & function) {
    if (node == EXPRESSIO_NULL)
        return *this;

    inOrder(node->left, function);

    function.compute(node->data);

    inOrder(node->right, function);

    return *this;
}
template<typename T>
Tree<T> & Tree<T>::destroy(NodePointer & node) {
    if (node != EXPRESSIO_NULL) {
        destroy(node->left);
        destroy(node->right);

        delete node;
    }

    return *this;
}

EXPRESSIO_NAMESPACE_END

#endif