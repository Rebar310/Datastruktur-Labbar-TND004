/*********************************************
 * file:	~\code4-part1\digraph.h           *
 * remark: interface for directed graphs      *
 **********************************************/

#pragma once

#include <list>
#include <vector>

#include "edge.h"

class Digraph {
public:
    // -- CONSTRUCTOR
    // Create a digraph with n vertices and the edges in V
    Digraph(const std::vector<Edge>& V, int n);

    // Disallow copying
    Digraph(const Digraph&) = delete;
    Digraph& operator=(const Digraph&) = delete;

    // -- MEMBER FUNCTIONS

    /*
     * insert directed edge e = (u, v, w)
     * update weight w if edge (u, v) is present
     */
    void insertEdge(const Edge& e);

    // construct unweighted single source shortest path-tree for start vertex s
    void uwsssp(int s) const;

    // print graph
    void printGraph() const;

    // print shortest path tree for s
    void printTree() const;

    // print shortest path from s to t and the corresponding path length
    void printPath(int t) const;

private:
    // -- DATA MEMBERS
    std::vector<std::list<Edge>> table;  // table of adjacency lists
    int size;                            // number of vertices
    int n_edges;                         // number of edges

    /*
     * vectors below can be modified by const - member functions
     * these vectors are used by member function uwsssp to store info related to shortest paths
     * member functions printTree and printPath need also access to these vectors
     */
    mutable std::vector<int> dist;
    mutable std::vector<int> path;
    mutable std::vector<bool> done;

    // -- Private CONSTRUCTOR
    explicit Digraph(int n);

    void printHelp(int t) const; // rekursiv hjälpfunktion
};
