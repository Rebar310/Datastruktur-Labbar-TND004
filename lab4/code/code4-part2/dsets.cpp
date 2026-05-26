/*********************************************
 * file:	~\code4-part2\dsets.cpp           *
 * remark: implementation of disjoint sets    *
 **********************************************/

#include <iostream>
#include <format>
#include <cassert>

#include "dsets.h"

// -- CONSTRUCTORS

DSets::DSets(int theSize) : V(theSize + 1) {  // slot zero is not used
    assert(theSize > 0);
    init();
}

// -- MEMBER FUNCTIONS

// create initial sets
void DSets::init() {

    // Går igenom alla element från 1 till sista index
    for (auto i = 1; i < std::ssize(V); ++i) {

        // V[i] = -1 betyder:
        // 1. i är root för sin egen mängd
        // 2. mängden har storlek 1
        //
        // Negativa värden betyder alltså att noden är root.
        V[i] = -1;  // every disjoint set has one node -- singleton
    }
}

// join sets named r and s where r != s
// i.e. join trees with roots r and s
void DSets::join(int r, int s) {
    assert(r != s); // Kontrollera att det inte är samma root
    assert(r >= 1 && r <= std::ssize(V) - 1); // Kontrollera att r ligger inom giltigt index
    assert(s >= 1 && s <= std::ssize(V) - 1); // Kontrollera att s ligger inom giltigt index
    assert(V[r] < 0); // Kontrollera att r verkligen är root
    assert(V[s] < 0); // Kontrollera att s verkligen är root

    // Den gjorde bara att r pekade på s,
    // utan att ta hänsyn till trädens storlek.
    // simple union (gammal)
    // V[r] = s;

    // *** TODO ***
    // Union by size:
    // V[root] är negativ och representerar storleken på mängden.
    //
    // Exempel:
    // V[r] = -5 betyder att r är root för en mängd med 5 element.
    // V[s] = -2 betyder att s är root för en mängd med 2 element.
    //
    // Eftersom -5 är mindre än -2 betyder:
    // "mindre tal" = större mängd.
    // Do Union by Size
    if (V[r] <= V[s]) {
        // r har större eller lika stort träd
        V[r] += V[s];
        V[s] = r;
    }
    else {
        // s har större träd
        V[s] += V[r];
        V[r] = s;
    }

}

// return name of current set for x
// i.e. return root of tree for x
int DSets::find(int x) {
    assert(x >= 1 && x <= std::ssize(V) - 1);

    // Den hittar rooten, men uppdaterar inte vägen dit.
    // simple find (gammal) 
    /*if (V[x] < 0) {
        return x;
    } else {
        return find(V[x]);
    }*/

    // *** TODO ***
     // Find with path compression:
    // Om V[x] är negativt är x en root.
    // Då har vi hittat mängdens namn/root.
    // find with path compression
    if (V[x] < 0) {
        return x;
    }

    // Om x inte är root, så pekar V[x] på x:s parent.
    // find(V[x]) går vidare uppåt i trädet tills root hittas.
    // Sedan sparas rooten direkt i V[x].
    // Det kallas path compression.
    V[x] = find(V[x]);
    return V[x]; // Returnera rooten
}

// just in case ...
void DSets::print() const {
    std::cout << "\n";

    for (auto i = 1; i < std::ssize(V); i++) {
        std::cout << std::format("{:4}", i);
    }
    std::cout << "\n";

    for (auto u : V) {
        std::cout << std::format("{:4}", u);
    }
    std::cout << "\n";
}
