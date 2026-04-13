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
	head = new Node(0, nullptr, nullptr);  // skapa dummy heah
	tail = new Node(0, nullptr, head);  // skapa dummy tail
	head->next = tail;  // koppla head och tail
}

/*
 *  Conversion constructor: convert val into singleton {val}
 */
//tar en ett heltal
Set::Set(int val) : Set{} {  // create an empty list
    // IMPLEMENT before Lab2 HA
	insert_node(head, val);  // infoga val efter dummy head
}

/*
 * Constructor to create a Set from a sorted vector of ints
 * Create a Set with all ints in vector list_of_values
 * Assume all ints in list_of_values are distinct and sorted in increasing order
 */
 //för test fas 1, tar en vektor av heltal
Set::Set(const std::vector<int>& list_of_values) : Set{} {  // create an empty list
    // IMPLEMENT before Lab2 HA
	for (int val : list_of_values) {
        insert_node(tail->prev, val);  // infoga varje val innan dummy tail
    }
}

/*
 * Copy constructor: create a new Set as a copy of Set S
 * \param S Set to copied
 * Function does not modify Set S in any way
 */
Set::Set(const Set& S) : Set{} {  // create an empty list
    // IMPLEMENT before Lab2 HA
	Node* ptr = S.head->next;  // startar från första riktiga noden i S
    while (ptr != S.tail) {  // loopar tills vi når dummy tail
        insert_node(tail->prev, ptr->value);  // infoga varje val innan dummy tail
        ptr = ptr->next;  // gå vidare till nästa nod
    }

}

/*
 * Transform the Set into an empty set
 * Remove all nodes from the list, except the dummy nodes
 */
void Set::make_empty() {
    // IMPLEMENT before Lab2 HA
	Node* current = head->next;  // startar från första riktiga noden

    while (current != tail) {  // loopar tills vi når dummy tail
        Node* nextNode = current->next;  // spara pekaren till nästa nod innan vi tar bort den nuvarande noden
        remove_node(current);  // ta bort den nuvarande noden
        current = nextNode;  // gå vidare till nästa nod
	}
}

/*
 * Destructor: deallocate all memory (Nodes) allocated for the list
 */
Set::~Set() {
    // IMPLEMENT before Lab2 HA
	make_empty();  // töm mängden på alla riktiga noder
	delete head;  // ta bort dummy head
	delete tail; // ta bort dummy tail
}

/*
 * Assignment operator: assign new contents to the *this Set, replacing its current content
 * \param S Set to be copied into Set *this
 * Use copy-and swap idiom -- TNG033: lecture 5
 */
Set& Set::operator=(Set S) {
    // IMPLEMENT before Lab2 HA
	std::swap(head, S.head);  // byt head pekare
	std::swap(tail, S.tail);  // byt tail pekare
	std::swap(counter, S.counter);  // byt counter
    return *this;
}

/*
 * Test whether val belongs to the Set
 * Return true if val belongs to the set, otherwise false
 * This function does not modify the Set in any way
 */
bool Set::is_member(int val) const {
    // IMPLEMENT before Lab2 HA
	Node* ptr = head->next;  // startar från första riktiga noden

    while(ptr!=tail && ptr->value < val) {  // loopar tills vi når dummy tail eller hittar en nod med värde större än val
       
        ptr = ptr->next; // gå vidare till nästa nod
	}
	return (ptr != tail && ptr->value == val);  // returnera true om vi hittade en nod med värde lika med val, annars false
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
 
	//om *this är en delmängd av S så returnerar vi less
	bool is_sub = true;  // flagga för att kontrollera om *this är en delmängd av S
	bool is_super = true;  // flagga för att kontrollera om S är en delmängd av *this

	Node* ptr1 = head->next;  // startar från första riktiga noden i *this
	Node* ptr2 = S.head->next;  // startar från första riktiga noden i S
    //&& ptr1!=nullptr && ptr2!=nullptr
    while (ptr1 != tail && ptr2 != S.tail ) {
        if (ptr1->value < ptr2->value) {  
           //värde finns i *this men inte i s, så *this kan inte vara en delmängd av S
            is_sub = false;
            ptr1 = ptr1->next;
        }
        else if(ptr2->value < ptr1->value){  // om vi hittar en nod i S som inte finns i *this
            is_super = false;
            ptr2 = ptr2->next;
        }
        else {
            //lika värden
			ptr1 = ptr1->next;  // gå vidare till nästa nod i *this
			ptr2 = ptr2->next;  // gå vidare till nästa nod i S
        }
    }

    if (ptr1 != tail) is_sub = false;   // *this har mer skräp kvar
    if (ptr2 != S.tail) is_super = false; // S har mer skräp kvar

    if (is_sub && is_super) return std::partial_ordering::equivalent;
    if (is_sub) return std::partial_ordering::less;
    if (is_super) return std::partial_ordering::greater;

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
    //om de har olika antal element
    if (counter != S.counter) {
        return false;
	}
	Node* ptr1 = head->next;  // startar från första riktiga noden i *this
	Node* ptr2 = S.head->next;  // startar från första riktiga noden i S
	//går igenom båda listorna samtidigt och jämför värdena
	while (ptr1 != tail && ptr2 != S.tail) {
        if (ptr1->value != ptr2->value) {  // om vi hittar en nod med olika värde
            return false;
        }
		ptr1 = ptr1->next;  // gå vidare till nästa nod i *this
		ptr2 = ptr2->next;  // gå vidare till nästa nod i S
	}

	return true;  // alla noder har samma värde, så mängderna är lika
}

/*
 * Modify Set *this such that it becomes the union of *this with Set S
 * Set *this is modified and then returned
 */
Set& Set::operator+=(const Set& S) {
    // IMPLEMENT
    return *this;
}

/*
 * Modify Set *this such that it becomes the intersection of *this with Set S
 * Set *this is modified and then returned
 */
Set& Set::operator*=(const Set& S) {
    // IMPLEMENT
    return *this;
}

/*
 * Modify Set *this such that it becomes the Set difference between Set *this and Set S
 * Set *this is modified and then returned
 */
Set& Set::operator-=(const Set& S) {
    // IMPLEMENT
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
	//skapa en ny node som pekar på den node som p pekar på, p(prev), p->next (next)
	Node* newNode = new Node(val, p->next, p);
    //koppla grannarna
	p->next->prev = newNode;
    p->next = newNode;

	counter++;  //ökar storleken på mängden
}

/*
 * Remove the Node pointed by p
 * \param p pointer to a Node
 */
void Set::remove_node(Node* p) {
    // IMPLEMENT before Lab2 HA
	//koppla grannarna
	p->prev->next = p->next;
	p->next->prev = p->prev;

	delete p;  // frigör minnet som noden upptar
	counter--;  // minskar storleken på mängden
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
