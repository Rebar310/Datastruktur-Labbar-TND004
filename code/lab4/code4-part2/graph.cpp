/*********************************************
 * file:	~\code4-part2\graph.cpp           *
 * remark:implementation of undirected graphs *
 **********************************************/

#include <iostream>
#include <algorithm>
#include <format>
#include <cassert>  // assert
#include <limits>   // std::numeric_limits

#include "graph.h"
#include "dsets.h"

#include <queue> // för std::queue
#include <tuple> // för std::tuple
// Note: graph vertices are numbered from 1 -- i.e. there is no vertex zero

// -- CONSTRUCTORS

// Create a graph with n vertices and no vertices
Graph::Graph(int n) : table(n + 1), size{n}, n_edges{0} {
    assert(n >= 1);
}

Graph::Graph(const std::vector<Edge> &V, int n) : Graph{n} {
    for (auto e : V) {
        insertEdge(e);
    }
}

// -- MEMBER FUNCTIONS

/*
 * insert undirected edge e
 * update weight if edge e is present
 */
void Graph::insertEdge(const Edge &e) {
    assert(e.from >= 1 && e.from <= size);
    assert(e.to >= 1 && e.to <= size);

    // Check if a given edge e1 already exists
    auto edge_insertion = [&T = this->table, &n = this->n_edges](const Edge &e1) {
        if (auto it = std::find_if(begin(T[e1.from]), end(T[e1.from]),
                                   [e1](const Edge &ed) { return e1.links_same_nodes(ed); });
            it == end(T[e1.from])) {
            T[e1.from].push_back(e1);  // insert new edge e1
            ++n;                       // increment the counter of edges
        } else {
            it->weight = e1.weight;  // update the weight
        }
    };

    edge_insertion(e);
    edge_insertion(e.reverse());  // graph is undirected
}

// Prim's minimum spanning tree algorithm
void Graph::mstPrim() const {
    std::vector<int> dist(size + 1, std::numeric_limits<int>::max());
    std::vector<int> path(size + 1, 0);
    std::vector<bool> done(size + 1, false);

    // *** TODO ***

    int startNode = 1;
    dist[startNode] = 0;
    //done[startNode] = true;

    //skapa ett par, vikt och nod, och en kö för att hålla dessa par

    using Pair = std::pair<int, std::pair<int, int>>; // {vikt, {från_nod, till_nod}}

    // Skapa en min-prioritetskö som sorterar på lägsta vikt först
    std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> pq; // min-heap

    //lägger till startnoden i kön
    pq.push({ 0, { 0, startNode } });
	int totalWeight = 0;

    while (!pq.empty()) {
        // Hämta värdena från det nästlade paret
        int weight = pq.top().first;
        int from = pq.top().second.first;
        int u = pq.top().second.second;

        pq.pop();       //ta bort det elementet från kön

        // Om noden redan är klar, hoppa över den
        if (done[u]) {
            continue;
        }

        done[u] = true; // Markera noden som klar

        // Om kanten har en giltig ursprungsnod (inte 0), skriv ut direkt!
        if (from != 0) {
            std::cout << std::format("({:2}, {:2}, {:2})\n", from, u, weight);
            totalWeight += weight;
        }

        // Gå igenom alla grannar till u
        for (const auto& e : table[u]) {
            int v = e.to;
            int weight = e.weight;
            // Om grannen inte är klar och vikten är mindre än det nuvarande avståndet
            if (!done[v] && weight < dist[v]) {
                dist[v] = weight; // Uppdatera avståndet
                path[v] = u; // Spara vägen
                pq.push({ dist[v], {u,v} }); // Lägg grannen i kön med uppdaterat avstånd
            }
        }
    }

    std::cout << std::format("\nTotal weight = {}\n", totalWeight);
}

// Kruskal's minimum spanning tree algorithm
void Graph::mstKruskal() const {
    // *** TODO ***
    using Elements = std::tuple<int, int, int>; // {vikt, från_nod, till_nod}

    // Skapa en min-prioritetskö som sorterar på lägsta vikt först
    std::priority_queue<Elements, std::vector<Elements>, std::greater<Elements>> pq; // min-heap
    
	//går igenom alla kanter i grafen och lägger till dem i kön
	for (int v = 1; v <= size; ++v) {
		for (const auto& e : table[v]) {
			if (e.from < e.to) { // För att undvika dubbletter i en undirected graph
				pq.push({ e.weight, -e.from, -e.to });
			}
		}
	}

	//skapar en disjoint set för att hålla koll på vilka noder som är i samma träd
	DSets dsets(size);

	int totalWeight = 0;
    int edgesUsed = 0;

	std::cout << "\n";

	//hämta de kanter i ordning av vikt
	while (!pq.empty() && edgesUsed < size - 1) {
        int weight = std::get<0>(pq.top());
        // Lägg till ett minustecken framför för att återställa till det positiva originalnumret
        int u = -std::get<1>(pq.top());
        int v = -std::get<2>(pq.top());

		pq.pop();

        //hitta rötterna
		int rootU = dsets.find(u);
		int rootV = dsets.find(v);

		//kolla om from och to är i olika träd
		if (rootU != rootV) {
            dsets.join(rootU, rootV); // Slå ihop träden

			std::cout << std::format("({:2}, {:2}, {:2})\n", u, v, weight);
			totalWeight += weight; // Lägg till vikten i totalen
			edgesUsed++; // Öka antalet använda kanter

            
		}
	}
    std::cout << std::format("\nTotal weight = {}\n", totalWeight);
}

// print graph
void Graph::printGraph() const {
    std::cout << std::format("{:-<66}\n", '-');
    std::cout << "Vertex  adjacency lists\n";
    std::cout << std::format("{:-<66}\n", '-');

    for (int v = 1; v <= size; v++) {
        std::cout << std::format("{:4} : ", v);
        for (auto const &e : table[v]) {
            std::cout << std::format("({:2}, {:2}) ", e.to, e.weight);
        }
        std::cout << "\n";
    }
    std::cout << std::format("{:-<66}\n", '-');
}
