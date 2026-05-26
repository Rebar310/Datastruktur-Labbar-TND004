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

// Create a graph with n vertices and no edges
Graph::Graph(int n) : 
    table(n + 1),  // Skapar adjacency list med n + 1 platser.
    size{n},   // Sparar antal noder
    n_edges{0} { // Från början finns inga kanter

    assert(n >= 1); // Kontrollera att grafen har minst en nod
}

// Skapar en graf med n noder och kanterna som finns i V
Graph::Graph(const std::vector<Edge> &V, int n) : Graph{n} { // Anropar först konstruktorn ovan för att skapa en tom graf
    
    // Gå igenom alla kanter i vektorn V
    for (auto e : V) {
        insertEdge(e); // Lägg in varje kant i grafen
    }
}

// -- MEMBER FUNCTIONS

/*
 * insert undirected edge e
 * update weight if edge e is present
 */
void Graph::insertEdge(const Edge &e) {
    assert(e.from >= 1 && e.from <= size); // Kontrollera att startnoden finns i grafen
    assert(e.to >= 1 && e.to <= size); // Kontrollera att slutnoden finns i grafen

    // Check if a given edge e1 already exists
    // Lambda-funktion som lägger in en riktad version av kanten.
    // Eftersom grafen är odirigerad kommer vi senare att anropa denna
    // både för e och för e.reverse().
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
// Bygger ett minimum spanning tree genom att börja i en nod
// och hela tiden välja billigaste kanten till en ny nod.
void Graph::mstPrim() const {
    std::vector<int> dist(size + 1, std::numeric_limits<int>::max()); // dist[v] sparar billigaste kända kantvikt för att nå nod v
    std::vector<int> path(size + 1, 0); // path[v] sparar vilken nod vi kom ifrån när nod v lades till
    std::vector<bool> done(size + 1, false);  // done[v] markerar om nod v redan är med i MST

    // *** TODO ***
    // Ett element i priority queue består av:
    // weight = kantens vikt
    // from   = noden kanten kommer från
    // to     = noden kanten går till
    using PQElement = std::tuple<int, int, int>;
    // weight, from, to

    // Priority queue används som min-heap.
    // std::priority_queue är normalt max-heap,
    // men std::greater gör att minsta tuple hamnar först.
    // Eftersom tuple jämförs från vänster till höger sorteras kön först på weight.
    std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>> pq;

    int start = 1; // Vi väljer nod 1 som startnod
    dist[start] = 0; // Kostnaden att starta i startnoden är 0

    // Lägg in startnoden i kön.
    // from = 0 betyder att detta inte är en riktig kant,
    // utan bara startpunkten.
    pq.push({ 0, 0, start });

    int totalWeight = 0; // Summan av vikterna i MST
    int edgesUsed = 0; // Antal kanter som har lagts till i MST


    // Kör så länge det finns kandidater i kön.
    // Ett MST för size noder ska ha size - 1 kanter.
    while (!pq.empty() && edgesUsed < size) {
        auto [weight, from, v] = pq.top(); // Hämta kanten med minst vikt
        pq.pop();// Ta bort den från kön

        // Om noden redan är med i MST hoppar vi över den
        if (done[v]) {
            continue;
        }

        done[v] = true;    // Markera noden som inkluderad i MST

        // Om from != 0 betyder det att detta är en riktig kant.
        // Startnoden hade from = 0 och ska inte skrivas ut.
        if (from != 0) {
            std::cout << std::format("({}, {}, {})\n", from, v, weight); // Skriv ut kanten som valts till MST
            totalWeight += weight; // Lägg till kantens vikt i totalsumman
            edgesUsed++;  // Öka antal valda kanter
        }

        // Lägg till kantens vikt i totalsumman
        // Gå igenom alla kanter som går ut från nod v
        for (auto const& e : table[v]) {
            int u = e.to; // u är grannnoden

            // Om grannnoden inte redan är med i MST
            // och denna kant är billigare än tidigare känd kant till u
            if (!done[u] && e.weight < dist[u]) {
                dist[u] = e.weight; // Uppdatera billigaste kända kant till u
                path[u] = v; // Spara att u bäst nås från v
                pq.push({ e.weight, v, u }); // Lägg in kanten i priority queue som kandidat
            }
        }
    }

    // Skriv ut total vikt för MST
    std::cout << "Total weight = " << totalWeight << "\n";
}





// Kruskal's minimum spanning tree algorithm
// Bygger ett MST genom att sortera alla kanter efter vikt
// och lägga till en kant om den inte skapar en cykel.
void Graph::mstKruskal() const {
    // *** TODO ***

    // Ett element i priority queue består av:
    // weight = kantens vikt
    // from   = ena noden
    // to     = andra noden
    using PQElement = std::tuple<int, int, int>;
    // weight, from, to

    // Priority queue som min-heap.
    // Minsta kantvikt hamnar först.
    std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>> pq;

    // Lägg in varje kant endast en gång.
    // Eftersom grafen är odirigerad finns varje kant lagrad åt båda håll:
    // u -> v och v -> u.
    // Villkoret e.from < e.to gör att vi bara tar med en av dem.
    for (int v = 1; v <= size; ++v) {
        for (auto const& e : table[v]) { // Gå igenom alla kanter från nod v
            if (e.from < e.to) { // Lägg bara in kanten om from är mindre än to
                pq.push({ e.weight, e.from, e.to }); // Lägg in kanten i priority queue
            }
        }
    }

    // Skapa disjoint sets för alla noder.
    // Används för att kontrollera om en kant skapar cykel.
    DSets sets(size);

    int totalWeight = 0; // Summan av vikterna i MST
    int edgesUsed = 0; // Antal kanter som lagts till i MST

    // Ett MST ska innehålla exakt size - 1 kanter
    while (!pq.empty() && edgesUsed < size - 1) {
        auto [weight, u, v] = pq.top(); // Hämta kanten med lägst vikt
        pq.pop(); // Ta bort kanten från kön

        int rootU = sets.find(u); // Hitta vilken komponent u tillhör
        int rootV = sets.find(v); // Hitta vilken komponent v tillhör

        // Om u och v ligger i olika komponenter
        // skapar kanten inte en cykel
        if (rootU != rootV) {
            std::cout << std::format("({}, {}, {})\n", u, v, weight); // Skriv ut kanten som läggs till i MST

            totalWeight += weight; // Lägg till kantens vikt i totalsumman
            edgesUsed++; // Öka antal valda kanter

            sets.join(rootU, rootV); // Slå ihop komponenterna
        }

        // Om rootU == rootV betyder det att u och v redan är ihopkopplade.
        // Då skulle kanten skapa en cykel, så den hoppas över.
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
