/*********************************************
 * file:	~\code4-part1\digraph.cpp         *
 * remark: implementation of directed graphs  *
 **********************************************/

#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>
#include <format>

#include <queue> // lade till denna för std::queue
#include <limits> // lade till denna för std::numeric_limits

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

   // ADD CODE
    // Initialisera alla noder
    for (int v = 1; v <= size; ++v) {
        dist[v] = std::numeric_limits<int>::max();
        path[v] = 0;
        done[v] = false;
    }

    //startnode 
	dist[s] = 0;  
	done[s] = true;

    std::queue<int> q;

	// Lägg startnoden i kön
	q.push(s);


	// BFS
	while (!q.empty()) {
		int v= q.front();
		q.pop();
        
		//går igenom alla grannar till v
		for (const auto& e : table[v]) {
			int u = e.to;

			//om ej besökt, uppdatera avstånd, väg och markera som besökt
			if (!done[u]) {
				dist[u] = dist[v] + 1; // Eftersom det är unweighted, öka avståndet med 1
				path[u] = v; // Spara vägen
				done[u] = true; // Markera noden som klar
				q.push(u); // Lägg noden i kön
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

    // ADD CODE

	//om det inte finns någon väg från s till t, skriv ut det
	if (dist[t] == std::numeric_limits<int>::max()) {
		std::cout << " No path from s to " << t << "\n";
	}
	else {
		printHelp(t);
		std::cout << "(" <<dist[t]<<")"<<"\n";
	}
}

void Digraph::printHelp(int t) const {
    if (t == 0) {
		return; // Basfall: ingen väg
	}
	printHelp(path[t]); // Rekursivt anropa för att skriva ut vägen
	std::cout <<" " << t << " "; // Skriv ut noden
}
