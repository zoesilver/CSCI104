#ifndef RBBST_H
#define RBBST_H

#include "bst.h"
#include <algorithm>
#include <cstdlib>
#include <exception>
#include <iostream>

struct KeyError {};

/**
 * A special kind of node for an AVL tree, which adds the height as a data member, plus
 * other additional helper functions. You do NOT need to implement any functionality or
 * add additional data members or helper functions.
 */
template<typename Key, typename Value>
class AVLNode : public Node<Key, Value> {
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int getHeight() const;
    void setHeight(int height);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int height_;
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
 * An explicit constructor to initialize the elements by calling the base class constructor and setting
 * the color to red since every new node will be red when it is first inserted.
 */
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent)
        : Node<Key, Value>(key, value, parent), height_(1) {}

/**
 * A destructor which does nothing.q
 */
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode() {}

/**
 * A getter for the height of a AVLNode.
 */
template<class Key, class Value>
int AVLNode<Key, Value>::getHeight() const {
    return height_;
}

/**
 * A setter for the height of a AVLNode.
 */
template<class Key, class Value>
void AVLNode<Key, Value>::setHeight(int height) {
    height_ = height;
}

/**
 * An overridden function for getting the parent since a static_cast is necessary to make sure
 * that our node is a AVLNode.
 */
template<class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getParent() const {
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
 * Overridden for the same reasons as above.
 */
template<class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getLeft() const {
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
 * Overridden for the same reasons as above.
 */
template<class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getRight() const {
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}

/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/

template<class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value> {
public:
    virtual void insert(const std::pair<const Key, Value>& new_item);  // TODO
    virtual void remove(const Key& key);
    // TODO
protected:
    virtual void nodeSwap(AVLNode<Key, Value>* n1, AVLNode<Key, Value>* n2);

    // Add helper functions here
    virtual void insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* child);
    bool checkZigZig(AVLNode<Key, Value>* parent);
    void rotateLeft(AVLNode<Key, Value>* child);
    void rotateRight(AVLNode<Key, Value>* child);
    int calculateHeight(AVLNode<Key, Value>* node, int num);
    void removeFix(AVLNode<Key, Value>* node);
    AVLNode<Key, Value>* findUnbalanced(AVLNode<Key, Value>* node);
    void updateHeights(AVLNode<Key, Value>* node);
};

template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value>& new_item) {
    AVLNode<Key, Value>* new_node = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);

    AVLNode<Key, Value>* parent
            = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::getInsertLocation(new_node));

    if (parent != nullptr) {
        // calcuate original parent height
        parent->setHeight(calculateHeight(parent, 0));
        if (parent->getParent() != nullptr) {
            // calculate original grandparent height
            parent->getParent()->setHeight(calculateHeight(parent->getParent(), 0));
        }
    }
    // simple BST insert
    BinarySearchTree<Key, Value>::insertHelper(new_node);

    int newParentHeight = calculateHeight(parent, 0);

    // if item is first node in tree, return
    if (parent == nullptr) {
        // set height to be 1
        new_node->setHeight(1);
        return;
    }
    // if parent height did not change, tree is balanced
    if (newParentHeight == parent->getHeight()) {
        // update heights below parent
        updateHeights(parent);
        return;
    } else {

        insertFix(parent, new_node);
        // update all heights
        AVLNode<Key, Value>* avlRoot = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::root_);
        updateHeights(avlRoot);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::updateHeights(AVLNode<Key, Value>* node) {
    // update all node heights (down the tree)
    if (node != nullptr) {
        node->setHeight(calculateHeight(node, 0));
        updateHeights(node->getRight());
        updateHeights(node->getLeft());
    }
    return;
}

template<class Key, class Value>
int AVLTree<Key, Value>::calculateHeight(AVLNode<Key, Value>* node, int num) {
    // if at an end node, return number
    if (node == nullptr) {
        return num;
    } else {
        num++;
        int leftDepth = calculateHeight(node->getLeft(), num);
        int rightDepth = calculateHeight(node->getRight(), num);

        // calculate tallest branch
        int biggestSize = std::max(leftDepth, rightDepth);

        return biggestSize;
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* child) {

    if (parent == nullptr) {
        return;
    }
    if (parent->getParent() == nullptr) {
        return;
    }
    // get new grandparent height
    int newGPHeight;
    if (parent != nullptr) {
        if (parent->getParent() != nullptr) {
            newGPHeight = calculateHeight(parent->getParent(), 0);
        }
    }
    // if grandparent height did not change
    if (newGPHeight == parent->getParent()->getHeight()) {
        // done
        return;
    }
    // if grandparent node is balanced
    if (findUnbalanced(child) != parent->getParent()) {
        // recursive call on grandparent and parent
        return insertFix(parent->getParent(), parent);
    }
    // grandparent node is unbalanced
    else {
        // find longest tree
        if (calculateHeight(child->getRight(), 0) < calculateHeight(child->getLeft(), 0)) {
        }
        // true if zig zig, false if zig zag
        bool zigZig = checkZigZig(child);
        if (!zigZig) {
            parent->setHeight(calculateHeight(parent, 0));
            // figure out if left_right from top to bottom
            nodeSwap(child, parent);

            if (child->getRight() != nullptr && child->getLeft() != nullptr) {

                if (child->getRight()->getKey() == parent->getKey()) {
                    AVLNode<Key, Value>* ogLeftChild = child->getLeft();
                    AVLNode<Key, Value>* ogparentRight = parent->getRight();

                    child->setLeft(parent);
                    child->getLeft()->setParent(child);

                    parent->setRight(parent->getLeft());
                    parent->getRight()->setParent(parent);

                    parent->setLeft(ogLeftChild);
                    if (parent->getLeft() != nullptr) {
                        parent->getLeft()->setParent(parent);
                    }

                    parent->setLeft(ogLeftChild);
                    parent->getLeft()->setParent(parent);

                    child->setRight(ogparentRight);
                    if (child->getRight() != nullptr) {
                        child->getRight()->setParent(child);
                    }

                    rotateRight(parent);
                    return;

                } else if (child->getLeft()->getKey() == parent->getKey()) {
                    AVLNode<Key, Value>* ogRightChild = child->getRight();
                    AVLNode<Key, Value>* ogparentLeft = parent->getLeft();
                    child->setRight(parent);
                    child->getRight()->setParent(child);
                    parent->setLeft(parent->getRight());
                    parent->getLeft()->setParent(parent);
                    parent->setRight(ogRightChild);
                    parent->getRight()->setParent(parent);
                    child->setLeft(ogparentLeft);
                    if (child->getLeft() != nullptr) {
                        child->getLeft()->setParent(child);
                    }

                    rotateLeft(parent);
                    return;
                }
            }
            // parent is now a child of child
            // need to switch sides of node
            if (child->getRight() != nullptr) {

                if (child->getRight()->getKey() == parent->getKey()) {

                    child->setLeft(parent);
                    child->setRight(nullptr);

                    rotateRight(parent);
                }
            } else {
                child->setRight(parent);
                child->setLeft(nullptr);

                rotateLeft(parent);
            }

            return;
        }
    }

    // now it is a zig zig
    if (child->getParent()->getRight() != nullptr) {
        // if it is a right child
        if (child->getParent()->getRight()->getKey() == child->getKey()) {
            rotateLeft(child);
        } else {
            rotateRight(child);
        }
    } else {
        rotateRight(child);
    }

    return;
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* child) {

    AVLNode<Key, Value>* OGgrandparent = child->getParent()->getParent();

    // great grandparent
    AVLNode<Key, Value>* greatgp = child->getParent()->getParent()->getParent();

    // set right child's new parent to grandparent
    if (child->getParent()->getRight() != nullptr) {
        child->getParent()->getRight()->setParent(OGgrandparent);
    }

    OGgrandparent->setLeft(child->getParent()->getRight());

    // set parent's right to OGgrandparent
    child->getParent()->setRight(OGgrandparent);
    child->getParent()->getRight()->setParent(child->getParent());

    if (greatgp != nullptr) {
        // if this tree is a left child, update left child of great grandparent
        if (greatgp->getLeft()->getKey() == OGgrandparent->getKey()) {
            greatgp->setLeft(child->getParent());
        }
        // update right child
        else {
            greatgp->setRight(child->getParent());
        }
    }
    // if greatgp is nullptr, update root
    else {
        BinarySearchTree<Key, Value>::root_ = child->getParent();
    }

    // Set new great grandparent
    child->getParent()->setParent(greatgp);

    return;
}
template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* child) {

    AVLNode<Key, Value>* OGgrandparent = child->getParent()->getParent();

    // great grandparent
    AVLNode<Key, Value>* greatgp = child->getParent()->getParent()->getParent();

    // set left child's new parent to grandparent
    if (child->getParent()->getLeft() != nullptr) {
        child->getParent()->getLeft()->setParent(OGgrandparent);
    }
    OGgrandparent->setRight(child->getParent()->getLeft());
    // set parents left to OGgrandparent
    child->getParent()->setLeft(OGgrandparent);
    child->getParent()->getLeft()->setParent(child->getParent());
    if (greatgp != nullptr) {
        // if this tree is a right child, update right child of great grandparent
        if (greatgp->getRight()->getKey() == OGgrandparent->getKey()) {
            greatgp->setRight(child->getParent());
        }
        // update left child
        else {
            greatgp->setLeft(child->getParent());
        }
    }
    // if greatgp is nullptr, update root
    else {
        BinarySearchTree<Key, Value>::root_ = child->getParent();
        // child->getParent()->setParent(nullptr);
    }
    // set new great grandparent
    child->getParent()->setParent(greatgp);

    return;
}
template<class Key, class Value>
bool AVLTree<Key, Value>::checkZigZig(AVLNode<Key, Value>* child) {
    if (child->getParent()->getRight() != nullptr) {
        // if child is parents right child
        if (child->getParent()->getRight()->getKey() == child->getKey()) {
            // if parent is grandparents right child
            if (child->getParent()->getParent()->getRight() != nullptr) {
            }
            if (child->getParent()->getParent()->getRight() == child->getParent()) {
                // both are right children
                return true;
            }
        }
        return false;
    }

    // if child is parent's left child
    else {
        if (child->getParent()->getParent() != nullptr) {
            // if parent is grandparent's left child
            if (child->getParent()->getParent()->getLeft() == child->getParent()) {
                // both are left children
                return true;
            }
        }
        return false;
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key) {
    // update all heights
    AVLNode<Key, Value>* avlRoot = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::root_);
    updateHeights(avlRoot);
    // find parent of node
    Node<Key, Value>* existingNode = BinarySearchTree<Key, Value>::internalFind(key);
    // if key does not exist, return
    if (existingNode == nullptr) {

        return;
    }
    // //set parents height
    // if(parent!=nullptr){
    //     parent->setHeight(calculateHeight(parent, 0));
    // }
    // it is the only item in table
    else {
        BinarySearchTree<Key, Value>::remove(key);

        return;
    }
    AVLNode<Key, Value>* parent = static_cast<AVLNode<Key, Value>*>(existingNode->getParent());

    // simple BST remove
    BinarySearchTree<Key, Value>::remove(key);
    updateHeights(avlRoot);

    removeFix(parent);
}
template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* node) {
    if (node == nullptr) {
        return;
    } else if (!BinarySearchTree<Key, Value>::isBalanced()) {
        AVLNode<Key, Value>* parent = findUnbalanced(node);
        // left side is longer
        // if (parent->getRight()==nullptr){}
        if ((calculateHeight(parent->getRight(), 0) < calculateHeight(parent->getLeft(), 0))) {
            // left-left zig zig
            if (parent->getLeft()->getLeft() != nullptr) {
                rotateRight(parent->getLeft()->getLeft());
            }
            // left-right zig zag
            else {
                nodeSwap(parent->getLeft(), parent->getLeft()->getRight());
                // parent is now a child of child
                // need to switch sides of node
                parent->getLeft()->setLeft(parent->getLeft()->getRight());
                parent->getLeft()->setRight(nullptr);
                rotateRight(parent->getLeft()->getLeft());
            }
        } else {
            // right-right zig zig
            if (parent->getRight()->getRight() != nullptr) {
                rotateLeft(parent->getRight()->getRight());
            }
            // right-left zig zag
            else {
                nodeSwap(parent->getRight(), parent->getRight()->getLeft());
                // switch sides of child node
                parent->getRight()->setRight(parent->getRight()->getLeft());
                parent->getRight()->setLeft(nullptr);
                rotateLeft(parent->getRight()->getRight());
            }
        }

    }
    // node height didn't change, return
    else if (calculateHeight(node, 0) != node->getHeight()) {
        return;
    } else {
        // update height
        node->setHeight(calculateHeight(node, 0));
        removeFix(node->getParent());
    }
}
template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::findUnbalanced(AVLNode<Key, Value>* node) {
    // iterate upwards through tree until finding a node that is unbalanced
    while (node != nullptr) {
        int leftH = calculateHeight(node->getLeft(), 0);
        // std::cout<<"left h: "<<leftH<<std::endl;
        int rightH = calculateHeight(node->getRight(), 0);
        // std::cout<<"right h: "<<rightH<<std::endl;

        if (abs(leftH - rightH) > 1) {
            return node;
        } else {
            // store the result of the recursive call
            AVLNode<Key, Value>* result = findUnbalanced(node->getParent());
            // only return if an unbalanced node is found
            return result;
        }
    }
    return nullptr;
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap(AVLNode<Key, Value>* n1, AVLNode<Key, Value>* n2) {
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int tempH = n1->getHeight();
    n1->setHeight(n2->getHeight());
    n2->setHeight(tempH);
}

#endif
