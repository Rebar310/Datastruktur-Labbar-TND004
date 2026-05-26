/*********************************************
 * file:	~\code4-part1\digraph.cpp         *
 * remark: implementation of directed graphs  *
 **********************************************/

#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>
#include <format>

#include <queue> // lade till denna 
#include <limits> // lade till denna 

#include "digraph.h"

// Note: graph vertices are numbered from 1 -- i.e. there is no vertex zero

// -- CONSTRUCTORS

// n = antalet noder
Digraph::Digraph(int n)
    : table(n + 1)  // slot zero not used, Skapar adjacency list med n + 1 platser.
    , size{n}       // // Sparar antal noder(verticies) i grafen
    , n_edges{0}    // Från början finns inga kanter
    , dist(n + 1)   // Vektor för kortaste avstånd från startnod
    , path(n + 1)   // Vektor för föregående nod på kortaste vägen
    , done(n + 1) { // Vektor som markerar om en nod har besökts

    assert(n >= 1);// Säkerställer att grafen har minst en nod
    
}

// Create a digraph with n vertices and the edges in V
Digraph::Digraph(const std::vector<Edge>& V, int n) 
    : Digraph{n} { // Anropar först konstruktorn ovan för att skapa tom graf

    // Går igenom alla kanter i vektorn V
    for (auto e : V) {
        insertEdge(e); // Lägger in varje kant i grafen
    }
}

// -- MEMBER FUNCTIONS

 /*
 * Lägger in en riktad kant e = (from, to, weight)
 * insert directed edge e = (u, v, w)
 * update weight w if edge (u, v) is present
 */
void Digraph::insertEdge(const Edge& e) {
    assert(e.from >= 1 && e.from <= size); // Kontrollerar att startnoden finns i grafen
    assert(e.to >= 1 && e.to <= size); // Kontrollerar att slutnoden finns i grafen

    // Check if edge e already exists 
    if (auto it = std::find_if(begin(table[e.from]), end(table[e.from]),
                               [e](const Edge& ed) { return e.links_same_nodes(ed); });
        it == end(table[e.from])) { // Om kanten inte hittades
        table[e.from].push_back(e);  // Lägg in kanten i adjacency listan för startnoden
        ++n_edges;
    } else {
        it->weight = e.weight;  // Om kanten redan finns, uppdatera endast vikten
    }
}

// construct unweighted single source shortest path-tree for start vertex s
// Funktionen konstruerar shortest path tree från startnoden s.
// Eftersom grafen behandlas som oviktad används BFS.
void Digraph::uwsssp(int s) const { // O(V+E) anatl noder + antal edges
    assert(s >= 1 && s <= size); // Kontrollera att startnoden finns i grafen

   // ADD CODE ****************************************************

    // Initialisera/ går igenom alla noder , v (verticies) är noder
    for (int v = 1; v <= size; ++v) {
        dist[v] = std::numeric_limits<int>::max(); // Sätt avståndet till oändligt då vi ännu inte vet någon väg till noden
        path[v] = 0;  // Sätt föregående nod till 0, 0 används som "ingen föregångare"
        done[v] = false; // Markera alla noder som obesökta
    }

    // Queue för BFS, BFS använder FIFO: first in, first out.
    std::queue<int> q;

    // Startnod S
    dist[s] = 0; // Avståndet från startnoden till sig själv är 0
    done[s] = true; // Startnoden markeras som besökt

    q.push(s); // Lägg startnoden i kön

    // BFS (Breadth First Search) , Kör så länge det finns noder kvar i kön
    while (!q.empty()) {

        int v = q.front(); // Hämta första noden i kön
        q.pop(); // Ta bort noden från kön

        // Gå igenom alla grannar
        for (auto const& e : table[v]) { // Gå igenom alla kanter som går ut från nod v

            // e.to är noden som kanten går till
            int u = e.to;

            // Om nod u inte har besökts tidigare
            if (!done[u]) {

                done[u] = true; // Markera u som besökt

                // Eftersom grafen är oviktad är varje kant ett steg.
                // Avståndet till u blir därför avståndet till v + 1.
                dist[u] = dist[v] + 1;
                path[u] = v; // Spara att vi kom till u från v.

                q.push(u); // Lägg u i kön så att dess grannar också kan undersökas
            }
        }
    }

}


// print graph
void Digraph::printGraph() const {
    std::cout << std::format("{:-<66}\n", '-');
    std::cout << "Vertex  adjacency lists\n";
    std::cout << std::format("{:-<66}\n", '-');

    for (int v = 1; v <= size; ++v) {
        std::cout << std::format("{:4} : ", v);
        for (auto const& e : table[v]) {
            std::cout << std::format("({:2}, {:2}) ", e.to, e.weight);
        }
        std::cout << "\n";
    }
    std::cout << std::format("{:-<66}\n", '-');
}

// print shortest path tree for s
void Digraph::printTree() const {
    std::cout << std::format("{:-<22}\n", '-');
    // std::cout << "----------------------\n";
    std::cout << "vertex    dist    path\n";
    std::cout << std::format("{:-<22}\n", '-');
    // std::cout << "----------------------\n";

    for (int v = 1; v <= size; ++v) {
        std::cout << std::format("{:4} : {:6} {:6}\n", v,
                                 ((dist[v] == std::numeric_limits<int>::max()) ? -1 : dist[v]),
                                 path[v]);
    }
    std::cout << std::format("{:-<22}\n", '-');
    // std::cout << "----------------------\n";
}

// print shortest path from s to t and the corresponding path length
// Hint: consider using recursion
void Digraph::printPath(int t) const {
    assert(t >= 1 && t <= size);

    // ADD CODE ********************************************

    // Om dist[t] fortfarande är oändligt finns ingen väg till t
    if (dist[t] == std::numeric_limits<int>::max()) {
        std::cout << " no path\n";
        return;
    }

    printHelp(t);

    std::cout << "\nPath length = " << dist[t] << "\n";
}

//Hjälpfunktion för att kunna göra rekrusion i funktionen ovan.
void Digraph::printHelp(int t) const {

    // Om nod t har en föregående nod,
    // skriv först ut vägen till den föregående noden.
    if (path[t] != 0) {
        printHelp(path[t]);
    }

    std::cout << " " << t;
}