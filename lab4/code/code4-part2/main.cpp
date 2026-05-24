/*********************************************
 * file:	~\code4-part2\main.cpp            *
 * remark: test program for lab 4 part B      *
 **********************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>  // std::atoi
#include <vector>
#include <memory>  // std::unique_ptr

#include "graph.h"

constexpr int stop = 9;

// -- FUNCTION DECLARATIONS

int readInt(const std::string& prompt);

int menu();

// Read a graph's data from a file and create the graph
// Return a pointer to the graph
std::unique_ptr<Graph> readGraph(const std::string& fileName);

// -- MAIN PROGRAM ----------------------------------------------------------------------------------------

int main() {
    std::unique_ptr<Graph> G{nullptr};

    int choice{0};
    while (choice != stop) {
        switch (choice = menu()) {
            case 1: {
                std::cout << "File name (complete path)  ? ";
                std::cin >> std::ws;
                std::string fileName{};
                std::getline(std::cin, fileName);
                G = readGraph(fileName);
            } break;
            case 2: {
                std::cout << "\n";
                if (G)
                    G->printGraph();
            } break;
            case 3: {
                std::cout << "\n";
                if (G)
                    G->mstPrim();
            } break;
            case 4: {
                std::cout << "\n";
                if (G)
                    G->mstKruskal();
            } break;
            case stop:
                std::cout << "Bye bye ...\n";
                break;
            default:
                std::cout << "Bad choice!\n";
        }
    }
}

// -- FUNCTION DEFINITIONS

int readInt(const std::string& prompt) {
    std::string number{};

    std::cout << prompt;
    std::cin >> std::ws;
    getline(std::cin, number);

    return std::atoi(number.c_str());
}

int menu() {
    std::cout << "\n== Menu =======\n";
    std::cout << "1. readGraph   \n";
    std::cout << "2. printGraph  \n";
    std::cout << "3. mstPrim     \n";
    std::cout << "4. mstKruskal  \n";
    std::cout << "9. quit        \n";
    std::cout << "===============\n";

    return readInt("Your choice ? ");
}

// Read a graph's data from a file and create the graph
// Return a pointer to the graph
std::unique_ptr<Graph> readGraph(const std::string& fileName) {
    std::ifstream file{fileName};  // complete path

    if (!file) {
        std::cout << "File not found!\n";
        return nullptr;
    }

    int n{0};
    file >> n;  // read number of vertices

    std::vector<Edge> E{};  // to store the edges
    int u{0};
    int v{0};
    int w{0};

    // Read all edges
    while (file >> u >> v >> w) {
        // cout << u << " " << " " << w << "\n";
        E.push_back({u, v, w});
    }
    return std::unique_ptr<Graph>{new Graph{E, n}};
}
