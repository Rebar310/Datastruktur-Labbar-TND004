/*********************************************
 * file:	~\code4-part1\main.cpp            *
 * remark: test program for lab 4 part 1      *
 **********************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>  // std::atoi
#include <vector>
#include <memory>  // std::unique_ptr
#include <cassert>

#include "digraph.h"

constexpr int stop = 9;

// -- FUNCTION DECLARATIONS

int readInt(const std::string& prompt);

int menu();

// Read a graph's data from a file and create the graph
// Return a pointer to the graph
std::unique_ptr<Digraph> readGraph(const std::string& fileName);

// -- MAIN PROGRAM

int main() {
    int choice{0};
    std::unique_ptr<Digraph> G{nullptr};

    while (choice != stop) {
        switch (choice = menu()) {
            case 1: {
                std::cout << "File name (complete path)   ? ";
                std::cin >> std::ws;
                std::string fileName;
                std::getline(std::cin, fileName);
                G = readGraph(fileName);
            } break;
            case 2: {
                std::cout << "\n";
                if (G)
                    G->printGraph();
            } break;
            case 3: {
                int s = readInt("Source s    ? ");
                if (G)
                    G->uwsssp(s);
            } break;
            case 4: {
                std::cout << "\n";
                if (G)
                    G->printTree();
            } break;
            case 5: {
                int t = readInt("Target t    ? ");
                std::cout << "\nShortest path =";
                if (G)
                    G->printPath(t);
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
    std::cout << prompt;

    std::string number;
    std::cin >> std::ws;
    getline(std::cin, number);

    return std::atoi(number.c_str());  // does not crash if the user enters text instead of a number
}

int menu() {
    std::cout << "\n== Menu =======\n";
    std::cout << "1. readGraph   \n";
    std::cout << "2. printGraph  \n";
    std::cout << "3. uwsssp      \n";
    std::cout << "4. printTree   \n";
    std::cout << "5. printPath   \n";
    std::cout << "9. quit        \n";
    std::cout << "===============\n";

    return readInt("Your choice ? ");
}

// Read a graph's data from a file and create the graph
// Return a pointer to the graph
std::unique_ptr<Digraph> readGraph(const std::string& fileName) {
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
        // std::cout << u << " " << v << " " << w << '\n';
        E.push_back({u, v, w});
    }
    return std::unique_ptr<Digraph>{new Digraph{E, n}};
}
