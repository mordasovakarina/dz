#include "BST.h"
#include <iostream>
#include <algorithm>

using namespace std;

static void* last_tree_root = nullptr;

BinarySearchTree::Node::Node(Key key, Value value, Node* parent, Node* left, Node* right)
    : keyValuePair(key, value), parent(parent), left(left), right(right) {}

BinarySearchTree::Node::Node(const Node& source)
    : keyValuePair(source.keyValuePair), parent(nullptr), left(nullptr), right(nullptr) {
    if (source.left) {
        left = new Node(*source.left);
        left->parent = this;
    }
    if (source.right) {
        right = new Node(*source.right);
        right->parent = this;
    }
}

bool BinarySearchTree::Node::operator==(const Node& other) const {
    return keyValuePair == other.keyValuePair;
}

void BinarySearchTree::Node::output_node_tree() const {
    cout << "(" << keyValuePair.first << ", " << keyValuePair.second << ")";
    bool has_left = (left != nullptr);
    bool has_right = (right != nullptr);
    
    if (has_left || has_right) {
        cout << " [";
        if (has_left)
            left->output_node_tree();
        else
            cout << "null";
        cout << ", ";
        if (has_right)
            right->output_node_tree();
        else
            cout << "null";
        cout << "]";
    }
}

void BinarySearchTree::Node::insert(const Key& key, const Value& value) {
    if (key < keyValuePair.first) {
        if (left != nullptr)
            left->insert(key, value);
        else
            left = new Node(key, value, this);
    }
    else {
        if (right != nullptr)
            right->insert(key, value);
        else
            right = new Node(key, value, this);
    }
}

void BinarySearchTree::Node::erase(const Key&) {}

BinarySearchTree::BinarySearchTree(const BinarySearchTree& other)
    : _size(other._size), _root(nullptr) {
    if (other._root != nullptr)
        _root = new Node(*other._root);
}

BinarySearchTree& BinarySearchTree::operator=(const BinarySearchTree& other) {
    if (this == &other) return *this;
    
    while (_root != nullptr) {
        erase(_root->keyValuePair.first);
    }
    
    if (other._root != nullptr)
        _root = new Node(*other._root);
    else
        _root = nullptr;
    
    _size = other._size;
    return *this;
}

BinarySearchTree::BinarySearchTree(BinarySearchTree&& other) noexcept
    : _size(other._size), _root(other._root) {
    other._root = nullptr;
    other._size = 0;
}

BinarySearchTree& BinarySearchTree::operator=(BinarySearchTree&& other) noexcept {
    if (this == &other) return *this;
    
    while (_root != nullptr) {
        erase(_root->keyValuePair.first);
    }
    
    _root = other._root;
    _size = other._size;
    
    other._root = nullptr;
    other._size = 0;
    return *this;
}

BinarySearchTree::~BinarySearchTree() {
    while (_root != nullptr) {
        erase(_root->keyValuePair.first);
    }
}

void BinarySearchTree::insert(const Key& key, const Value& value) {
    if (_root == nullptr) {
        _root = new Node(key, value);
    }
    else {
        _root->insert(key, value);
    }
    ++_size;
}

void BinarySearchTree::erase(const Key& key) {
    while (true) {
        Node* current = _root;
        Node* found = nullptr;
        
        while (current != nullptr) {
            if (key < current->keyValuePair.first) {
                current = current->left;
            }
            else if (key > current->keyValuePair.first) {
                current = current->right;
            }
            else {
                found = current;
                break;
            }
        }
        
        if (found == nullptr) break;
        current = found;
        
        if (current->left == nullptr && current->right == nullptr) {
            if (current == _root) {
                _root = nullptr;
            }
            else if (current->parent->left == current) {
                current->parent->left = nullptr;
            }
            else {
                current->parent->right = nullptr;
            }
            delete current;
        }
        else if (current->left != nullptr && current->right == nullptr) {
            Node* child = current->left;
            if (current == _root) {
                _root = child;
                child->parent = nullptr;
            }
            else {
                if (current->parent->left == current)
                    current->parent->left = child;
                else
                    current->parent->right = child;
                child->parent = current->parent;
            }
            current->left = nullptr;
            delete current;
        }
        else if (current->left == nullptr && current->right != nullptr) {
            Node* child = current->right;
            if (current == _root) {
                _root = child;
                child->parent = nullptr;
            }
            else {
                if (current->parent->left == current)
                    current->parent->left = child;
                else
                    current->parent->right = child;
                child->parent = current->parent;
            }
            current->right = nullptr;
            delete current;
        }
        else {
            Node* successor = current->right;
            while (successor->left != nullptr) {
                successor = successor->left;
            }
            
            current->keyValuePair = successor->keyValuePair;
            
            if (successor->parent->left == successor)
                successor->parent->left = successor->right;
            else
                successor->parent->right = successor->right;
            
            if (successor->right != nullptr)
                successor->right->parent = successor->parent;
            
            delete successor;
        }
        --_size;
    }
}

BinarySearchTree::ConstIterator BinarySearchTree::find(const Key& key) const {
    const Node* current = _root;
    const Node* result = nullptr;
    
    while (current != nullptr) {
        if (key < current->keyValuePair.first) {
            current = current->left;
        }
        else if (key > current->keyValuePair.first) {
            current = current->right;
        }
        else {
            result = current;
            current = current->left;
        }
    }
    
    if (result != nullptr)
        return ConstIterator(result);
    return cend();
}

BinarySearchTree::Iterator BinarySearchTree::find(const Key& key) {
    Node* current = _root;
    Node* result = nullptr;
    
    while (current != nullptr) {
        if (key < current->keyValuePair.first) {
            current = current->left;
        }
        else if (key > current->keyValuePair.first) {
            current = current->right;
        }
        else {
            result = current;
            current = current->left;
        }
    }
    
    if (result != nullptr)
        return Iterator(result);
    return end();
}

pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator>
BinarySearchTree::equalRange(const Key& key) {
    auto left_it = find(key);
    if (left_it == end())
        return {end(), end()};
    
    auto right_it = left_it;
    while (right_it != end() && right_it->first == key) {
        ++right_it;
    }
    return {left_it, right_it};
}

pair<BinarySearchTree::ConstIterator, BinarySearchTree::ConstIterator>
BinarySearchTree::equalRange(const Key& key) const {
    auto left_it = find(key);
    if (left_it == cend())
        return {cend(), cend()};
    
    auto right_it = left_it;
    while (right_it != cend() && right_it->first == key) {
        ++right_it;
    }
    return {left_it, right_it};
}

BinarySearchTree::ConstIterator BinarySearchTree::min() const {
    if (_root == nullptr)
        return cend();
    
    const Node* current = _root;
    while (current->left != nullptr) {
        current = current->left;
    }
    return ConstIterator(current);
}

BinarySearchTree::ConstIterator BinarySearchTree::max() const {
    if (_root == nullptr)
        return cend();
    
    const Node* current = _root;
    while (current->right != nullptr) {
        current = current->right;
    }
    return ConstIterator(current);
}

BinarySearchTree::ConstIterator BinarySearchTree::min(const Key& key) const {
    auto range = equalRange(key);
    if (range.first == cend())
        return cend();
    
    auto best = range.first;
    for (auto it = range.first; it != range.second; ++it) {
        if (it->second < best->second)
            best = it;
    }
    return best;
}

BinarySearchTree::ConstIterator BinarySearchTree::max(const Key& key) const {
    auto range = equalRange(key);
    if (range.first == cend())
        return cend();
    
    auto best = range.first;
    for (auto it = range.first; it != range.second; ++it) {
        if (it->second > best->second)
            best = it;
    }
    return best;
}

BinarySearchTree::Iterator BinarySearchTree::begin() {
    last_tree_root = static_cast<void*>(_root);
    if (_root == nullptr)
        return Iterator(nullptr);
    
    Node* current = _root;
    while (current->left != nullptr) {
        current = current->left;
    }
    return Iterator(current);
}

BinarySearchTree::Iterator BinarySearchTree::end() {
    last_tree_root = static_cast<void*>(_root);
    return Iterator(nullptr);
}

BinarySearchTree::ConstIterator BinarySearchTree::cbegin() const {
    last_tree_root = static_cast<void*>(_root);
    if (_root == nullptr)
        return ConstIterator(nullptr);
    
    const Node* current = _root;
    while (current->left != nullptr) {
        current = current->left;
    }
    return ConstIterator(current);
}

BinarySearchTree::ConstIterator BinarySearchTree::cend() const {
    last_tree_root = static_cast<void*>(_root);
    return ConstIterator(nullptr);
}

size_t BinarySearchTree::size() const {
    return _size;
}

void BinarySearchTree::output_tree() {
    if (_root != nullptr)
        _root->output_node_tree();
    else
        cout << "Empty tree";
    cout << endl;
}

BinarySearchTree::Iterator::Iterator(Node* node) : _node(node) {}

pair<Key, Value>& BinarySearchTree::Iterator::operator*() {
    return _node->keyValuePair;
}

const pair<Key, Value>& BinarySearchTree::Iterator::operator*() const {
    return _node->keyValuePair;
}

pair<Key, Value>* BinarySearchTree::Iterator::operator->() {
    return &_node->keyValuePair;
}

const pair<Key, Value>* BinarySearchTree::Iterator::operator->() const {
    return &_node->keyValuePair;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++() {
    if (_node == nullptr)
        return *this;
    
    if (_node->right != nullptr) {
        _node = _node->right;
        while (_node->left != nullptr) {
            _node = _node->left;
        }
    }
    else {
        Node* parent = _node->parent;
        while (parent != nullptr && _node == parent->right) {
            _node = parent;
            parent = parent->parent;
        }
        _node = parent;
    }
    return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++(int) {
    Iterator temp = *this;
    ++(*this);
    return temp;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--() {
    if (_node == nullptr) {
        _node = static_cast<Node*>(last_tree_root);
        if (_node == nullptr)
            return *this;
        while (_node->right != nullptr) {
            _node = _node->right;
        }
        return *this;
    }
    
    if (_node->left != nullptr) {
        _node = _node->left;
        while (_node->right != nullptr) {
            _node = _node->right;
        }
    }
    else {
        Node* parent = _node->parent;
        while (parent != nullptr && _node == parent->left) {
            _node = parent;
            parent = parent->parent;
        }
        _node = parent;
    }
    return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--(int) {
    Iterator temp = *this;
    --(*this);
    return temp;
}

bool BinarySearchTree::Iterator::operator==(const Iterator& other) const {
    return _node == other._node;
}

bool BinarySearchTree::Iterator::operator!=(const Iterator& other) const {
    return _node != other._node;
}

BinarySearchTree::ConstIterator::ConstIterator(const Node* node) : _node(node) {}

const pair<Key, Value>& BinarySearchTree::ConstIterator::operator*() const {
    return _node->keyValuePair;
}

const pair<Key, Value>* BinarySearchTree::ConstIterator::operator->() const {
    return &_node->keyValuePair;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++() {
    if (_node == nullptr)
        return *this;
    
    if (_node->right != nullptr) {
        _node = _node->right;
        while (_node->left != nullptr) {
            _node = _node->left;
        }
    }
    else {
        const Node* parent = _node->parent;
        while (parent != nullptr && _node == parent->right) {
            _node = parent;
            parent = parent->parent;
        }
        _node = parent;
    }
    return *this;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++(int) {
    ConstIterator temp = *this;
    ++(*this);
    return temp;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--() {
    if (_node == nullptr) {
        _node = static_cast<const Node*>(last_tree_root);
        if (_node == nullptr)
            return *this;
        while (_node->right != nullptr) {
            _node = _node->right;
        }
        return *this;
    }
    
    if (_node->left != nullptr) {
        _node = _node->left;
        while (_node->right != nullptr) {
            _node = _node->right;
        }
    }
    else {
        const Node* parent = _node->parent;
        while (parent != nullptr && _node == parent->left) {
            _node = parent;
            parent = parent->parent;
        }
        _node = parent;
    }
    return *this;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--(int) {
    ConstIterator temp = *this;
    --(*this);
    return temp;
}

bool BinarySearchTree::ConstIterator::operator==(const ConstIterator& other) const {
    return _node == other._node;
}

bool BinarySearchTree::ConstIterator::operator!=(const ConstIterator& other) const {
    return _node != other._node;
}