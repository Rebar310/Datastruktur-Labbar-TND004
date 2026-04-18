#include "set.h"
#include "node.h"

int Set::Node::count_nodes = 0;  // Do not modify this variable anywhere in your code

/*
 * Return number of existing nodes
 * Used solely for debug purposes
 */
int Set::get_count_nodes() {
    return Set::Node::count_nodes;
}

/*****************************************************
 * Implementation of the member functions             *
 ******************************************************/

/*
 *  Default constructor :create an empty Set
 */
Set::Set() : counter{0} {
    // IMPLEMENT before Lab2 HA
    head = new Node{};
    tail = new Node{};

    head->next = tail;
    head->prev = nullptr;

    tail->prev = head;
    tail->next = nullptr;
}

/*
 *  Conversion constructor: convert val into singleton {val}
 */
Set::Set(int val) : Set{} {  // create an empty list
    // IMPLEMENT before Lab2 HA
    insert_node(head, val);
    counter = 1;
}

/*
 * Constructor to create a Set from a sorted vector of ints
 * Create a Set with all ints in vector list_of_values
 * Assume all ints in list_of_values are distinct and sorted in increasing order
 */
Set::Set(const std::vector<int>& list_of_values) : Set{} {  // create an empty list
    // IMPLEMENT before Lab2 HA
    Node* p = head;

    for (int value : list_of_values) {
        insert_node(p, value);
        p = p->next;
        ++counter;
    }
}

/*
 * Copy constructor: create a new Set as a copy of Set S
 * \param S Set to copied
 * Function does not modify Set S in any way
 */
Set::Set(const Set& S) : Set{} {  // create an empty list
    // IMPLEMENT before Lab2 HA
    Node* p_this = head;
    Node* p_S = S.head->next;

    while (p_S != S.tail) {
        insert_node(p_this, p_S->value);
        p_this = p_this->next;
        p_S = p_S->next;
        ++counter;
    }
}

/*
 * Transform the Set into an empty set
 * Remove all nodes from the list, except the dummy nodes
 */
void Set::make_empty() {
    // IMPLEMENT before Lab2 HA
    Node* current = head->next;

    while (current != tail) {
        Node* temp = current;
        current = current->next;
        remove_node(temp);
    }

    counter = 0;
}

/*
 * Destructor: deallocate all memory (Nodes) allocated for the list
 */
Set::~Set() {
    // IMPLEMENT before Lab2 HA
    make_empty();   // tar bort alla riktiga noder
    delete head;    // tar bort dummy head
    delete tail;    // tar bort dummy tail
}

/*
 * Assignment operator: assign new contents to the *this Set, replacing its current content
 * \param S Set to be copied into Set *this
 * Use copy-and swap idiom -- TNG033: lecture 5
 */
Set& Set::operator=(Set S) {
    // IMPLEMENT before Lab2 HA
    std::swap(head, S.head);
    std::swap(tail, S.tail);
    std::swap(counter, S.counter);

    return *this;
}


/*
 * Test whether val belongs to the Set
 * Return true if val belongs to the set, otherwise false
 * This function does not modify the Set in any way
 */
bool Set::is_member(int val) const {
    // IMPLEMENT before Lab2 HA
    Node* current = head->next;

    while (current != tail) {
        if (current->value == val) {
            return true;
        }
        if (current->value > val) {
            return false;
        }
        current = current->next;
    }

    return false;
}

/*
 * Three-way comparison operator: to test whether *this == S, *this < S, *this > S
 * Return std::partial_ordering::equivalent, if *this == S
 * Return std::partial_ordering::less, if *this < S
 * Return std::partial_ordering::greater, if *this > S
 * Return std::partial_ordering::unordered, otherwise
 *
 * Requirement: must iterate through each set no more than once
 */
std::partial_ordering Set::operator<=>(const Set& S) const {
    // IMPLEMENT before Lab2 HA
    Node* p1 = head->next;
    Node* p2 = S.head->next;

    bool this_has_extra = false; // finns element i *this som inte finns i S?
    bool s_has_extra = false;    // finns element i S som inte finns i *this?

    while (p1 != tail && p2 != S.tail) {
        if (p1->value == p2->value) {
            p1 = p1->next;
            p2 = p2->next;
        }
        else if (p1->value < p2->value) {
            this_has_extra = true;
            p1 = p1->next;
        }
        else { // p2->value < p1->value
            s_has_extra = true;
            p2 = p2->next;
        }

        if (this_has_extra && s_has_extra) {
            return std::partial_ordering::unordered;
        }
    }

    while (p1 != tail) {
        this_has_extra = true;
        p1 = p1->next;
    }

    while (p2 != S.tail) {
        s_has_extra = true;
        p2 = p2->next;
    }

    if (!this_has_extra && !s_has_extra) {
        return std::partial_ordering::equivalent;
    }
    if (!this_has_extra && s_has_extra) {
        return std::partial_ordering::less;
    }
    if (this_has_extra && !s_has_extra) {
        return std::partial_ordering::greater;
    }

    return std::partial_ordering::unordered;
}


/*
 * Test whether Set *this and S represent the same set
 * Return true, if *this has same elemnts as set S
 * Return false, otherwise
 *
 * Requirement: must iterate through each set no more than once
 */
bool Set::operator==(const Set& S) const {
    // IMPLEMENT before Lab2 HA
    if (counter != S.counter) {
        return false;
    }

    Node* p1 = head->next;
    Node* p2 = S.head->next;

    while (p1 != tail && p2 != S.tail) {
        if (p1->value != p2->value) {
            return false;
        }
        p1 = p1->next;
        p2 = p2->next;
    }

    return true;
}

/*
 * Modify Set *this such that it becomes the union of *this with Set S
 * Set *this is modified and then returned
 */
Set& Set::operator+=(const Set& S) {
    
    Node* p1 = head->next;      // *this
    Node* p2 = S.head->next;    // S

    while (p1 != tail && p2 != S.tail) {
        if (p1->value == p2->value) {
            p1 = p1->next;
            p2 = p2->next;
        }
        else if (p2->value < p1->value) {
            insert_node(p1->prev, p2->value);
            ++counter;
            p2 = p2->next;
        }
        else { // p1->value < p2->value
            p1 = p1->next;
        }
    }

    // Lägg till resten från S
    while (p2 != S.tail) {
        insert_node(tail->prev, p2->value);
        ++counter;
        p2 = p2->next;
    }

    return *this;
}


/*
 * Modify Set *this such that it becomes the intersection of *this with Set S
 * Set *this is modified and then returned
 */
Set& Set::operator*=(const Set& S) {
    // IMPLEMENT
    Node* p1 = head->next;
    Node* p2 = S.head->next;

    while (p1 != tail && p2 != S.tail) {
        if (p1->value == p2->value) {
            p1 = p1->next;
            p2 = p2->next;
        }
        else if (p1->value < p2->value) {
            Node* temp = p1;
            p1 = p1->next;
            remove_node(temp);
            --counter;
        }
        else {
            p2 = p2->next;
        }
    }

    while (p1 != tail) {
        Node* temp = p1;
        p1 = p1->next;
        remove_node(temp);
        --counter;
    }

    return *this;
}

/*
 * Modify Set *this such that it becomes the Set difference between Set *this and Set S
 * Set *this is modified and then returned
 */
Set& Set::operator-=(const Set& S) {
    // IMPLEMENT
    Node* p1 = head->next;
    Node* p2 = S.head->next;

    while (p1 != tail && p2 != S.tail) {
        if (p1->value == p2->value) {
            Node* temp = p1;
            p1 = p1->next;
            p2 = p2->next;
            remove_node(temp);
            --counter;
        }
        else if (p1->value < p2->value) {
            p1 = p1->next;
        }
        else {
            p2 = p2->next;
        }
    }

    return *this;
}


/* ******************************************** *
 * Private Member Functions -- Implementation   *
 * ******************************************** */

/*
 * Insert a new Node storing val after the Node pointed by p
 * \param p pointer to a Node
 * \param val value to be inserted  after position p
 */
void Set::insert_node(Node* p, int val) {
    // IMPLEMENT before Lab2 HA
    Node* newNode = new Node(val, p->next, p);

    p->next->prev = newNode;
    p->next = newNode;
}

/*
 * Remove the Node pointed by p
 * \param p pointer to a Node
 */
void Set::remove_node(Node* p) {
    // IMPLEMENT before Lab2 HA
    p->prev->next = p->next;
    p->next->prev = p->prev;
    delete p;
}

/*
 * Write Set *this to stream os
 */
void Set::write_to_stream(std::ostream& os) const {
    if (is_empty()) {
        os << "Set is empty!";
    } else {
        Set::Node* ptr{head->next};

        os << "{ ";
        while (ptr != tail) {
            os << ptr->value << " ";
            ptr = ptr->next;
        }
        os << "}";
    }
}
