//main Lab 4, Part 3

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <sstream>      // för stringstream
#include <algorithm>    // för sort

// =========================================================================
// 1. Struktur och Globala variabler
// =========================================================================
constexpr int stop = 9;

struct Flight {
	std::string flightNum;      //flyg nummer
	std::string fromNode;       //origin, start flygplats
	std::string toNode;         //till flygplats, destination
	int depTime;    //departure time, tiden från 00:00 till avgången i minuter
	int arrivalTime;    //ankomsttid, tiden från x till ankomsten i minuter
};

//=========================================================================
//2. Graph class 
//=========================================================================

class Graph{
public:

	void readGraph(const std::string& fileName);        //option 1: läsa in grafen från en fil, där varje rad representerar en flygning med formatet: flightNumber fromNode toNode depTime arrivalTime
	void printGraph() const;                            //option 2: visa grafen i form av en adjacency list
	void computeTravelPlans() const;     //Option 3: Dijkstra's algorithm för att beräkna den snabbaste resvägen från en startflygplats till en destination
	void showPathTo() const;   //Option 4: Planera en resa som involverar flera flygningar, där användaren kan specificera en startflygplats, en destination och en önskad avresetid. Programmet bör sedan föreslå den bästa resvägen baserat på tillgängliga flygningar och deras avgångs- och ankomsttider.
	void showAllPaths() const;   //Option 5: Visa alla resvägar från en startflygplats till en destination, sorterade efter ankomsttid.

private:
	int numAirports=0; // Antal flygplatser

	std::map<std::string, int> airporttoIndex; // Mappa flygplatsnamn till index, flyplatsnamn -> index
	std::vector<std::vector<Flight>> graph; // Adjacency list representation av en graf
	std::vector<std::string> indexToAirport; // En lista över flygplatser, index -> flygplatsnamn

	//för Dijsktras algoritm
	int currentStartNode = -1; // Aktuell startflygplats index
	int currentStartTime = -1; // Aktuell starttid i minuter

	std::vector<int> dist; // Vektor för att lagra kortaste avstånd från startnoden
	std::vector<int> prevAirport; // Vektor för att lagra föregående flygplats i den kortaste vägen
	std::vector<Flight> prevFlight; // Vektor för att lagra föregående flygning i den kortaste vägen
    std::vector<bool> visited; // Vektor för att markera besökta flygplatser

    int timeToMinutes(const std::string& time) const;
	std::string minutesToTime(int minutes) const;
};

// Global instans av din Graph-klass
Graph G;

//=========================================================================
//3. Graph class implementation, medlemsfunktioner
//========================================================================= 

//läsa från graph

void Graph::readGraph(const std::string& fileName) {       //använder  adjacency List
	std::ifstream file(fileName);

    if (!file) {
        std::cout << "File not found!\n";
        return;
    }
    graph.clear();
    airporttoIndex.clear();
    indexToAirport.clear();
    visited.clear();


	//läser in antalet flygplatser
    std::string word;
	while (file >> word) {
		if (word == "airports") {
			file >> numAirports; // Läs antal flygplatser
			break;
		}
	}
	// Läs flygplatsnamn och mappa till index
    graph.resize(numAirports); // Reservera plats för flygplatser
	indexToAirport.resize(numAirports);

	for (int i = 0; i < numAirports; ++i) {
		file >> indexToAirport[i];
        airporttoIndex[indexToAirport[i]] = i;
	}

	// Läs flygningar och bygg grafen
    std::string line;
	std::getline(file, line); // Läs den tomma raden efter flygplatsnamnen)

	//hoppa över "Flights : 2026 - 02 - 10 00 : 00:00 till headern "flight_no, origin, destination, dep_time, arr_time"
    while(std::getline(file, line)) {
        if (line == "flight_no,origin,destination,dep_time,arr_time") {
            break;
        }
    }

    //läs igenom alla flygningar rad för rad tills filen är slut, hoppa över toma rader
    while(std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        std::stringstream ss(line);
        std::string flight_no, origin, destination, dep_time, arr_time;
        std::getline(ss, flight_no, ',');
        std::getline(ss, origin, ',');
        std::getline(ss, destination, ',');
        std::getline(ss, dep_time, ',');
        std::getline(ss, arr_time, ',');

        if(airporttoIndex.find(origin) != airporttoIndex.end() && airporttoIndex.find(destination) != airporttoIndex.end()) {
            // Skapa en ny flight och lägg till den i grafen


            std::string dep_Time = dep_time.substr(11, 5);
            std::string arr_Time = arr_time.substr(11, 5);

            Flight flight;
            flight.flightNum = flight_no;
            flight.fromNode = origin;
            flight.toNode = destination;
            flight.depTime = timeToMinutes(dep_Time);
            flight.arrivalTime = timeToMinutes(arr_Time);

            graph[airporttoIndex[origin]].push_back(flight);
        }
    }

}

int Graph::timeToMinutes(const std::string& time) const {
    int hours = std::stoi(time.substr(0, 2));       //hämat antalet timmar
	int minutes = std::stoi(time.substr(3, 2));     //hämtar antalet minuter
    return hours * 60 + minutes;
}

std::string Graph::minutesToTime(int minutes) const {
    if (minutes == INT_MAX) return "--:--";         // säkerhetsspärr om en flygplats inte kan nås

    int hours = minutes / 60;       //beräknar antalet timmar
	int min = minutes % 60;         //beräknar antalet minuter
 
    // Bygg tidssträngen manuellt med villkor (if-satser direkt i strängen)
    std::string hoursStr = (hours < 10 ? "0" : "") + std::to_string(hours);
    std::string minStr = (min < 10 ? "0" : "") + std::to_string(min);


    return hoursStr + ":" + minStr;
}

void Graph::printGraph() const {

    //om ingen har läst in
    if (numAirports == 0) return;

	//läs in grafen och skriv ut den i form av en adjacency list

    std::cout << std::format("{:-<66}\n", '-');
    std::cout << "Airport  adjacency lists\n";
    std::cout << std::format("{:-<66}\n", '-');

    //sortera de i rätt ordning
	std::vector <int> order;
	for (int i = 0; i < indexToAirport.size(); ++i) {
        order.push_back(i);
	}

	std::sort(order.begin(), order.end(), [&](int a, int b) {
		return indexToAirport[a] < indexToAirport[b];
	});

	for (int i : order) {
		std::cout << indexToAirport[i]<<" :\n";    //skriver ut flygplatsnamnet
		for (const auto& flight : graph[i]) {
			std::cout << "\t(" << flight.flightNum << " " << flight.toNode << ", " << minutesToTime(flight.depTime) << ", " << minutesToTime(flight.arrivalTime) << ")\n";
		}
		std::cout << "\n";
	}

    std::cout << std::format("{:-<66}\n", '-');
}

// =========================================================================
//3. Meny och main
// =========================================================================
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

int main() {

    int choice{ 0 };
    while (choice != stop) {
        switch (choice = menu()) {
        case 1: {   //Read a graph's data from a file and create the graph
            std::cout << "File name  ? ";
            std::cin >> std::ws;
            std::string fileName{};
            std::getline(std::cin, fileName);
            G.readGraph(fileName);
        } break;
        case 2: {
            std::cout << "\n";
            G.printGraph();
        } break;
        case 3: {       //Dijkstra's algorithm
            std::cout << "\n";
            G.computeTravelPlans();
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
