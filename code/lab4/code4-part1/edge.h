/*************************************************
 * file:	~\code4-part1\edge.h                 *
 * remark: interface for directed weighted edges *
 *************************************************/

#pragma once

#include <iostream>
#include <format>

// Represents a directed edge
class Edge {
public:
    // -- CONSTRUCTORS
    Edge(int u = 0, int v = 0, int w = 0) : from{u}, to{v}, weight{w} {
    }

    bool links_same_nodes(const Edge &e) const {
        return (from == e.from && to == e.to);
    }

    // -- FRIENDS
    friend std::ostream &operator<<(std::ostream &os, const Edge &e) {
        os << std::format("({:2}, {:2}, {:2}) ", e.from, e.to, e.weight);
        return os;
    }

    // -- DATA MEMBERS: represent an edge 'from' 'to' with 'weight'
    int from;
    int to;
    int weight;
};
