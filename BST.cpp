#include "BST.h"
#include <iostream>
#include <iomanip>
#include <cmath>

// Node

BinarySearchTree::Node::Node(Key key, Value value, Node *parent, Node *left, Node *right) : keyValuePair(key, value), parent(parent), left(left), right(right) {}

BinarySearchTree::Node::Node(const Node &other) : keyValuePair(other.keyValuePair), parent(other.parent), left(other.left), right(other.right), root(other.root) {}

bool BinarySearchTree::Node::operator==(const Node &other) const
{
    return (keyValuePair == other.keyValuePair);
}

void BinarySearchTree::Node::output_node_tree() const
{
    if (isend)
    {
        if (left != nullptr)
        {
            left->output_node_tree();
        }
        return;
    }
    size_t h = get_height();
    size_t l = get_key_len();
    for (size_t i = 1; i <= h; i++)
    {
        std::cout << std::setw((pow(2, h-i)-1)*l) << "";
        print_children(i-1, l, (pow(2, h-i+1)-1)*l);
        std::cout << std::endl;
    }
}

void BinarySearchTree::Node::insert(const Key &key, const Value &value)
{
    if (key >= keyValuePair.first)
    {
        if (right == nullptr)
        {
            right = new Node(key, value, this);
            right->root = root;
        }
        else
        {
            right->insert(key, value);
        }
    }
    else
    {
        if (left == nullptr)
        {
            left = new Node(key, value, this);
            left->root = root;
        }
        else
        {
            left->insert(key, value);
        }
    }
}

void BinarySearchTree::Node::erase(const Key &key)
{
    if (key == keyValuePair.first)
    {
        if (left == nullptr && right != nullptr)
        {
            if (parent != nullptr)
            {
                if (parent->left == this)
                {
                    parent->left = right;
                }
                else
                {
                    parent->right = right;
                }
            }
            else
            {
                *root = right;
            }
            right->parent = parent;
            delete this;
        }
        else if (left != nullptr && right == nullptr)
        {
            if (parent != nullptr)
            {
                if (parent->left == this)
                {
                    parent->left = left;
                }
                else
                {
                    parent->right = left;
                }
            }
            else
            {
                *root = left;
            }
            left->parent = parent;
            delete this;
        }
        else if (left == nullptr && right == nullptr)
        {
            if (parent != nullptr)
            {
                if (parent->left == this)
                {
                    parent->left = nullptr;
                }
                else
                {
                    parent->right = nullptr;
                }
            }
            else
            {
                *root = nullptr;
            }
            delete this;
        }
        else
        {
            Node* node = right;
            while (node->left != nullptr)
            {
                node = node->left;
            }
            if (node->parent->left == node)
            {
                node->parent->left = node->right; 
            }
            else
            {
                node->parent->right = node->right;
            }
            if (node->right != nullptr)
            {
                node->right->parent = node->parent;
            }
            if (parent != nullptr)
            {
                if (parent->left == this)
                {
                    parent->left = node;
                }
                else
                {
                    parent->right = node;
                }
                node->parent = parent;
            }
            else
            {
                *root = node;
                node->parent = nullptr;
            }
            node->left = left;
            if (node->left != nullptr)
            {
                node->left->parent = node;
            }
            node->right = right;
            if (node->right != nullptr)
            {
                node->right->parent = node;
            }
            delete this;
        }
    }
    else if (key >= keyValuePair.first)
    {
        if (right != nullptr)
        {
            right->erase(key);
        }
    }
    else
    {
        if (left != nullptr)
        {
            left->erase(key);
        }
    }
}

size_t BinarySearchTree::Node::get_height() const
{
    size_t h = 0;
    if (left != nullptr)
    {
        h = left->get_height();
    }
    if (right != nullptr)
    {
        h = std::max(h, right->get_height());
    }
    return h+1;
}

size_t BinarySearchTree::Node::get_key_len() const
{
    size_t l = 1;
    if (!isend)
    {
        l = std::to_string(keyValuePair.first).length();
    }
    if (left != nullptr)
    {
        l = std::max(l, left->get_key_len());
    }
    if (right != nullptr)
    {
        l = std::max(l, right->get_key_len());
    }
    return l;
}

void BinarySearchTree::Node::print_children(size_t k, size_t w, size_t d) const
{
    if (k == 0)
    {
        if (!isend)
        {
            std::cout << std::setw(w) << keyValuePair.first << std::setw(d) << "";
        }
        else
        {
            std::cout << std::setw(w) << "-" << std::setw(d) << "";
        }
    }
    else
    {
        if (left != nullptr)
        {
            left->print_children(k-1, w, d);
        }
        else
        {
            for (size_t i = 0; i < pow(2, k-1); i++)
            {
                std::cout << std::setw(w) << "-" << std::setw(d) << "";
            }
        }
        if (right != nullptr)
        {
            right->print_children(k-1, w, d);
        }
        else
        {
            for (size_t i = 0; i < pow(2, k-1); i++)
            {
                std::cout << std::setw(w) << "-" << std::setw(d) << "";
            }
        }
    }
}

void BinarySearchTree::Node::balance_if_can()
{
    size_t lside = 0, rside = 0;
    if (left != nullptr)
    {
        lside = left->get_height();
    }
    if (right != nullptr)
    {
        rside = right->get_height();
    }
    if (lside > rside)
    {
        if (lside - rside < 1)
        {
            left->balance_if_can();
            return;
        }
        rotate_right();
        if (parent != nullptr)
        {
            parent->balance_if_can();
        }
    }
    else if (rside > lside)
    {
        if (rside - lside < 2)
        {
            right->balance_if_can();
            return;
        }
        rotate_left();
        if (parent != nullptr)
        {
            parent->balance_if_can();
        }
    }
    else
    {
        if (left != nullptr)
        {
            left->balance_if_can();
        }
        if (right != nullptr)
        {
            right->balance_if_can();
        }
    }
}

void BinarySearchTree::Node::rotate_left()
{
    size_t h_c = 0, h_r = 0;
    Node* b = right;
    Node* c = nullptr;
    if (right->left != nullptr)
    {
        h_c = right->left->get_height();
        c = right->left;
    }
    if (right->right != nullptr)
    {
        h_r = right->right->get_height();
    }
    if (h_c <= h_r)
    {
        if (c != nullptr)
        {
            c->parent = this;
        }
        right = c;
        if (parent != nullptr)
        {
            if (parent->left == this)
            {
                parent->left = b;
            }
            else
            {
                parent->right = b;
            }
        }
        b->parent = parent;
        b->left = this;
        parent = b;
        if (*root == this)
        {
            *root = b;
        }
    }
    else
    {
        Node* m = c->left;
        Node* n = c->right;
        if (m != nullptr)
        {
            m->parent = this;
        }
        right = m;
        if (n != nullptr)
        {
            n->parent = b;
        }
        b->left = n;
        if (parent != nullptr)
        {
            if (parent->left == this)
            {
                parent->left = c;
            }
            else
            {
                parent->right = c;
            }
        }
        c->parent = parent;
        c->left = this;
        c->right = b;
        parent = c;
        b->parent = c;
        if (*root == this)
        {
            *root = c;
        }
    }
}

void BinarySearchTree::Node::rotate_right()
{
    size_t h_c = 0, h_l = 0;
    Node* b = left;
    Node* c = nullptr;
    if (left->right != nullptr)
    {
        h_c = left->right->get_height();
        c = left->right;
    }
    if (left->left != nullptr)
    {
        h_l = left->left->get_height();
    }
    if (h_c <= h_l)
    {
        if (c != nullptr)
        {
            c->parent = this;
        }
        left = c;
        if (parent != nullptr)
        {
            if (parent->left == this)
            {
                parent->left = b;
            }
            else
            {
                parent->right = b;
            }
        }
        b->parent = parent;
        b->right = this;
        parent = b;
        if (*root == this)
        {
            *root = b;
        }
    }
    else
    {
        Node* m = c->left;
        Node* n = c->right;
        if (n != nullptr)
        {
            n->parent = this;
        }
        left = n;
        if (m != nullptr)
        {
            m->parent = b;
        }
        b->right = m;
        if (parent != nullptr)
        {
            if (parent->left == this)
            {
                parent->left = c;
            }
            else
            {
                parent->right = c;
            }
        }
        c->parent = parent;
        c->right = this;
        c->left = b;
        parent = c;
        b->parent = c;
        if (*root == this)
        {
            *root = c;
        }
    }
}

// BinarySearchTree

BinarySearchTree::Node* BinarySearchTree::copy_nodes(Node* other, Node* par, Node** nroot)
{
    if (other == nullptr)
    {
        return nullptr;
    }
    Node* node = new Node(other->keyValuePair.first, other->keyValuePair.second, par);
    node->root = nroot;
    node->left = copy_nodes(other->left, node, nroot);
    node->right = copy_nodes(other->right, node, nroot);
    return node;
}

BinarySearchTree::BinarySearchTree(const BinarySearchTree &other) : _size(other._size)
{
    _root = copy_nodes(other._root, nullptr, &_root);
}

BinarySearchTree& BinarySearchTree::operator=(const BinarySearchTree &other)
{
    if (&other == this)
    {
        return *this;
    }
    _size = other._size;
    _root = copy_nodes(other._root, nullptr, &_root);
    return *this;
}

void BinarySearchTree::move_nodes(Node* node, Node** nroot)
{
    if (node == nullptr)
    {
        return;
    }
    node->root = nroot;
    move_nodes(node->left, nroot);
    move_nodes(node->right, nroot);
}

BinarySearchTree::BinarySearchTree(BinarySearchTree &&other) noexcept : _size(other._size), _root(other._root)
{
    move_nodes(_root, &_root);
    other._size = 0;
    other._root = nullptr;
}

BinarySearchTree& BinarySearchTree::operator=(BinarySearchTree &&other) noexcept
{
    if (&other == this)
    {
        return *this;
    }
    _size = other._size;
    _root = other._root;
    move_nodes(_root, &_root);
    other._size = 0;
    other._root = nullptr;
    return *this;
}

BinarySearchTree::~BinarySearchTree()
{
    Key k;
    while (_size-1 > 0 && _root != nullptr)
    {
        k = min()->first;
        erase(k);
    }
    delete _root;
}

void BinarySearchTree::insert(const Key &key, const Value &value)
{
    if (_root == nullptr)
    {
        _root = new Node(std::numeric_limits<Key>::max(), 0);
        _root->root = &_root;
        _size++;
        _root->isend = true;
    }
    _root->insert(key, value);
    balance();
    _size++;
}

void BinarySearchTree::erase(const Key &key)
{
    if (_root == nullptr)
    {
        return;
    }
    while ((_size-1 > 0) && (find(key) != end()))
    {
        _root->erase(key);
        balance();
        _size--;
    }
}

BinarySearchTree::Iterator BinarySearchTree::find(const Key &key)
{
    if (_root == nullptr)
    {
        return Iterator(nullptr);
    }
    Node* node = _root;
    while (true)
    {
        if (node == nullptr)
        {
            return end();
        }
        if (node->keyValuePair.first == key)
        {
            while ((node->left != nullptr) && (node->left->keyValuePair.first == key))
            {
                node = node->left;
            }
            bool ok = false;
            while (!ok)
            {
                if (node->left != nullptr)
                {
                    ok = true;
                    Node* test_node = node->left;
                    while (test_node->right != nullptr)
                    {
                        test_node = test_node->right;
                        if (test_node->keyValuePair.first == key)
                        {
                            ok = false;
                            node = test_node;
                            break;
                        }
                    }
                }
                else
                {
                    ok = true;
                }
            }
            return Iterator(node);
        }
        else if (node->keyValuePair.first < key)
        {
            if (node->right == nullptr)
            {
                return end();
            }
            node = node->right;
        }
        else
        {
            if (node->left == nullptr)
            {
                return end();
            }
            node = node->left;
        }
    }
}

BinarySearchTree::ConstIterator BinarySearchTree::find(const Key &key) const
{
    if (_root == nullptr)
    {
        return ConstIterator(nullptr);
    }
    Node* node = _root;
    while (true)
    {
        if (node == nullptr)
        {
            return cend();
        }
        if (node->keyValuePair.first == key)
        {
            while ((node->left != nullptr) && (node->left->keyValuePair.first == key))
            {
                node = node->left;
            }
            bool ok = false;
            while (!ok)
            {
                if (node->left != nullptr)
                {
                    ok = true;
                    Node* test_node = node->left;
                    while (test_node->right != nullptr)
                    {
                        test_node = test_node->right;
                        if (test_node->keyValuePair.first == key)
                        {
                            ok = false;
                            node = test_node;
                            break;
                        }
                    }
                }
                else
                {
                    ok = true;
                }
            }
            return ConstIterator(node);
        }
        else if (node->keyValuePair.first < key)
        {
            if (node->right == nullptr)
            {
                return cend();
            }
            node = node->right;
        }
        else
        {
            if (node->left == nullptr)
            {
                return cend();
            }
            node = node->left;
        }
    }
}

std::pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator> BinarySearchTree::equalRange(const Key &key)
{
    if (_root == nullptr)
    {
        return std::make_pair(Iterator(nullptr), Iterator(nullptr));
    }
    Iterator it_b = find(key);
    if (it_b == end())
    {
        return std::make_pair(end(), end());
    }
    Iterator it_e = it_b;
    while (it_b->first == it_e->first)
    {
        it_e++;
        if (it_e == end())
        {
            break;
        }
    }
    return std::make_pair(it_b, it_e);
}

std::pair<BinarySearchTree::ConstIterator, BinarySearchTree::ConstIterator> BinarySearchTree::equalRange(const Key &key) const
{
    if (_root == nullptr)
    {
        return std::make_pair(ConstIterator(nullptr), ConstIterator(nullptr));
    }
    ConstIterator it_b = find(key);
    if (it_b == cend())
    {
        return std::make_pair(cend(), cend());
    }
    ConstIterator it_e = it_b;
    while (it_b->first == it_e->first)
    {
        it_e++;
        if (it_e == cend())
        {
            break;
        }
    }
    return std::make_pair(it_b, it_e);
}

BinarySearchTree::ConstIterator BinarySearchTree::min() const
{
    Node* node = _root;
    if (!node)
    {
        return cend();
    }
    while (node->left)
    {
        node = node->left;
    }
    return ConstIterator(node);
}

BinarySearchTree::ConstIterator BinarySearchTree::max() const
{
    Node* node = _root;
    if (!node)
    {
        return cend();
    }
    while (node->right)
    {
        node = node->right;
    }
    return --ConstIterator(node);
}

BinarySearchTree::ConstIterator BinarySearchTree::min(const Key &key) const
{
    std::pair<ConstIterator, ConstIterator> pair = equalRange(key);
    if (pair.first == cend())
    {
        return cend();
    }
    ConstIterator it = pair.first;
    for (ConstIterator i = pair.first; i != pair.second; i++)
    {
        if (i->second < it->second)
        {
            it = i;
        }
    }
    return it;
}

BinarySearchTree::ConstIterator BinarySearchTree::max(const Key &key) const
{
    std::pair<ConstIterator, ConstIterator> pair = equalRange(key);
    if (pair.first == cend())
    {
        return cend();
    }
    ConstIterator it = pair.first;
    for (ConstIterator i = pair.first; i != pair.second; i++)
    {
        if (i->second > it->second)
        {
            it = i;
        }
    }
    return it;
}

BinarySearchTree::Iterator BinarySearchTree::begin()
{
    Node* node = _root;
    if (!node)
    {
        return Iterator(nullptr);
    }
    while (node->left)
    {
        node = node->left;
    }
    return Iterator(node);
}

BinarySearchTree::Iterator BinarySearchTree::end()
{
    Node* node = _root;
    if (!node)
    {
        return Iterator(nullptr);
    }
    while (node->right)
    {
        node = node->right;
    }
    return (Iterator(node));
}

BinarySearchTree::ConstIterator BinarySearchTree::cbegin() const
{
    return min();
}

BinarySearchTree::ConstIterator BinarySearchTree::cend() const
{
    Node* node = _root;
    if (!node)
    {
        return ConstIterator(nullptr);
    }
    while (node->right)
    {
        node = node->right;
    }
    return (ConstIterator(node));
}

size_t BinarySearchTree::size() const
{
    if (_size == 0)
    {
        return 0;
    }
    return _size-1;
}

size_t BinarySearchTree::max_height() const
{
    if (_root == nullptr)
    {
        return 0;
    }
    return _root->get_height();
}

void BinarySearchTree::output_tree()
{
    if (_root == nullptr || _size < 2)
    {
        std::cout << "Empty" << std::endl;
    }
    else
    {
        _root->output_node_tree();
    }
}

void BinarySearchTree::balance()
{
    if (_root == nullptr)
    {
        return;
    }
    _root->balance_if_can();
}

// Iterator

BinarySearchTree::Iterator::Iterator(Node *node) : _node(node) {}

std::pair<Key, Value> &BinarySearchTree::Iterator::operator*()
{
    return _node->keyValuePair;
}

const std::pair<Key, Value> &BinarySearchTree::Iterator::operator*() const
{
    return _node->keyValuePair;
}

std::pair<Key, Value> *BinarySearchTree::Iterator::operator->()
{
    return &_node->keyValuePair;
}

const std::pair<Key, Value> *BinarySearchTree::Iterator::operator->() const
{
    return &_node->keyValuePair;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++()
{
    if (!_node || _node->isend)
    {
        return *this;
    }

    if (_node->right)
    {
        _node = _node->right;
        while (_node->left)
        {
            _node = _node->left;
        }
    }
    else
    {
        Node *p = _node->parent;
        while (p && _node == p->right)
        {
            _node = p;
            p = p->parent;
        }
        if (p == nullptr)
        {
            while (_node->right)
            {
                _node = _node->right;
            }
            return *this;
        }
        _node = p;
    }
    return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++(int)
{
    if (!_node)
    {
        return *this;
    }
    return ++(*this);
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--()
{
    if (!_node)
    {
        return *this;
    }

    if (_node->left)
    {
        _node = _node->left;
        while (_node->right)
        {
            _node = _node->right;
        }
    }
    else
    {
        Node *p = _node->parent;
        while (p && _node == p->left)
        {
            _node = p;
            p = p->parent;
        }
        _node = p;
    }
    return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--(int)
{
    if (!_node)
    {
        return *this;
    }
    return --(*this);
}

bool BinarySearchTree::Iterator::operator==(const Iterator &other) const
{
    return ((_node == other._node) && !(_node->isend) && !(other._node->isend)) || ((_node->isend) && (other._node->isend));
}

bool BinarySearchTree::Iterator::operator!=(const Iterator &other) const
{
    return !(*this == other);
}

// ConstIterator

BinarySearchTree::ConstIterator::ConstIterator(const Node *node) : _node(node) {}

const std::pair<Key, Value> &BinarySearchTree::ConstIterator::operator*() const
{
    return _node->keyValuePair;
}

const std::pair<Key, Value> *BinarySearchTree::ConstIterator::operator->() const
{
    return &_node->keyValuePair;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++()
{
    if (!_node || _node->isend)
    {
        return *this;
    }

    if (_node->right)
    {
        _node = _node->right;
        while (_node->left)
        {
            _node = _node->left;
        }
    }
    else
    {
        Node *p = _node->parent;
        while (p && _node == p->right)
        {
            _node = p;
            p = p->parent;
        }
        if (p == nullptr)
        {
            while (_node->right)
            {
                _node = _node->right;
            }
            return *this;
        }
        _node = p;
    }
    return *this;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++(int)
{
    if (!_node)
    {
        return *this;
    }
    return ++(*this);
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--()
{
    if (!_node)
    {
        return *this;
    }

    if (_node->left)
    {
        _node = _node->left;
        while (_node->right)
        {
            _node = _node->right;
        }
    }
    else
    {
        Node *p = _node->parent;
        while (p && _node == p->left)
        {
            _node = p;
            p = p->parent;
        }
        _node = p;
    }
    return *this;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--(int)
{
    if (!_node)
    {
        return *this;
    }
    return --(*this);
}

bool BinarySearchTree::ConstIterator::operator==(const ConstIterator &other) const
{
    return ((_node == other._node) && !(_node->isend) && !(other._node->isend)) || ((_node->isend) && (other._node->isend));
}

bool BinarySearchTree::ConstIterator::operator!=(const ConstIterator &other) const
{
    return !(*this == other);
}
