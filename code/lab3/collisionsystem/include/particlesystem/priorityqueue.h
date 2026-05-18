#pragma once

#include <iostream>
#include <vector>
#include <cassert>

#define TEST_PRIORITY_QUEUE

/**
 * A heap based priority queue where the root is the smallest element -- min heap
 */
template <class Comparable>
class PriorityQueue {
public:
    /**
     * Constructor to create a queue with the given capacity
     */
    explicit PriorityQueue(int initCapacity = 100);

    /**
     * Constructor to initialize a priority queue based on a given vector V
     */
    explicit PriorityQueue(const std::vector<Comparable>& V);

    // Disable copying
    PriorityQueue(const PriorityQueue&) = delete;
    PriorityQueue& operator=(const PriorityQueue&) = delete;

    /**
     * Make the queue empty
     */
    void makeEmpty();

    /**
     * Check is the queue is empty
     * Return true if the queue is empty, false otherwise
     */
    bool isEmpty() const;

    /**
     * Get the size of the queue, i.e. number of elements in the queue
     */
    size_t size() const;

    /**
     * Get the smallest element in the queue
     */
    Comparable findMin();

    /**
     * Remove and return the smallest element in the queue
     */
    Comparable deleteMin();

    /**
     * Add a new element x to the queue
     */
    void insert(const Comparable& x);

private:
    std::vector<Comparable> pq;

    // Auxiliary member functions

    /**
     * Restore the heap property
     */
    void heapify();

    /**
     * Test whether pq is a min heap
     */
    bool isMinHeap() const;
};

/* *********************** Member functions implementation *********************** */

/**
 * Constructor to create a queue with the given capacity
 */
template <class Comparable>
PriorityQueue<Comparable>::PriorityQueue(int initCapacity) {
    /*
     * ADD CODE HERE
     */
    // reservera minne i vektorn pq för initCapacity element
    pq.reserve(initCapacity);


    assert(isEmpty());  // do not remove this line
}

/**
 * Constructor to initialize a priority queue based on a given vector V
 */
template <class Comparable>
PriorityQueue<Comparable>::PriorityQueue(const std::vector<Comparable>& V) : pq{V} {
    // Implementation is provided for you
    heapify();
#ifdef TEST_PRIORITY_QUEUE
    assert(isMinHeap());
#endif
}

/**
 * Make the queue empty
 */
template <class Comparable>
void PriorityQueue<Comparable>::makeEmpty() {
    /*
     * ADD CODE HERE
     */

    // ta bort elementen i vektorn pq, ger size 0
    pq.clear();
}

/**
 * Check is the queue is empty
 * Return true if the queue is empty, false otherwise
 */
template <class Comparable>
bool PriorityQueue<Comparable>::isEmpty() const {
    /*
     * ADD CODE HERE
     */

    // kontrollerer om tom då returnerar true annars false

    return pq.empty();  
}

/**
 * Get the size of the queue, i.e. number of elements in the queue
 */
template <class Comparable>
size_t PriorityQueue<Comparable>::size() const {
    /*
     * ADD CODE HERE
     */
    return pq.size();  // returnerar storleken på vektorn pq

}

/**
 * Get the smallest element in the queue
 */
template <class Comparable>
Comparable PriorityQueue<Comparable>::findMin() {
    assert(isEmpty() == false);  // do not remove this line
    /*
     * ADD CODE HERE
     */
    return pq.front();  // returnerar det minsta elementet i vektorn pq, pq[0], första värdet i
                        // vektorn är alltid det minsta elementet i en min-heap
}

/**
 * Remove and return the smallest element in the queue
 */
template <class Comparable>
Comparable PriorityQueue<Comparable>::deleteMin() {
    assert(!isEmpty());  // do not remove this line

    /*
     * ADD CODE HERE
     */
    // byt ut elementet i rooten (första elementet i vektorn) med det sista elementet i vektorn
    //spara minsta värdet(rooten)
    Comparable minValue = pq[0];

    if (pq.size()>1) {  // om det finns fler än ett element i vektorn, då kan vi flytta sista elementet till rooten
        pq[0] = pq.back();  // flytta sista elementet till rooten
        pq.pop_back();      // ta bort sista elementet i vektorn

         // återställ heapens order, property=percolate down
        size_t currentIndex = 0;                   // starta från rooten

        while (2 * currentIndex + 1 < pq.size()) {  // om vänsterbarnet finns, om noden är en leaf node, då
                                        // finns det inget vänsterbarn
            size_t leftChild = 2 * currentIndex + 1;   // vänster barn index
            size_t rightChild = 2 * currentIndex + 2;  // högerbarn index
            size_t smallest = currentIndex;            // minsta elementets index

            if (leftChild < pq.size() && pq[leftChild] < pq[smallest]) {  // om vänsterbarnet är mindre än det nuvarande minsta elementet
                smallest = leftChild;  // uppdatera minsta elementets index
            }
            if (rightChild < pq.size() && pq[rightChild] < pq[smallest]) {  // om högerbarnet finns och är mindre än det nuvarande minsta elementet
                smallest = rightChild;            // uppdatera minsta elementets index
            }
            if (smallest != currentIndex) {
                std::swap(pq[currentIndex], pq[smallest]);  /// byt plats på det nuvarande elementet och det minsta elementet
                currentIndex = smallest;  // uppdatera currentIndex till det minsta elementets index
            } else {
                break;
            }
        }
    } else {
        // Om det bara fanns ett element, töm bara vektorn
        pq.pop_back();
    }

    // Do not remove this code block
    #ifdef TEST_PRIORITY_QUEUE
        assert(isMinHeap());
    #endif

    return minValue;  // returnerar det minsta elementet
}

/**
 * Add a new element x to the queue
 */
template <class Comparable>
void PriorityQueue<Comparable>::insert(const Comparable& x) {
    /*
     * ADD CODE HERE
     */
    // insert ett värde i taget, börja från en tom heap, lägg till elementet i slutet av vektorn och percolate up 
    // 
    //nya elementet sist i vektorn
    pq.push_back(x);
    
    // percolate up
    size_t currentIndex = pq.size() - 1;  // barnet index, det nya elementet är alltid sist i vektorn

    // om barnet är mindre än föräldern och större än 0, byt plats på barnet och föräldern
    while (currentIndex > 0) { 
         size_t parentIndex =(currentIndex - 1) / 2;  // förälderns index, föräldern är alltid (childIndex - 1) / 2

         if (pq[currentIndex] < pq[parentIndex]) {  //om barnet är mindre än föräldern, byt plats på barnet och föräldern
                                                      
             std::swap(pq[currentIndex], pq[parentIndex]);
             // uppdatera currentIndex och parentIndex
             currentIndex = parentIndex;
         } else {  // annars är heapen korrekt, breaka loopen
             break;  
         }
    }

    // Do not remove this code block
    #ifdef TEST_PRIORITY_QUEUE
        assert(isMinHeap());
    #endif
}

/* ******************* Private member functions ********************* */

/**
 * Restore the heap property
 */
template <class Comparable>
void PriorityQueue<Comparable>::heapify() {
    assert(pq.size() > 1);  // do not remove this line

    /*
     * ADD CODE HERE
     */
    if (pq.empty()) return;  // ifall vectorn är tom, returnera

    //int n = pq.size() - 1;  // sista indexet i vektorn
    for (int i =static_cast<int>(pq.size() / 2) - 1; i >= 0; --i) {
        // percolate down
        size_t currentIndex = i;

        while (2 * currentIndex + 1 < pq.size()) {
            size_t leftChild = 2 * currentIndex + 1;
            size_t rightChild = 2 * currentIndex + 2;
            size_t smallest = currentIndex;

            if (leftChild < pq.size() && pq[leftChild] < pq[smallest]) {
                smallest = leftChild;
            }
            if (rightChild < pq.size() && pq[rightChild] < pq[smallest]) {
                smallest = rightChild;
            }
            if (smallest == currentIndex) {
                break;
            }
            std::swap(pq[currentIndex], pq[smallest]);
            currentIndex = smallest;
        }
    }
}

/**
 * Test whether pq is a min heap
 */
template <class Comparable>
bool PriorityQueue<Comparable>::isMinHeap() const {
    /*
     * ADD CODE HERE
    */
    // gå igenom alla noder i heapen och kontrollera om varje förälder är mindre än eller lika med sina barn
    for (size_t i = 0; i < pq.size(); ++i) {
        size_t leftChild = 2 * i + 1;
        size_t rightChild = 2 * i + 2;
        // om vänsterbarnet finns och föräldern är större än vänsterbarnet, returnera false
        if (leftChild < pq.size() && pq[i] > pq[leftChild]) {
            return false;  // föräldern är större än vänsterbarnet
        }
        if (rightChild < pq.size() && pq[i] > pq[rightChild]) {
            return false;  // föräldern är större än högerbarnet
        }
    }

    return true;  // alla föräldrar är mindre än eller lika med sina barn
}