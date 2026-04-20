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
    // anropar konstruktorn med default värden
    head = new Node{}; // Skapar en dummy "head"-nod (början av listan)
    tail = new Node{}; // Skapar en dummy "tail"-nod (slutet av listan)

    head->next = tail; // Head ska peka framåt på tail
    head->prev = nullptr;// Head är första noden → inget före den

    tail->prev = head; // Tail ska peka bakåt på head
    tail->next = nullptr; // Tail är sista noden → inget efter den
}

/*
 *  Conversion constructor: convert val into singleton {val}
 */
Set::Set(int val) : Set{} {  // create an empty list ( alltså kör defualt constructor)
    // IMPLEMENT before Lab2 HA
    insert_node(head, val); // Lägger in en ny nod med värdet val direkt efter head
    counter = 1; // Setet innehåller nu ett exakt element
}

/*
 * Constructor to create a Set from a sorted vector of ints
 * Create a Set with all ints in vector list_of_values
 * Assume all ints in list_of_values are distinct and sorted in increasing order
 */
Set::Set(const std::vector<int>& list_of_values) : Set{} {  // create an empty list
    // IMPLEMENT before Lab2 HA

    // Loopa igenom alla värden i vektorn
    for (int val : list_of_values) {
        insert_node(tail->prev, val);  // lägg till sist, precis innan tail varje gång
        ++counter;                     // uppdatera storlek
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
        ++counter;
    }
}

/*
 * Transform the Set into an empty set
 * Remove all nodes from the list, except the dummy nodes
 */
void Set::make_empty() {
    // IMPLEMENT before Lab2 HA
    Node* current = head->next; // börja på första riktiga noden

    while (current != tail) { // fortsätt tills vi når dummy tail
        Node* nodeToRemove = current; // spara noden som ska tas bort
        current = current->next; // gå vidare innan noden tas bort
        remove_node(nodeToRemove); // ta bort noden ur listan
    }

    counter = 0; // listan är nu tom
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
    // OBS: S skickas in "by value"
    // → det betyder att en KOPIA av S redan har skapats (via copy constructor)
    
    // IMPLEMENT before Lab2 HA
    std::swap(head, S.head); // Byt ut vår listas head med kopians head
    std::swap(tail, S.tail); // Samma sak för tail (dummy-noden i slutet)
    std::swap(counter, S.counter); // Byt antal element så att *this får rätt storlek

    return *this;  // Returnera referens till nu uppdaterade objektet

}


/*
 * Test whether val belongs to the Set
 * Return true if val belongs to the set, otherwise false
 * This function does not modify the Set in any way
 */
bool Set::is_member(int val) const {
    // IMPLEMENT before Lab2 HA
    Node* current = head->next; // Börja vid första riktiga noden (efter dummy head)

    while (current != tail) { // Loopa igenom alla noder tills vi når dummy tail
        if (current->value == val) { 
            // Om vi hittar värdet → det finns i mängden
            return true;
        }
        if (current->value > val) {
            // Om current->value redan är större än val
            // → då kommer val aldrig dyka upp längre fram
            // detta eftersom listan är sorterad
            return false;
        }
        current = current->next; // Gå vidare till nästa nod i listan

    }

    return false;
    // Om vi gått igenom hela listan utan att hitta val
   // → då finns det inte i Set
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
    Node* p1 = head->next;     // Pekare till första riktiga noden i *this
    Node* p2 = S.head->next; // Pekare till första riktiga noden i S

    bool this_has_extra = false; // finns element i *this som inte finns i S?
    bool s_has_extra = false;    // finns element i S som inte finns i *this?

    while (p1 != tail && p2 != S.tail) {  // Så länge båda listorna fortfarande har element kvar

        if (p1->value == p2->value) {
            // Samma värde i båda mängderna
            // → gå vidare i båda
            p1 = p1->next;
            p2 = p2->next;
        }
        else if (p1->value < p2->value) {
            // Ett värde finns i *this som inte finns i S just här
            // → *this har ett "extra" element
            this_has_extra = true;
            p1 = p1->next;
        }
        else { // p2->value < p1->value
            // Ett värde finns i S som inte finns i *this just här
            // → S har ett "extra" element
            s_has_extra = true;
            p2 = p2->next;
        }

        if (this_has_extra && s_has_extra) {
            // Om båda mängderna har egna extra element
            // → ingen är delmängd av den andra
            return std::partial_ordering::unordered;
        }
    }

    while (p1 != tail) {
        // Om *this fortfarande har element kvar efter att S tog slut
        // → *this har extra element
        this_has_extra = true;
        p1 = p1->next;
    }

    while (p2 != S.tail) {
        // Om S fortfarande har element kvar efter att *this tog slut
        // → S har extra element
        s_has_extra = true;
        p2 = p2->next;
    }

    if (!this_has_extra && !s_has_extra) {
        // Inga extra element i någon mängd
        // → exakt samma innehåll
        return std::partial_ordering::equivalent;
    }
    if (!this_has_extra && s_has_extra) {
        // *this har inga egna extra element,
        // men S har fler
        // → *this är delmängd av S
        return std::partial_ordering::less;
    }
    if (this_has_extra && !s_has_extra) {
        // S har inga egna extra element,
        // men *this har fler
        // → *this är supermängd av S
        return std::partial_ordering::greater;
    }

    return std::partial_ordering::unordered;
    // Säkerhetsretur om båda skulle ha extra element

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
        // Om de har olika antal element
        // → kan de inte vara lika
        return false;
    }

    Node* p1 = head->next;   // Pekare till första riktiga noden i *this
    Node* p2 = S.head->next; // Pekare till första riktiga noden i S


    while (p1 != tail && p2 != S.tail) {  // Gå igenom båda listorna samtidigt

        if (p1->value != p2->value) {
            // Om något värde skiljer sig
            // → mängderna är inte lika
            return false;
        }
        p1 = p1->next; // Gå vidare i första listan
        p2 = p2->next; // Gå vidare i andra listan
    }

    return true;
    // Om alla värden matchade
    // → mängderna är lika
}


/*
 * Modify Set *this such that it becomes the union of *this with Set S
 * Set *this is modified and then returned
 */
Set& Set::operator+=(const Set& S) {
    
    Node* p1 = head->next;  // Pekare i *this (första riktiga noden)
    Node* p2 = S.head->next; // Pekare i S (första riktiga noden)

    while (p1 != tail && p2 != S.tail) {  // Gå igenom båda listorna samtidigt

        if (p1->value == p2->value) {
            // Samma värde finns redan i båda
            // → hoppa över (ingen dubblett ska läggas till)
            p1 = p1->next;
            p2 = p2->next;
        }
        else if (p2->value < p1->value) {
            // Ett värde finns i S som inte finns i *this ännu
            // → lägg in det före p1 (rätt sorterad position)
            insert_node(p1->prev, p2->value);
            ++counter;
            p2 = p2->next; // gå vidare i S
        }
        else { // p1->value < p2->value
            // Värdet i *this är mindre → det är redan korrekt
           // → gå vidare i *this
            p1 = p1->next;
        }
    }

    // Lägg till resten från S (om S har fler element kvar)
    while (p2 != S.tail) {
        // Alla dessa är större än allt i *this
        // → lägg till sist
        insert_node(tail->prev, p2->value);
        ++counter;
        p2 = p2->next;
    }

    return *this; // Returnera den uppdaterade mängden

}


/*
 * Modify Set *this such that it becomes the intersection of *this with Set S
 * Set *this is modified and then returned
 */
Set& Set::operator*=(const Set& S) {
    // IMPLEMENT
    Node* p1 = head->next; // Pekare till första riktiga noden i *this
    Node* p2 = S.head->next;  // Pekare till första riktiga noden i S

    while (p1 != tail && p2 != S.tail) {     // Gå igenom båda listorna samtidigt så länge båda har element kvar

        if (p1->value == p2->value) {
            // Samma värde finns i båda mängderna
            // → det ska vara kvar i snittet
            p1 = p1->next;
            p2 = p2->next;
        }
        else if (p1->value < p2->value) {
            // Värdet i *this är mindre än värdet i S
            // → då finns detta värde inte i S
            // → ta bort det från *this
            Node* temp = p1; // spara noden som ska tas bort
            p1 = p1->next; // gå vidare innan noden tas bort
            remove_node(temp); // ta bort noden ur *this
            --counter; // minska antal element
        }
        else {
            // p2->value < p1->value
           // Värdet i S är mindre
           // → gå vidare i S tills vi kanske hittar samma värde som p1
            p2 = p2->next;
        }
    }

    // Om S är slut men *this fortfarande har element kvar,
    // så finns de inte i S och ska därför tas bort
    while (p1 != tail) {
        Node* temp = p1;// spara noden som ska tas bort
        p1 = p1->next; // gå vidare innan borttagning
        remove_node(temp); // ta bort noden
        --counter; // minska antal element
    }

    // Returnera den modifierade mängden
    return *this;
}

/*
 * Modify Set *this such that it becomes the Set difference between Set *this and Set S
 * Set *this is modified and then returned
 */
Set& Set::operator-=(const Set& S) {
    // IMPLEMENT
    Node* p1 = head->next; // Pekare i *this (börjar på första riktiga noden)
    Node* p2 = S.head->next; // Pekare i S

    while (p1 != tail && p2 != S.tail) { // Gå igenom båda listorna samtidigt

        if (p1->value == p2->value) {
            // Samma värde finns i båda
            // → ska tas bort från *this (eftersom vi gör differens)
            Node* temp = p1; // spara noden som ska tas bort
            p1 = p1->next; // gå vidare innan borttagning
            p2 = p2->next; // gå vidare i S också
            remove_node(temp); // ta bort noden från *this
            --counter; // minska antal element
        }
        else if (p1->value < p2->value) {
            // Värdet i *this är mindre
            // → det finns inte i S (än)
            // → ska behållas
            p1 = p1->next;
        }
        else {
            // p2->value < p1->value
            // → gå vidare i S för att kanske hitta match
            p2 = p2->next;
        }
    }

    return *this;    // Returnera den uppdaterade mängden

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
    // Skapa en ny nod med värdet val
    // newNode->next ska peka på noden som tidigare låg efter p
    // newNode->prev ska peka tillbaka på p
    Node* newNode = new Node(val, p->next, p);

    p->next->prev = newNode; // Noden som tidigare låg efter p ska nu peka tillbaka på newNode

    p->next = newNode; // p ska nu peka framåt på newNode

}

/*
 * Remove the Node pointed by p
 * \param p pointer to a Node
 */
void Set::remove_node(Node* p) {
    // IMPLEMENT before Lab2 HA
    p->prev->next = p->next;  // Noden före p ska hoppa över p och peka på noden efter p
    p->next->prev = p->prev; // Noden efter p ska hoppa över p och peka tillbaka på noden före p

    delete p;  // Frigör minnet för noden p
}

/*
 * Write Set *this to stream os
 */
void Set::write_to_stream(std::ostream& os) const {
    if (is_empty()) {
        // Om mängden är tom, skriv ut ett meddelande
        os << "Set is empty!";
    } else {
        // Börja på första riktiga noden
        Set::Node* ptr{head->next};

        os << "{ ";
        while (ptr != tail) {
            // Skriv ut värdet i varje nod
            os << ptr->value << " ";

            // Gå vidare till nästa nod
            ptr = ptr->next;
        }
        os << "}";
    }
}
