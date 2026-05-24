//main Lab 4, Part 3

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>  // std::atoi
#include <vector>

#include <unordered_map>


constexpr int stop = 9;

// Struktur som representerar ett flyg
struct Flight {
    int from = -1;      // Index för startflygplats
    int to = -1;        // Index för destinationsflygplats
    int departure = 0; // Avgångstid i minuter efter midnatt
    int arrival = 0;   // Ankomsttid i minuter efter midnatt
    std::string flightNo = "";
    std::string fromAirport = ""; // Flygplatskod för start
    std::string toAirport = "";   // Flygplatskod för destination
};

// Själva grafen
// graph[i] innehåller alla utgående flyg från flygplats i
std::vector < std::vector < Flight>> graph;
std::vector<std::string> indextoairports;
std::unordered_map<std::string, int> airporttoIndex; // Mappar flygplatskoder till index i grafen

// -- FUNCTION DECLARATIONS
void readGraph(const std::string& fileName);
int menu();

// -- MAIN PROGRAM

int main() {

    int choice{ 0 };
    while (choice != stop) {
        switch (choice = menu()) {
		case 1: {   //Read a graph's data from a file and create the graph
            std::cout << "File name  ? ";
            std::cin >> std::ws;
            std::string fileName{};
            std::getline(std::cin, fileName);
            readGraph(fileName);
        } break;
        case 2: {
           /* std::cout << "\n";
            if (G)
                G->printGraph();*/
        } break;
        case 3: {
            /*std::cout << "\n";
            if (G)
                G->mstPrim();*/
        } break;
        case 4: {
           /* std::cout << "\n";
            if (G)
                G->mstKruskal();*/
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
    std::cout << "1. Read Graph   \n";
    std::cout << "2. Print Graph  \n";
    std::cout << "3. Dijkstra     \n";
    std::cout << "4. Travel Plan  \n";
    std::cout << "5. Print Earliest Arrival Paths\n";
    std::cout << "9. quit        \n";
    std::cout << "===============\n";

    return readInt("Your choice ? ");
}

//läsa från graph
void readGraph(const std::string& fileName) {
	std::ifstream file(fileName);

    if (!file) {
        std::cout << "File not found!\n";
        return;
    }

    int Airports= 0;
    std::string word;
	file >>word>> Airports;  // läs antal numer av flygplatser, Airports 12

	graph.clear(); // Rensa grafen innan du läser in nya data
    graph.resize(Airports); // ändra storlek på grafen
	indextoairports.resize(Airports); // ändra storlek på flygplatslistan)

	airporttoIndex.clear(); // Rensa mappen innan du läser in nya data

    //läs in flygplatserna
	for (int i = 0; i < Airports; ++i) {
		std::string airportCode;
		file >> airportCode; // läs in flygplatskod
		indextoairports[i] = airportCode; // spara flygplatskod i listan
		airporttoIndex[airportCode] = i; // mappa flygplatskod till index
	}

    // hoppa till flights header
    std::string line;
    std::getline(file, line);

	while (std::getline(file, line)) {
        
	}

}