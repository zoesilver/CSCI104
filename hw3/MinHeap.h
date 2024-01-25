#include <cctype>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

template<typename T>
struct myPriorityNode {
    T item;
    int priority;
    myPriorityNode(T& str, int prior) : item(str), priority(prior){};
    // operator overloader for when nodes have same priority
    bool operator>(const myPriorityNode<T>& node) const { return item > node.item; };
};
template<class T>
class MinHeap {
public:
    MinHeap(int d);
    /* Constructor that builds a d-ary Min Heap
       This should work for any d >= 2,
       but doesn't have to do anything for smaller d.*/

    ~MinHeap();

    void add(T item, int priority);

    const T& peek() const;
    /* returns the element with smallest priority.
       Break ties however you wish.
       Throws an exception if the heap is empty. */

    void remove();
    /* removes the element with smallest priority.
       Break ties however you wish.
       Throws an exception if the heap is empty. */

    bool isEmpty() const;
    /* returns true iff there are no elements on the heap. */

private:
    std::vector<myPriorityNode<T>> storage;
    bool compareByIntValue(const myPriorityNode<T>& node1, const myPriorityNode<T>& node2);
    int m_d;
};

template<class T>
MinHeap<T>::MinHeap(int d) {
    m_d = d;
}
template<class T>
MinHeap<T>::~MinHeap() {}

template<class T>
void MinHeap<T>::add(T item, int priority) {
    // create new instance of this object
    myPriorityNode<T> newNode{item, priority};
    storage.push_back(newNode);
    int index = storage.size() - 1;

    // if this is the first item in storage, there is no need to sort
    if (index == 0) {
        return;
    }

    if (storage.size() != 1) {
        // if the compare bool returns true, child is greater than parents and no more sorting is needed
        // storage[index] is child
        while (index > 0 && !compareByIntValue(storage[index], storage[(index - 1) / m_d])) {
            // swap parent and child
            std::swap(storage[index], storage[(index - 1) / m_d]);
            // increment index to follow newest added node
            index = (index - 1) / m_d;
        }
    }
}
template<class T>
const T& MinHeap<T>::peek() const {
    if (!isEmpty()) {
        return storage.front().item;
    } else {
        throw std::runtime_error("No items in heap");
    }
}
template<class T>
void MinHeap<T>::remove() {
    if (!isEmpty()) {
        // if there's only one item  in storage, no swapping can occur. thus return.
        if (storage.size() == 1) {
            storage.pop_back();
        } else {
            // swap first and last elements
            std::swap(storage[0], storage[storage.size() - 1]);
            // remove last element
            storage.pop_back();
            typename std::vector<T>::size_type index = 0;
            // find smallest child between "siblings" (children of same parent)
            while (index * m_d + 1 < storage.size()) {
                // default child is first child
                size_t child = index * m_d + 1;
                // iterate through children and find smallest child priority
                for (int i = 1; i < m_d; ++i) {
                    size_t nextChild = index * m_d + 1 + i;
                    if (nextChild < storage.size() && !compareByIntValue(storage[nextChild], storage[child])) {
                        child = nextChild;
                    }
                }
                // swap root element with children if needed
                if (!compareByIntValue(storage[child], storage[index])) {
                    std::swap(storage[index], storage[child]);
                    // update index to follow the node that's getting swapped down
                    index = child;
                } else {
                    // If the root is smaller than all its children, no further swaps needed
                    break;
                }
            }
        }
    }

    else {
        throw std::runtime_error("No items in heap");
    }
}
template<class T>
bool MinHeap<T>::isEmpty() const {
    if (storage.size() == 0) {
        return true;
    } else {
        return false;
    }
}
template<class T>
bool MinHeap<T>::compareByIntValue(const myPriorityNode<T>& node1, const myPriorityNode<T>& node2) {
    // check if equal
    if (node1.priority != node2.priority) {
        // returns true if child node is bigger than parent
        return node1.priority > node2.priority;
    }
    // if items are equal
    else {
        return node1 > node2;
    }
}
