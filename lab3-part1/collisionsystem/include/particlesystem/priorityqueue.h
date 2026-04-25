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
    // ADD CODE HERE
   
    // Reservar minne i vektorn för 'initCapacity' antal element.
    // OBS: detta lägger INTE till några element, det gör bara att vektorn
    // slipper allokera om minne varje gång vi gör insert senare.
    pq.reserve(initCapacity);

    // Kontrollerar att kön är tom direkt efter konstruktion.
    // isEmpty() ska returnera true här, annars är något fel.
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

  // ADD CODE HERE

  // Tar bort alla element från vektorn (priority queue)
  // Efter detta kommer kön vara helt tom (size = 0)
  pq.clear();
     
} 

/**
 * Check is the queue is empty
 * Return true if the queue is empty, false otherwise
 */
template <class Comparable>
bool PriorityQueue<Comparable>::isEmpty() const {
    
 // ADD CODE HERE

    // Kollar om den underliggande vektorn (pq) är tom
    // Returnerar true om inga element finns, annars false
    return pq.empty();

}

/**
 * Get the size of the queue, i.e. number of elements in the queue
 */
template <class Comparable>
size_t PriorityQueue<Comparable>::size() const {
    
 // ADD CODE HERE
 
    // Returnerar antalet element i priority queue
    // Detta hämtas direkt från vektorn pq
    return pq.size();    

}

/**
 * Get the smallest element in the queue
 */
template <class Comparable>
Comparable PriorityQueue<Comparable>::findMin() {
    assert(isEmpty() == false);  // do not remove this line
    
     //ADD CODE HERE
     
    // I en min-heap ligger det minsta elementet alltid i roten,
    // vilket motsvarar index 0 i vektorn
    return pq[0];
     
}

/**
 * Remove and return the smallest element in the queue
 */
template <class Comparable>
Comparable PriorityQueue<Comparable>::deleteMin() {
    assert(!isEmpty());  // do not remove this line

    
     // ADD CODE HERE *********************************
     
    // Spara det minsta elementet (roten)
    Comparable minValue = pq[0];

    // Flytta sista elementet till roten
    pq[0] = pq.back();
    pq.pop_back();

    // "Bubble down" för att återställa heap-egenskapen
    size_t hole = 0;

    while (true) {
        size_t left = 2 * hole + 1;
        size_t right = 2 * hole + 2;
        size_t smallest = hole;

        // Kolla vänster barn
        if (left < pq.size() && pq[left] < pq[smallest]) {
            smallest = left;
        }

        // Kolla höger barn
        if (right < pq.size() && pq[right] < pq[smallest]) {
            smallest = right;
        }

        // Om vi redan är korrekt → stoppa
        if (smallest == hole) {
            break;
        }

        // Byt plats med minsta barnet
        std::swap(pq[hole], pq[smallest]);

        // Fortsätt nedåt
        hole = smallest;
    }

    // Do not remove this code block
#ifdef TEST_PRIORITY_QUEUE
    assert(isMinHeap());
#endif

    return minValue; 
}

/**
 * Add a new element x to the queue
 */
template <class Comparable>
void PriorityQueue<Comparable>::insert(const Comparable& x) {
    
   //  ADD CODE HERE
     
     // Lägg in det nya elementet sist i vektorn
   pq.push_back(x);

   // Startposition (det "hål" vi ska flytta upp)
   size_t hole = pq.size() - 1;

   // Bubble up: jämför med parent och byt plats om det behövs
   while (hole > 0) {
       size_t parent = (hole - 1) / 2;

       // Om heap-egenskapen redan är uppfylld → stoppa
       if (!(pq[hole] < pq[parent])) {
           break;
       }

       // Annars: byt plats med parent
       std::swap(pq[hole], pq[parent]);

       // Flytta upp ett steg
       hole = parent;
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

     // ADD CODE HERE

    // Vi börjar från sista inre noden (föräldrar till löv)
    // pq.size()/2 - 1 ger index för sista nod som har barn
    for (int i = static_cast<int>(pq.size()) / 2 - 1; i >= 0; --i) {

        // Startposition för "bubble down"
        size_t hole = static_cast<size_t>(i);

        // Sifta ner elementet tills heap-egenskapen uppfylls
        while (true) {
            size_t left = 2 * hole + 1;
            size_t right = 2 * hole + 2;
            size_t smallest = hole;

            // Kolla vänster barn
            if (left < pq.size() && pq[left] < pq[smallest]) {
                smallest = left;
            }

            // Kolla höger barn
            if (right < pq.size() && pq[right] < pq[smallest]) {
                smallest = right;
            }

            // Om inget barn är mindre → klart
            if (smallest == hole) {
                break;
            }

            // Byt plats med minsta barnet
            std::swap(pq[hole], pq[smallest]);

            // Fortsätt ner i trädet
            hole = smallest;
        }
    }
     
}

/**
 * Test whether pq is a min heap
 */
template <class Comparable>
bool PriorityQueue<Comparable>::isMinHeap() const {
    
    //ADD CODE HERE
    
    //  Gå igenom alla noder i heapen
    for (size_t i = 0; i < pq.size(); ++i) {

        // Beräkna index för barn
        size_t left = 2 * i + 1;
        size_t right = 2 * i + 2;

        // Kontrollera vänster barn
        // Om det finns och är mindre än parent → bryter heap-regeln
        if (left < pq.size() && pq[left] < pq[i]) {
            return false;
        }

        // Kontrollera höger barn
        if (right < pq.size() && pq[right] < pq[i]) {
            return false;
        }
    }

    // Om inga fel hittades → det är en korrekt min-heap
    return true;
}