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
    for (auto i = 1; i < std::ssize(V); ++i) {
        V[i] = -1;  // every disjoint set has one node -- singleton
    }
}

// join sets named r and s where r != s
// i.e. join trees with roots r and s
void DSets::join(int r, int s) {
    assert(r != s);
    assert(r >= 1 && r <= std::ssize(V) - 1);
    assert(s >= 1 && s <= std::ssize(V) - 1);
    assert(V[r] < 0);
    assert(V[s] < 0);

    // simple union (gammal)
    //V[r] = s;

    // *** TODO ***
    // Do Union by Size
	if (V[r] < V[s]) { // r har mer noder än s
		V[r] += V[s]; // uppdatera storleken på r
		V[s] = r; // gör r till den nya roten
	}
	else { // s har mer noder än r
		V[s] += V[r]; // uppdatera storleken på s
		V[r] = s; // gör s till den nya roten
	}

}

// return name of current set for x
// i.e. return root of tree for x
int DSets::find(int x) {
    assert(x >= 1 && x <= std::ssize(V) - 1);

    // simple find (gamal)
  /*  if (V[x] < 0) {
        return x;
    } else {
        return find(V[x]);
    }*/

    // *** TODO ***
    // find with path compression

	if (V[x] < 0) {
		return x;   //x är redan en rot
	} else {
		//sparar roten i en variabel för att kunna använda den senare
		V[x] = find(V[x]); 
		return V[x];
	}

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
