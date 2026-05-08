/*********************************************
 * file:	~\code4-part2\graph.cpp           *
 * remark:implementation of undirected graphs *
 **********************************************/

#include <iostream>
#include <algorithm>
#include <format>
#include <cassert>  // assert
#include <limits>   // std::numeric_limits

#include <queue> // ja la till 
#include <tuple> // ja la till 
#include <functional> // jag la till

#include "graph.h"
#include "dsets.h"

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
    using PQElement = std::tuple<int, int, int>;
    // weight, from, to

    std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>> pq;

    int start = 1;
    dist[start] = 0;

    pq.push({ 0, 0, start });

    int totalWeight = 0;
    int edgesUsed = 0;

    while (!pq.empty() && edgesUsed < size) {
        auto [weight, from, v] = pq.top();
        pq.pop();

        if (done[v]) {
            continue;
        }

        done[v] = true;

        if (from != 0) {
            std::cout << std::format("({}, {}, {})\n", from, v, weight);
            totalWeight += weight;
            edgesUsed++;
        }

        for (auto const& e : table[v]) {
            int u = e.to;

            if (!done[u] && e.weight < dist[u]) {
                dist[u] = e.weight;
                path[u] = v;
                pq.push({ e.weight, v, u });
            }
        }
    }

    std::cout << "Total weight = " << totalWeight << "\n";
}





// Kruskal's minimum spanning tree algorithm
void Graph::mstKruskal() const {
    // *** TODO ***
    using PQElement = std::tuple<int, int, int>;
    // weight, from, to

    std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>> pq;

    // Lägg in varje kant endast en gång
    for (int v = 1; v <= size; ++v) {
        for (auto const& e : table[v]) {
            if (e.from < e.to) {
                pq.push({ e.weight, e.from, e.to });
            }
        }
    }

    DSets sets(size);

    int totalWeight = 0;
    int edgesUsed = 0;

    while (!pq.empty() && edgesUsed < size - 1) {
        auto [weight, u, v] = pq.top();
        pq.pop();

        int rootU = sets.find(u);
        int rootV = sets.find(v);

        if (rootU != rootV) {
            std::cout << std::format("({}, {}, {})\n", u, v, weight);

            totalWeight += weight;
            edgesUsed++;

            sets.join(rootU, rootV);
        }
    }

    std::cout << "Total weight = " << totalWeight << "\n";
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
