#ifndef BST_H
#define BST_H

#include <cstdlib>
#include <exception>
#include <iostream>
#include <utility>

/**
 * A templated class for a Node in a search tree.
 * The getters for parent/left/right are virtual so
 * that they can be overridden for future kinds of
 * search trees, such as Red Black trees, Splay trees,
 * and AVL trees.
 */
template<typename Key, typename Value>
class Node {
public:
    Node(const Key& key, const Value& value, Node<Key, Value>* parent);
    virtual ~Node();

    const std::pair<const Key, Value>& getItem() const;
    std::pair<const Key, Value>& getItem();
    const Key& getKey() const;
    const Value& getValue() const;
    Value& getValue();

    virtual Node<Key, Value>* getParent() const;
    virtual Node<Key, Value>* getLeft() const;
    virtual Node<Key, Value>* getRight() const;

    void setParent(Node<Key, Value>* parent);
    void setLeft(Node<Key, Value>* left);
    void setRight(Node<Key, Value>* right);
    void setValue(const Value& value);

protected:
    std::pair<const Key, Value> item_;
    Node<Key, Value>* parent_;
    Node<Key, Value>* left_;
    Node<Key, Value>* right_;
};

/*
  -----------------------------------------
  Begin implementations for the Node class.
  -----------------------------------------
*/

/**
 * Explicit constructor for a node.
 */
template<typename Key, typename Value>
Node<Key, Value>::Node(const Key& key, const Value& value, Node<Key, Value>* parent)
        : item_(key, value), parent_(parent), left_(NULL), right_(NULL) {}

/**
 * Destructor, which does not need to do anything since the pointers inside of a node
 * are only used as references to existing nodes. The nodes pointed to by parent/left/right
 * are freed within the deleteAll() helper method in the BinarySearchTree.
 */
template<typename Key, typename Value>
Node<Key, Value>::~Node() {}

/**
 * A const getter for the item.
 */
template<typename Key, typename Value>
const std::pair<const Key, Value>& Node<Key, Value>::getItem() const {
    return item_;
}

/**
 * A non-const getter for the item.
 */
template<typename Key, typename Value>
std::pair<const Key, Value>& Node<Key, Value>::getItem() {
    return item_;
}

/**
 * A const getter for the key.
 */
template<typename Key, typename Value>
const Key& Node<Key, Value>::getKey() const {
    return item_.first;
}

/**
 * A const getter for the value.
 */
template<typename Key, typename Value>
const Value& Node<Key, Value>::getValue() const {
    return item_.second;
}

/**
 * A non-const getter for the value.
 */
template<typename Key, typename Value>
Value& Node<Key, Value>::getValue() {
    return item_.second;
}

/**
 * An implementation of the virtual function for retreiving the parent.
 */
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getParent() const {
    return parent_;
}

/**
 * An implementation of the virtual function for retreiving the left child.
 */
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getLeft() const {
    return left_;
}

/**
 * An implementation of the virtual function for retreiving the right child.
 */
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getRight() const {
    return right_;
}

/**
 * A setter for setting the parent of a node.
 */
template<typename Key, typename Value>
void Node<Key, Value>::setParent(Node<Key, Value>* parent) {
    parent_ = parent;
}

/**
 * A setter for setting the left child of a node.
 */
template<typename Key, typename Value>
void Node<Key, Value>::setLeft(Node<Key, Value>* left) {
    left_ = left;
}

/**
 * A setter for setting the right child of a node.
 */
template<typename Key, typename Value>
void Node<Key, Value>::setRight(Node<Key, Value>* right) {
    right_ = right;
}

/**
 * A setter for the value of a node.
 */
template<typename Key, typename Value>
void Node<Key, Value>::setValue(const Value& value) {
    item_.second = value;
}

/*
  ---------------------------------------
  End implementations for the Node class.
  ---------------------------------------
*/

/**
 * A templated unbalanced binary search tree.
 */
template<typename Key, typename Value>
class BinarySearchTree {
public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    virtual void insert(const std::pair<const Key, Value>& keyValuePair);
    virtual void remove(const Key& key);
    void clear();
    bool isBalanced() const;
    void print() const;
    bool empty() const;

public:
    /**
     * An internal iterator class for traversing the contents of the BST.
     */
    class iterator {
    public:
        iterator();

        std::pair<const Key, Value>& operator*() const;
        std::pair<const Key, Value>* operator->() const;

        bool operator==(const iterator& rhs) const;
        bool operator!=(const iterator& rhs) const;

        iterator& operator++();

    protected:
        friend class BinarySearchTree<Key, Value>;
        iterator(Node<Key, Value>* ptr);
        Node<Key, Value>* current_;
    };

public:
    iterator begin() const;
    iterator end() const;
    iterator find(const Key& key) const;

protected:
    // Mandatory helper functions
    Node<Key, Value>* internalFind(const Key& k) const;
    Node<Key, Value>* getSmallestNode() const;
    static Node<Key, Value>* predecessor(Node<Key, Value>* current);
    // Note:  static means these functions don't have a "this" pointer
    //        and instead just use the input argument.

    // Provided helper functions
    virtual void printRoot(Node<Key, Value>* r) const;
    virtual void nodeSwap(Node<Key, Value>* n1, Node<Key, Value>* n2);

    // Calculate depth of a tree
    int depthCalc(Node<Key, Value>* head, int num) const;
    // pointer to the parent of the node that will be inserted
    Node<Key, Value>* getInsertLocation(Node<Key, Value>* node);

    // inserts node
    void insertHelper(Node<Key, Value>* node);

protected:
    Node<Key, Value>* root_;
};

/*
--------------------------------------------------------------
Begin implementations for the BinarySearchTree::iterator class.
---------------------------------------------------------------
*/

/**
 * Explicit constructor that initializes an iterator with a given node pointer.
 */
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator(Node<Key, Value>* ptr) {
    current_ = ptr;
}

/**
 * A default constructor that initializes the iterator to NULL.
 */
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator() : current_(NULL) {}

/**
 * Provides access to the item.
 */
template<class Key, class Value>
std::pair<const Key, Value>& BinarySearchTree<Key, Value>::iterator::operator*() const {
    return current_->getItem();
}

/**
 * Provides access to the address of the item.
 */
template<class Key, class Value>
std::pair<const Key, Value>* BinarySearchTree<Key, Value>::iterator::operator->() const {
    return &(current_->getItem());
}

/**
 * Checks if 'this' iterator's internals have the same value
 * as 'rhs'
 */
template<class Key, class Value>
bool BinarySearchTree<Key, Value>::iterator::operator==(const BinarySearchTree<Key, Value>::iterator& rhs) const {
    if (rhs.current_ == current_) {
        return true;
    }
    return false;
}

/**
 * Checks if 'this' iterator's internals have a different value
 * as 'rhs'
 */
template<class Key, class Value>
bool BinarySearchTree<Key, Value>::iterator::operator!=(const BinarySearchTree<Key, Value>::iterator& rhs) const {
    if (rhs.current_ != current_) {
        return true;
    }
    return false;
}

/**
 * Advances the iterator's location using an in-order sequencing
 */
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator& BinarySearchTree<Key, Value>::iterator::operator++() {
    // temp node to help track
    Node<Key, Value>* currCopy = current_;

    // if it does not have a right child, search through parent nodes until bigger node is found or set to null
    if (current_->getRight() == nullptr) {
        currCopy = current_->getParent();
        while (currCopy != nullptr && currCopy->getKey() <= current_->getKey()) {
            currCopy = currCopy->getParent();
        }

        current_ = currCopy;
    }
    // if it has right child, go to the leftmost node and set current_.
    // if there is no left node of the right child, set current to right child.
    else {
        currCopy = current_->getRight();
        while (currCopy->getLeft() != nullptr) {
            currCopy = currCopy->getLeft();
        }
        current_ = currCopy;
    }
    if (current_ == nullptr) {
        *this = BinarySearchTree<Key, Value>::iterator(nullptr);
    }

    // return iterator
    return *this;
}

/*
-------------------------------------------------------------
End implementations for the BinarySearchTree::iterator class.
-------------------------------------------------------------
*/

/*
-----------------------------------------------------
Begin implementations for the BinarySearchTree class.
-----------------------------------------------------
*/

/**
 * Default constructor for a BinarySearchTree, which sets the root to NULL.
 */
template<class Key, class Value>
BinarySearchTree<Key, Value>::BinarySearchTree() : root_(NULL) {}

template<typename Key, typename Value>
BinarySearchTree<Key, Value>::~BinarySearchTree() {
    // clear all items from tree
    clear();
    // delete root_ node
    if (root_)
        delete root_;
}

/**
 * Returns true if tree is empty
 */
template<class Key, class Value>
bool BinarySearchTree<Key, Value>::empty() const {
    return root_ == NULL;
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::print() const {
    printRoot(root_);
    std::cout << "\n";
}

/**
 * Returns an iterator to the "smallest" item in the tree
 */
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<Key, Value>::begin() const {
    BinarySearchTree<Key, Value>::iterator begin(getSmallestNode());
    return begin;
}

/**
 * Returns an iterator whose value means INVALID
 */
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<Key, Value>::end() const {
    BinarySearchTree<Key, Value>::iterator end(NULL);
    return end;
}

/**
 * Returns an iterator to the item with the given key, k
 * or the end iterator if k does not exist in the tree
 */
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<Key, Value>::find(const Key& k) const {
    Node<Key, Value>* curr = internalFind(k);
    BinarySearchTree<Key, Value>::iterator it(curr);
    return it;
}

/**
 * An insert method to insert into a Binary Search Tree.
 * The tree will not remain balanced when inserting.
 */
/**
 * An insert method to insert into a Binary Search Tree.
 * The tree will not remain balanced when inserting.
 */
template<class Key, class Value>
void BinarySearchTree<Key, Value>::insert(const std::pair<const Key, Value>& keyValuePair) {
    // create new node
    Node<Key, Value>* item = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, nullptr);
    insertHelper(item);
}
template<class Key, class Value>
void BinarySearchTree<Key, Value>::insertHelper(Node<Key, Value>* node) {
    Node<Key, Value>* parent = getInsertLocation(node);
    // if it is the first node in the tree
    if (parent == nullptr) {
        root_ = node;
        return;
    }
    Node<Key, Value>* existingNode = internalFind(node->getKey());
    // if key is already in tree
    if (existingNode != nullptr) {
        // if it's the left child
        if (parent->getLeft()->getKey() == node->getKey()) {
            // update left child's value
            parent->getLeft()->setValue(node->getValue());
        } else {
            // update right child's value
            parent->getRight()->setValue(node->getValue());
        }
    } else {
        // update parent
        node->setParent(parent);
        // if its the left child, set parent's left child
        if (node->getKey() < node->getParent()->getKey()) {
            parent->setLeft(node);
        }
        // must be right child, set parent's right child
        else {
            parent->setRight(node);
        }
    }
}

template<class Key, class Value>
// get pointer to future parent of node that will be inserted
Node<Key, Value>* BinarySearchTree<Key, Value>::getInsertLocation(Node<Key, Value>* node) {
    // if first item in the tree, parent will be null
    if (BinarySearchTree<Key, Value>::root_ == nullptr) {
        return nullptr;
    }
    // if key is already in the tree, return pointer to that node's parent
    Node<Key, Value>* existingNode = internalFind(node->getKey());
    if (existingNode != nullptr) {
        return existingNode->getParent();
    } else {
        // find proper location to add new node
        Node<Key, Value>* location = BinarySearchTree<Key, Value>::root_;

        // while there is not empty right child and KVP is less than the parent,
        // or there is an empty left child and KVP is greater than the parent
        while ((location->getRight() != nullptr && location->getKey() < node->getKey())
               || (location->getLeft() != nullptr && location->getKey() > node->getKey())) {

            // if KVP is greater than the node being examined, examine right node
            if (location->getKey() < node->getKey()) {
                location = location->getRight();
            } else {
                location = location->getLeft();
            }
        }
        return location;
    }
}
/**
 * A remove method to remove a specific key from a Binary Search Tree.
 * The tree may not remain balanced after removal.
 */
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::remove(const Key& key) {
    // find node to be removed
    Node<Key, Value>* existingNode = internalFind(key);
    // if there is no node with this key, do nothing
    if (existingNode == nullptr) {
        return;
    }
    // get pointer to parent of deleted node
    Node<Key, Value>* replacementNode;

    // if it has two children, swap with successor
    if (existingNode->getRight() != nullptr && existingNode->getLeft() != nullptr) {
        Node<Key, Value>* successorNode = getSuccessor(existingNode);
        nodeSwap(existingNode, successorNode);
    }

    // now all cases will either have 0 or 1 child
    // if it has exactly one child
    if ((existingNode->getRight() == nullptr && existingNode->getLeft() != nullptr)
        || ((existingNode->getRight() != nullptr && existingNode->getLeft() == nullptr))) {
        // if it has a right child
        if (existingNode->getRight() != nullptr) {
            // replacementNode is the existingNodes right child
            replacementNode = existingNode->getRight();
            // swap parent and child
            nodeSwap(existingNode, replacementNode);
            // set replacementNode child's to it's original child
            replacementNode->setRight(existingNode->getRight());
            replacementNode->setLeft(existingNode->getLeft());

        }

        // if existingNode has a left child
        else {
            // replacementNode is the existingNodes left child
            replacementNode = existingNode->getLeft();
            // swap parent and child
            nodeSwap(existingNode, replacementNode);

            // set replacementNode child's to it's original child
            replacementNode->setRight(existingNode->getRight());
            replacementNode->setLeft(existingNode->getLeft());
        }
        // if child is not null, set it's parent to replacementNode
        if (replacementNode->getRight() != nullptr) {
            replacementNode->getRight()->setParent(replacementNode);
        }
        if (replacementNode->getLeft() != nullptr) {
            replacementNode->getLeft()->setParent(replacementNode);
        }
    }

    // if it has zero children
    // if the node's parent is not nullptr
    if (existingNode->getParent() != nullptr) {

        // if it is a left child
        if (existingNode->getParent()->getLeft() == existingNode) {
            // set parent's left child value to null
            existingNode->getParent()->setLeft(nullptr);
        }
        if (existingNode->getParent()->getRight() == existingNode) {
            // set parent's right child value to null
            existingNode->getParent()->setRight(nullptr);
        }
    } else {
        root_ = nullptr;
    }

    // delete the node
    delete existingNode;
}
// find next highest node
template<class Key, class Value>
Node<Key, Value>* getSuccessor(Node<Key, Value>* existingNode) {
    Node<Key, Value>* temp;
    // if a right child exists, go to leftmost value within that child
    if (existingNode->getRight() != nullptr) {
        temp = existingNode->getRight();
        // find left most value
        while (temp->getLeft() != nullptr) {
            temp = temp->getLeft();
        }
    } else {
        // search for node who is the left child of it's parent
        temp = existingNode->getParent();
        while (temp->getLeft() != temp) {
            temp = temp->getParent();
        }
    }
    return temp;
}

// get pointer to parent of deleted node
template<class Key, class Value>
Node<Key, Value>* getParent(Node<Key, Value>* parentPtr) {
    return parentPtr;
}

template<class Key, class Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::predecessor(Node<Key, Value>* current) {
    Node<Key, Value>* temp;

    // If current has a left child
    if (current->getLeft() != nullptr) {
        temp = current->getLeft();
        // Find the rightmost node in the left subtree
        while (temp->getRight() != nullptr) {
            temp = temp->getRight();
        }
        return temp;
    }
    // If current is the right child of its parent
    else if (current->getParent()->getRight() == current) {
        return current->getParent();
    } else {
        // Find ancestor where current is the right child of its parent
        temp = current->getParent();
        while (temp->getParent() != nullptr && temp->getLeft() == current) {
            temp = temp->getParent();
        }
        return temp->getParent();
    }
}

/**
 * A method to remove all contents of the tree and
 * reset the values in the tree for use again.
 */
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clear() {

    // //call helper function to recursively delete everything
    clearAssist(root_);
    // set root_ to null
    root_ = nullptr;
}
template<typename Key, typename Value>
void clearAssist(Node<Key, Value>* temp) {
    // when node to be examined in null, return
    if (temp == nullptr) {

        return;
    } else {
        // store children variables
        Node<Key, Value>* temp1 = temp->getRight();
        Node<Key, Value>* temp2 = temp->getLeft();
        // detach children from parent
        if (temp->getRight() != nullptr) {
            temp->getRight()->setParent(nullptr);
        }
        if (temp->getLeft() != nullptr) {
            temp->getLeft()->setParent(nullptr);
        }
        temp->setRight(nullptr);
        temp->setLeft(nullptr);
        delete temp;
        clearAssist(temp1);
        clearAssist(temp2);
    }
}

/**
 * A helper function to find the smallest node in the tree.
 */
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::getSmallestNode() const {
    Node<Key, Value>* currCopy = root_;
    // if no items in the tree, return nullptr
    if (root_ == nullptr) {
        return currCopy;
    }
    // go to left most node in tree
    while (currCopy->getLeft() != nullptr) {
        currCopy = currCopy->getLeft();
    }
    // return pointer to smallest node
    return currCopy;
}

/**
 * Helper function to find a node with given key, k and
 * return a pointer to it or NULL if no item with that key
 * exists
 */
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::internalFind(const Key& key) const {
    Node<Key, Value>* match = root_;
    // Check if root is null
    if (match == nullptr) {
        return nullptr;
    }  // if it is not a leaf node, keep iterating through
    while (match != nullptr) {

        // if KVP is greater than the node being examined, examine right node
        if (match->getKey() < key) {
            match = match->getRight();
        }
        // if KVP is less than the node being examined, examine right node
        else if (match->getKey() > key) {
            match = match->getLeft();
        }
        // if key equals node being examined, this is a match
        else {
            return match;
        }
    }
    // if all appropriate nodes have been checked, there is no match and NULL is returned

    return nullptr;
}

/**
 * Return true iff the BST is balanced.
 */
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isBalanced() const {
    return depthCalc(root_, 0) != -1;
}

// calculate height of any given branch
template<typename Key, typename Value>
int BinarySearchTree<Key, Value>::depthCalc(Node<Key, Value>* head, int num) const {

    // if at an end node, return number
    if (head == nullptr) {

        return num;
    } else {
        num++;
        int leftDepth = depthCalc(head->getLeft(), num);
        int rightDepth = depthCalc(head->getRight(), num);
        // if it is imbalanced at any point, return -1
        if (abs(leftDepth - rightDepth) > 1 || leftDepth == -1 || rightDepth == -1) {
            return -1;
        }
        // calculate tallest branch
        int biggestSize = std::max(leftDepth, rightDepth);

        return biggestSize;
    }
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::nodeSwap(Node<Key, Value>* n1, Node<Key, Value>* n2) {
    if ((n1 == n2) || (n1 == NULL) || (n2 == NULL)) {
        return;
    }
    Node<Key, Value>* n1p = n1->getParent();
    Node<Key, Value>* n1r = n1->getRight();
    Node<Key, Value>* n1lt = n1->getLeft();
    bool n1isLeft = false;
    if (n1p != NULL && (n1 == n1p->getLeft()))
        n1isLeft = true;
    Node<Key, Value>* n2p = n2->getParent();
    Node<Key, Value>* n2r = n2->getRight();
    Node<Key, Value>* n2lt = n2->getLeft();
    bool n2isLeft = false;
    if (n2p != NULL && (n2 == n2p->getLeft()))
        n2isLeft = true;

    Node<Key, Value>* temp;
    temp = n1->getParent();
    n1->setParent(n2->getParent());
    n2->setParent(temp);

    temp = n1->getLeft();
    n1->setLeft(n2->getLeft());
    n2->setLeft(temp);

    temp = n1->getRight();
    n1->setRight(n2->getRight());
    n2->setRight(temp);

    if ((n1r != NULL && n1r == n2)) {
        n2->setRight(n1);
        n1->setParent(n2);
    } else if (n2r != NULL && n2r == n1) {
        n1->setRight(n2);
        n2->setParent(n1);

    } else if (n1lt != NULL && n1lt == n2) {
        n2->setLeft(n1);
        n1->setParent(n2);

    } else if (n2lt != NULL && n2lt == n1) {
        n1->setLeft(n2);
        n2->setParent(n1);
    }

    if (n1p != NULL && n1p != n2) {
        if (n1isLeft)
            n1p->setLeft(n2);
        else
            n1p->setRight(n2);
    }
    if (n1r != NULL && n1r != n2) {
        n1r->setParent(n2);
    }
    if (n1lt != NULL && n1lt != n2) {
        n1lt->setParent(n2);
    }

    if (n2p != NULL && n2p != n1) {
        if (n2isLeft)
            n2p->setLeft(n1);
        else
            n2p->setRight(n1);
    }
    if (n2r != NULL && n2r != n1) {
        n2r->setParent(n1);
    }
    if (n2lt != NULL && n2lt != n1) {
        n2lt->setParent(n1);
    }

    if (this->root_ == n1) {
        this->root_ = n2;
    } else if (this->root_ == n2) {
        this->root_ = n1;
    }
}

/**
 * Lastly, we are providing you with a print function,
   BinarySearchTree::printRoot().
   Just call it with a node to start printing at, e.g:
   this->printRoot(this->root_) // or any other node pointer

   It will print up to 5 levels of the tree rooted at the passed node,
   in ASCII graphics format.
   We hope it will make debugging easier!
  */

// include print function (in its own file because it's fairly long)
#include "print_bst.h"

/*
---------------------------------------------------
End implementations for the BinarySearchTree class.
---------------------------------------------------
*/

#endif