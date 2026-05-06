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

Digraph::Digraph(int n)
    : table(n + 1)  // slot zero not used
    , size{n}       // number of vertices
    , n_edges{0}
    , dist(n + 1)
    , path(n + 1)
    , done(n + 1) {
    assert(n >= 1);
    // Note: graph vertices are numbered from 1 -- i.e. there is no vertex zero
}

// Create a digraph with n vertices and the edges in V
Digraph::Digraph(const std::vector<Edge>& V, int n) : Digraph{n} {
    for (auto e : V) {
        insertEdge(e);
    }
}

// -- MEMBER FUNCTIONS

 /*
 * insert directed edge e = (u, v, w)
 * update weight w if edge (u, v) is present
 */
void Digraph::insertEdge(const Edge& e) {
    assert(e.from >= 1 && e.from <= size);
    assert(e.to >= 1 && e.to <= size);

    // Check if edge e already exists 
    if (auto it = std::find_if(begin(table[e.from]), end(table[e.from]),
                               [e](const Edge& ed) { return e.links_same_nodes(ed); });
        it == end(table[e.from])) {
        table[e.from].push_back(e);  // insert new edge e
        ++n_edges;
    } else {
        it->weight = e.weight;  // update the weight, if the edge e already exists
    }
}

// construct unweighted single source shortest path-tree for start vertex s
void Digraph::uwsssp(int s) const {
    assert(s >= 1 && s <= size);

   // ADD CODE ****************************************************

    // Initialisera alla noder
    for (int v = 1; v <= size; ++v) {
        dist[v] = std::numeric_limits<int>::max();
        path[v] = 0;
        done[v] = false;
    }

    // Queue för BFS
    std::queue<int> q;

    // Startnod
    dist[s] = 0;
    done[s] = true;

    q.push(s);

    // BFS (Breadth First Search)
    while (!q.empty()) {

        int v = q.front();
        q.pop();

        // Gå igenom alla grannar
        for (auto const& e : table[v]) {

            int u = e.to;

            // Om ej besökt
            if (!done[u]) {

                done[u] = true;

                dist[u] = dist[v] + 1;

                path[u] = v;

                q.push(u);
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

    // Ingen väg finns
    if (dist[t] == std::numeric_limits<int>::max()) {
        std::cout << " no path\n";
        return;
    }

    printHelp(t);

    std::cout << "\nPath length = " << dist[t] << "\n";
}

//Hjälpfunktion för att kunna göra rekrusion i funktionen ovan.
void Digraph::printHelp(int t) const {

    if (path[t] != 0) {
        printHelp(path[t]);
    }

    std::cout << " " << t;
}