#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <limits>
#include <algorithm>

// Struktur som representerar ett flyg
struct Flight {
    int from = -1;      // Index för startflygplats
    int to = -1;        // Index för destinationsflygplats
    int departure = 0; // Avgångstid i minuter efter midnatt
    int arrival = 0;   // Ankomsttid i minuter efter midnatt
    std::string fromCode = ""; // Flygplatskod för start
    std::string toCode = "";   // Flygplatskod för destination
};

// Representerar "oändligt långt bort"
const int INF = std::numeric_limits<int>::max();

// Själva grafen
// graph[i] innehåller alla utgående flyg från flygplats i
std::vector<std::vector<Flight>> graph;

// Kopplar flygplatskod -> index
// Exempel: "ARN" -> 0
std::unordered_map<std::string, int> airportToIndex;

// Kopplar index -> flygplatskod
// Exempel: 0 -> "ARN"
std::vector<std::string> indexToAirport;

// Distansvektor
// dist[i] = tidigaste ankomsttid till flygplats i
std::vector<int> dist;

std::vector<int> previousAirport; // Sparar vilken flygplats vi kom ifrån
std::vector<Flight> previousFlight; // Sparar vilket flyg som användes

int startAirport = -1; // Startflygplatsens index
int startTime = 0; // Starttid
bool plansComputed = false; // Håller koll på om Dijkstra har körts

// Konverterar en tid från format HH:MM till minuter
// Exempel: "08:30" -> 510
int timeToMinutes(const std::string& time) {
    int hours = std::stoi(time.substr(0, 2)); // Hämtar timmarna
    int minutes = std::stoi(time.substr(3, 2)); // Hämtar minuterna

    return hours * 60 + minutes; // Omvandlar till total antal minuter
}

// Konverterar minuter tillbaka till HH:MM
// Exempel: 510 -> "08:30"
std::string minutesToTime(int minutes) {
    int hours = minutes / 60;     // Beräkna timmar
    int mins = minutes % 60;     // Beräkna resterande minuter

    std::string h = (hours < 10 ? "0" : "") + std::to_string(hours); // Lägg till ledande nolla vid behov

    std::string m = (mins < 10 ? "0" : "") + std::to_string(mins); // Lägg till ledande nolla vid behov

    return h + ":" + m; // Returnera komplett tid
}

void loadFlightData() { // Läser in flygdata från fil och bygger grafen

    std::string fileName;

    std::cout << "File name? "; // Fråga användaren efter filnamn

    std::getline(std::cin >> std::ws, fileName); 

    std::ifstream file(fileName); // Öppna filen


    if (!file) {  // Kontrollera om filen kunde öppnas
        std::cout << "Could not open file.\n";
        return;
    }

    graph.clear(); // Töm tidigare data
    airportToIndex.clear();
    indexToAirport.clear();

    int n; // Antal flygplatser

    file >> n;

    graph.resize(n); // Skapa plats i datastrukturerna
    indexToAirport.resize(n);

    // Läs in alla flygplatskoder
    for (int i = 0; i < n; ++i) {

        std::string airport;

        file >> airport;

        airportToIndex[airport] = i; // Koppla kod -> index
        indexToAirport[i] = airport; // Koppla index -> kod
    }

    std::string date; // Läs datumet (används ej mer)
    file >> date;
    std::string line;
    std::getline(file, line);  // Läs bort tom rad efter datumet

    // Läs flyg rad för rad
    while (std::getline(file, line)) {

        // Hoppa över tomma rader
        if (line.empty()) {
            continue;
        }

        std::stringstream ss(line); // Gör det enkelt att läsa från raden

        // Variabler för flygdata
        std::string fromCode;
        std::string toCode;
        std::string departureTime;
        std::string arrivalTime;

        // Läs flygrad
        ss >> fromCode >> toCode >> departureTime >> arrivalTime;

        // Kontrollera att startflygplats finns
        if (airportToIndex.find(fromCode) == airportToIndex.end()) {
            continue;
        }

        // Kontrollera att destinationsflygplats finns
        if (airportToIndex.find(toCode) == airportToIndex.end()) {
            continue;
        }

        int from = airportToIndex[fromCode]; // Hämta index
        int to = airportToIndex[toCode];

        Flight flight; // Skapa flygobjekt

        flight.from = from;
        flight.to = to;
        flight.departure = timeToMinutes(departureTime);
        flight.arrival = timeToMinutes(arrivalTime);
        flight.fromCode = fromCode;
        flight.toCode = toCode;

        graph[from].push_back(flight); // Lägg till flyget i adjacency list
    }

    std::cout << "Flight data loaded.\n";
}

// Skriver ut hela grafen
void displayGraph() {

    // Kontrollera att graf finns
    if (graph.empty()) {
        std::cout << "No graph loaded.\n";
        return;
    }

    // Gå igenom alla flygplatser
    for (int i = 0; i < graph.size(); ++i) {

        std::cout << indexToAirport[i] << ":\n";

        for (const auto& flight : graph[i]) { // Skriv ut alla flyg från flygplatsen

            std::cout
                << "  "
                << flight.fromCode
                << " -> "
                << flight.toCode
                << "  "
                << minutesToTime(flight.departure)
                << " - "
                << minutesToTime(flight.arrival)
                << "\n";
        }
    }
}

// Kör Dijkstra för att hitta tidigaste ankomsttider
void computeTravelPlans() {

    // Kontrollera att graf finns
    if (graph.empty()) {
        std::cout << "No graph loaded.\n";
        return;
    }

    std::string startCode;
    std::string timeString;

    std::cout << "Start airport? "; // Fråga efter startflygplats
    std::cin >> startCode;

    std::cout << "Start time (HH:MM)? "; // Fråga efter starttid
    std::cin >> timeString;

    // Kontrollera att flygplatsen finns
    if (airportToIndex.find(startCode) == airportToIndex.end()) {
        std::cout << "Unknown airport.\n";
        return;
    }

    // Spara startflygplats
    startAirport = airportToIndex[startCode];

    // Konvertera starttid till minuter
    startTime = timeToMinutes(timeString);

    int n = graph.size();

    dist.assign(n, INF); // Initiera alla avstånd till oändlighet

    previousAirport.assign(n, -1); // Ingen tidigare flygplats ännu

    previousFlight.assign( // Tomma flygobjekt
        n,
        Flight{ -1, -1, 0, 0, "", "" }
    );

    // Ett state i priority queue:
   // first = tid
   // second = flygplats
    using State = std::pair<int, int>;

    // Min-heap baserad på tid
    std::priority_queue<
        State,
        std::vector<State>,
        std::greater<State>
    > pq;

    // Startnoden får starttiden
    dist[startAirport] = startTime;

    // Lägg in startflygplatsen i heapen
    pq.push({ startTime, startAirport });

    // Dijkstra
    while (!pq.empty()) {

        State top = pq.top(); // Hämta noden med lägst tid
        pq.pop();
        int currentTime = top.first;
        int currentAirport = top.second;

        if (currentTime != dist[currentAirport]) { // Hoppa över gamla värden
            continue;
        }

        // Gå igenom alla flyg från nuvarande flygplats
        for (const auto& flight : graph[currentAirport]) {

            // Relaxation-villkor
            // Vi får bara ta flyget om:
            // 1. Flyget avgår efter vi anlänt
            // 2. Flyget ger tidigare ankomst
            if (
                flight.departure >= currentTime &&
                flight.arrival < dist[flight.to]
                ) {

                dist[flight.to] = flight.arrival;  // Uppdatera bästa ankomsttid
                previousAirport[flight.to] = currentAirport; // Spara föregående flygplats
                previousFlight[flight.to] = flight; // Spara vilket flyg som användes

                pq.push( // Lägg in i heapen
                    {
                        dist[flight.to],
                        flight.to
                    }
                );
            }
        }
    }

    plansComputed = true;

    std::cout << "Travel plans computed.\n";
}

// Skriver ut resvägen till en destination
void printTravelPlanTo(int destination) {

    // Ingen väg hittades
    if (dist[destination] == INF) {

        std::cout
            << "No reachable travel plan.\n";

        return;
    }

    std::vector<Flight> route; // Här sparas flygen i rutten
    int current = destination;

    while (//Gå baklänges genom path - informationen
        current != startAirport &&
        previousAirport[current] != -1
        ) {

        route.push_back(previousFlight[current]); // Lägg till flyget

        current = previousAirport[current]; // Gå till föregående flygplats
    }

    // Rutten byggdes baklänges
    // Därför vänds den
    std::reverse(route.begin(), route.end());

    // Skriv ut flygen i ordning
    for (const auto& flight : route) {

        std::cout
            << flight.fromCode
            << " -> "
            << flight.toCode
            << "  departure: "
            << minutesToTime(flight.departure)
            << "  arrival: "
            << minutesToTime(flight.arrival)
            << "\n";
    }

    // Skriv ut slutlig ankomsttid
    std::cout
        << "Earliest arrival: "
        << minutesToTime(dist[destination])
        << "\n";
}

// Frågar användaren efter destination
void showTravelPlanToDestination() {

    // Kontrollera att Dijkstra körts
    if (!plansComputed) {

        std::cout
            << "Compute travel plans first.\n";

        return;
    }

    std::string destinationCode;
    std::cout << "Destination airport? "; // Fråga efter destination
    std::cin >> destinationCode;

    if (  // Kontrollera att flygplatsen finns
        airportToIndex.find(destinationCode) ==
        airportToIndex.end()
        ) {

        std::cout << "Unknown airport.\n";

        return;
    }

    // Hämta index
    int destination =
        airportToIndex[destinationCode];

    // Skriv ut rutten
    printTravelPlanTo(destination);
}

// Skriver ut alla nåbara resplaner
void showAllReachablePlans() {

    // Kontrollera att Dijkstra körts
    if (!plansComputed) {

        std::cout
            << "Compute travel plans first.\n";

        return;
    }

    // Gå igenom alla flygplatser
    for (int i = 0; i < graph.size(); ++i) {

        // Hoppa över startflygplatsen
       // och flygplatser som ej nås
        if (
            i != startAirport &&
            dist[i] != INF
            ) {

            std::cout
                << "\nTravel plan to "
                << indexToAirport[i]
                << ":\n";

            printTravelPlanTo(i); // Skriv ut resplan
        }
    }
}

// Skriver ut meny =================================================
int menu() {

    std::cout << "\n== Travel Planner ==\n";

    std::cout << "1. Load flight data\n";

    std::cout << "2. Display graph\n";

    std::cout << "3. Compute travel plans\n";

    std::cout
        << "4. Show travel plan to destination\n";

    std::cout
        << "5. Show all reachable travel plans\n";

    std::cout << "9. Quit\n";

    int choice;

    std::cout << "Your choice? ";

    std::cin >> choice;

    return choice;
}

// Huvudprogram ____________________________________________________________
int main() {

    int choice = 0;

    while (choice != 9) { // Kör tills användaren väljer quit

        choice = menu(); // Visa meny

        switch (choice) {

        case 1:
            loadFlightData();
            break;

        case 2:
            displayGraph();
            break;

        case 3:
            computeTravelPlans();
            break;

        case 4:
            showTravelPlanToDestination();
            break;

        case 5:
            showAllReachablePlans();
            break;

        case 9:
            std::cout << "Bye bye!\n";
            break;

        default:
            std::cout << "Bad choice.\n";
        }
    }

    return 0;
}