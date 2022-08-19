// mymap.h
//
// Project 5 - mymap
// Dwij Shetty UIN:657811511

#pragma once

#include <iostream>
#include <sstream>

using namespace std;

template<typename keyType, typename valueType>
class mymap {
private:
    struct NODE {
        keyType key;  // used to build BST
        valueType value;  // stored data for the map
        NODE *left;  // links to left child
        NODE *right;  // links to right child
        int nL;  // number of nodes in left subtree
        int nR;  // number of nodes in right subtree
        bool isThreaded;
    };
    NODE *root;  // pointer to root node of the BST
    int size;  // # of key/value pairs in the mymap

    //
    // iterator:
    // This iterator is used so that mymap will work with a foreach loop.
    //
    struct iterator {
    private:
        NODE *curr;  // points to current in-order node for begin/end

    public:
        iterator(NODE *node) {
            curr = node;
        }

        keyType operator*() {
            return curr->key;
        }

        bool operator==(const iterator &rhs) {
            return curr == rhs.curr;
        }

        bool operator!=(const iterator &rhs) {
            return curr != rhs.curr;
        }

        bool isDefault() {
            return !curr;
        }

        //
        // operator++:
        //
        // This function should advance curr to the next in-order node.
        // O(logN)
        //
        iterator operator++() {
            NODE *node = curr->right;
            if (!curr->isThreaded) {
                while (node->left != nullptr) {
                    node = node->left;
                }
            }
            curr = node;
            return *this;
        }
    };

    string _toString(const NODE *n) {
        NODE *rightptr;
        if (n == nullptr) {
            return "";
        }
        string left = _toString(n->left);
        string curr = "key: " + to_string(n->key) + " " +
                      "value: " + to_string(n->value) + "\n";
        if (n->isThreaded) {
            rightptr = nullptr;
        } else {
            rightptr = n->right;
        }
        string right = _toString(rightptr);
        return left + curr + right;
    }

    void _clear(NODE *n) {
        NODE *rightptr;
        if (n == nullptr) {
            return;
        }
        _clear(n->left);
        if (n->isThreaded) {
            rightptr = nullptr;
        } else {
            rightptr = n->right;
        }
        _clear(rightptr);
        delete n;
    }

    NODE *_copy(NODE *n, NODE *prev) {
        NODE *rightptr;
        if (n == nullptr) {
            return nullptr;
        }
        if (n->isThreaded) {
            rightptr = nullptr;
        } else {
            rightptr = n->right;
        }
        NODE *right = _copy(rightptr, prev);
        NODE *copy = new NODE;
        copy->key = n->key;
        copy->value = n->value;
        copy->left = nullptr;
        copy->nL = n->nL;
        copy->nR = n->nR;
        copy->isThreaded = n->isThreaded;
        if (n->isThreaded) {
            copy->right = prev;
        } else {
            copy->right = right;
        }
        NODE *left = _copy(n->left, copy);
        copy->left = left;
        return copy;
    }

public:
    //
    // default constructor:
    //
    // Creates an empty mymap.
    // Time complexity: O(1)
    //
    mymap() {
        root = nullptr;
        size = 0;
    }

    //
    // copy constructor:
    //
    // Constructs a new mymap which is a copy of the "other" mymap.
    // Sets all member variables appropriately.
    // Time complexity: O(n), where n is total number of nodes in threaded,
    // self-balancing BST.
    //
    mymap(const mymap &other) {
        this->size = other.size;
        this->root = _copy(other.root, nullptr);
    }

    //
    // operator=:
    //
    // Clears "this" mymap and then makes a copy of the "other" mymap.
    // Sets all member variables appropriately.
    // Time complexity: O(n), where n is total number of nodes in threaded,
    // self-balancing BST.
    //
    mymap &operator=(const mymap &other) {
        clear();
        this->root = _copy(other.root, nullptr);
        this->size = other.size;
        return *this;
    }

    // clear:
    //
    // Frees the memory associated with the mymap; can be used for testing.
    // Time complexity: O(n), where n is total number of nodes in threaded,
    // self-balancing BST.
    //
    void clear() {
        _clear(root);
        root = nullptr;
        size = 0;
    }

    //
    // destructor:
    //
    // Frees the memory associated with the mymap.
    // Time complexity: O(n), where n is total number of nodes in threaded,
    // self-balancing BST.
    //
    ~mymap() {
        clear();
    }

    //
    // put:
    //
    // Inserts the key/value into the threaded, self-balancing BST based on
    // the key.
    // Time complexity: O(logn + mlogm), where n is total number of nodes in the
    // threaded, self-balancing BST and m is the number of nodes in the
    // sub-tree that needs to be re-balanced.
    // Space complexity: O(1)
    void put(keyType key, valueType value) {
        NODE *curr = root;
        NODE *prev = nullptr;

        while (curr != nullptr) {
            if (key == curr->key) {// found
                curr->value = value;
                return;
            }
            if (key < curr->key) // go left:
            {
                prev = curr;
                curr = curr->left;
            } else // go right:
            {
                prev = curr;
                if (curr->isThreaded) {
                    curr = nullptr;
                } else {
                    curr = curr->right;
                }
            }
        }
        NODE *n = new NODE();
        n->key = key;
        n->value = value;
        n->left = nullptr;
        n->right = nullptr;
        n->nL = 0;
        n->nR = 0;
        n->isThreaded = false;

        if (prev == nullptr) {
            root = n;
        } else if (key < prev->key) {
            prev->left = n;
            n->isThreaded = true;
            n->right = prev;
        } else {
            n->right = prev->right;
            n->isThreaded = true;
            prev->right = n;
            prev->isThreaded = false;
        }
        size++;
    }

    //
    // contains:
    // Returns true if the key is in mymap, return false if not.
    // Time complexity: O(logn), where n is total number of nodes in the
    // threaded, self-balancing BST
    //
    bool contains(keyType key) {
        NODE *curr = root;
        while (curr != nullptr) {
            if (key == curr->key) {// found
                return true;
            }
            if (key < curr->key) // go left:
            {
                curr = curr->left;
            } else // go right:
            {
                if (curr->isThreaded) {
                    curr = nullptr;
                } else {
                    curr = curr->right;
                }
            }
        }
        return false;
    }

    //
    // get:
    //
    // Returns the value for the given key; if the key is not found, the
    // default value, valueType(), is returned (but not added to mymap).
    // Time complexity: O(logn), where n is total number of nodes in the
    // threaded, self-balancing BST
    //
    valueType get(keyType key) {
        NODE *curr = root;

        while (curr != nullptr) {
            if (key == curr->key) {// found
                return curr->value;
            }
            if (key < curr->key) // go left:
            {
                curr = curr->left;
            } else // go right:
            {
                if (curr->isThreaded) {
                    curr = nullptr;
                } else {
                    curr = curr->right;
                }
            }
        }

        return valueType();
    }

    //
    // operator[]:
    //
    // Returns the value for the given key; if the key is not found,
    // the default value, valueType(), is returned (and the resulting new
    // key/value pair is inserted into the map).
    // Time complexity: O(logn + mlogm), where n is total number of nodes in the
    // threaded, self-balancing BST and m is the number of nodes in the
    // sub-trees that need to be re-balanced.
    // Space complexity: O(1)
    //
    valueType operator[](keyType key) {
        NODE *curr = root;
        NODE *prev = nullptr;

        while (curr != nullptr) {
            if (key == curr->key) {// found
                return curr->value;
            }
            if (key < curr->key) // go left:
            {
                prev = curr;
                curr = curr->left;
            } else // go right:
            {
                prev = curr;
                if (curr->isThreaded) {
                    curr = nullptr;
                } else {
                    curr = curr->right;
                }
            }
        }
        NODE *n = new NODE();
        n->key = key;
        n->value = valueType();
        n->left = nullptr;
        n->right = nullptr;
        n->nL = 0;
        n->nR = 0;
        n->isThreaded = false;

        if (prev == nullptr) {
            root = n;
        } else if (key < prev->key) {
            prev->left = n;
            n->isThreaded = true;
            n->right = prev;
        } else {
            n->right = prev->right;
            n->isThreaded = true;
            prev->right = n;
            prev->isThreaded = false;
        }
        size++;

        return n->value;
    }

    //
    // Size:
    //
    // Returns the # of key/value pairs in the mymap, 0 if empty.
    // O(1)
    //
    int Size() {
        return size;
    }

    //
    // begin:
    //
    // returns an iterator to the first in order NODE.
    // Time complexity: O(logn), where n is total number of nodes in the
    // threaded, self-balancing BST
    //
    iterator begin() {
        NODE *cur = root;
        while (cur->left != nullptr) {
            cur = cur->left;
        }
        return iterator(cur);  // TODO: Update this return.
    }

    //
    // end:
    //
    // returns an iterator to the last in order NODE.
    // this function is given to you.
    //
    // Time Complexity: O(1)
    //
    iterator end() {
        return iterator(nullptr);
    }

    //
    // toString:
    //
    // Returns a string of the entire mymap, in order.
    // Format for 8/80, 15/150, 20/200:
    // "key: 8 value: 80\nkey: 15 value: 150\nkey: 20 value: 200\n
    // Time complexity: O(n), where n is total number of nodes in the
    // threaded, self-balancing BST
    //
    string toString() {
        return _toString(root);
    }

    //
    // toVector:
    //
    // Returns a vector of the entire map, in order.  For 8/80, 15/150, 20/200:
    // {{8, 80}, {15, 150}, {20, 200}}
    // Time complexity: O(n), where n is total number of nodes in the
    // threaded, self-balancing BST
    //
    vector<pair<keyType, valueType> > toVector() {


        // TODO: write this function.


        return {};  // TODO: Update this return.
    }

    //
    // checkBalance:
    //
    // Returns a string of mymap that verifies that the tree is properly
    // balanced.  For example, if keys: 1, 2, 3 are inserted in that order,
    // function should return a string in this format (in pre-order):
    // "key: 2, nL: 1, nR: 1\nkey: 1, nL: 0, nR: 0\nkey: 3, nL: 0, nR: 0\n";
    // Time complexity: O(n), where n is total number of nodes in the
    // threaded, self-balancing BST
    //
    string checkBalance() {


        // TODO: write this function.


        return {};  // TODO: Update this return.
    }
};
