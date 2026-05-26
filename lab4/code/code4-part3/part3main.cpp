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
    std::string flightNo = "";
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

// Läser in flygdata från textfil och bygger grafen
void loadFlightData() {
    std::string fileName; // Variabel för filnamnet användaren skriver in

    std::cout << "File name   ? ";
    std::getline(std::cin >> std::ws, fileName); // Läser hela raden inklusive mellanslag

    std::ifstream file(fileName); // Öppnar filen

    if (!file) {
        std::cout << "Could not open file.\n";
        return;
    }

    graph.clear(); // Töm tidigare data i grafen
    airportToIndex.clear(); // Töm map som översätter flygplatskod -> index
    indexToAirport.clear(); // Töm vector som översätter index -> flygplatskod
    plansComputed = false; // Markerar att Dijkstra ännu inte körts

    std::string label; // Första ordet i filen ("airports")
    int n; // Antal flygplatser

    file >> label >> n; // Läser: airports 12

    graph.resize(n); // Skapar adjacency list med plats för n flygplatser
    indexToAirport.resize(n); // Skapar vector som lagrar flygplatskoder
    
    // Läs in alla flygplatskoder
    for (int i = 0; i < n; ++i) {
        std::string airport;
        file >> airport; // Läs flygplatskod

        airportToIndex[airport] = i; // Koppla flygplatskod -> index
        indexToAirport[i] = airport; // Koppla index -> flygplatskod
    }

    std::string line;

    // Läs bort resten av raden efter sista flygplatsen
    std::getline(file, line);

    // Hoppa fram till CSV-headern
    while (std::getline(file, line)) {
        // När vi hittar headern börjar flygdata efter denna
        if (line == "flight_no,origin,destination,dep_time,arr_time") {
            break;
        }
    }

    // Läs alla flygrader
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue; // Hoppa över tomma rader
        }

        std::stringstream ss(line); // Gör raden till en stringstream så vi kan dela upp CSV-data

        // Variabler för flyginformation
        std::string flightNo;
        std::string fromCode;
        std::string toCode;
        std::string depDateTime;
        std::string arrDateTime;

        // Läs CSV-fälten separerade med komma
        std::getline(ss, flightNo, ',');
        std::getline(ss, fromCode, ',');
        std::getline(ss, toCode, ',');
        std::getline(ss, depDateTime, ',');
        std::getline(ss, arrDateTime, ',');

        // Kontrollera att flygplatserna finns
        if (airportToIndex.find(fromCode) == airportToIndex.end() ||
            airportToIndex.find(toCode) == airportToIndex.end()) {
            continue;
        }

        // Plocka ut endast tiden från datum+tidssträngen
        // Exempel:
        // "2026-02-10 06:05"
        // blir:
        // "06:05"
        std::string depTime = depDateTime.substr(11, 5);
        std::string arrTime = arrDateTime.substr(11, 5);

        Flight flight; // Skapa Flight-objekt

        // 
        flight.flightNo = flightNo;  // Spara flygnummer

        flight.fromCode = fromCode;  // Spara flygplatskoder
        flight.toCode = toCode;

        flight.from = airportToIndex[fromCode]; // Konvertera flygplatskod -> index
        flight.to = airportToIndex[toCode];

        flight.departure = timeToMinutes(depTime); // Konvertera tid till minuter
        flight.arrival = timeToMinutes(arrTime);

        // Lägg in flyget i adjacency listan
        // graph[from] innehåller alla flyg som avgår från den flygplatsen
        graph[flight.from].push_back(flight);
    }

    std::cout << "\nFlight data loaded.\n";
}

// Skriver ut hela grafen som adjacency lists
void displayGraph() {

    // Om grafen är tom betyder det att ingen fil har lästs in ännu
    if (graph.empty()) {
        std::cout << "No graph loaded.\n";
        return;
    }

    // order ska innehålla index till flygplatserna
    // Den används för att kunna skriva ut flygplatserna alfabetiskt
    std::vector<int> order;

    // Lägg in alla flygplatsindex i order
    for (int i = 0; i < indexToAirport.size(); ++i) {
        order.push_back(i);
    }

    // Sortera indexen baserat på flygplatskoderna
    // Exempel: AMS kommer före ARN
    std::sort(order.begin(), order.end(), [](int a, int b) {
        return indexToAirport[a] < indexToAirport[b];
        });

    // Skriv rubrik
    std::cout << "\n------------------------------------------------------------------\n";
    std::cout << "Airport  adjacency lists\n";
    std::cout << "------------------------------------------------------------------\n";

    // Gå igenom flygplatserna i alfabetisk ordning
    for (int i : order) {

        // Skriv flygplatskod
        std::cout << indexToAirport[i] << "  :\n";

        // Gå igenom alla flyg som avgår från flygplats i
        for (const auto& flight : graph[i]) {

            // Skriv ut flygnummer, destination, avgångstid och ankomsttid
            std::cout << "\t("
                << flight.flightNo << " "
                << flight.toCode << ", "
                << minutesToTime(flight.departure) << ", "
                << minutesToTime(flight.arrival)
                << ")\n";
        }

        // Tom rad mellan flygplatser
        std::cout << "\n";
    }

    // Avslutande linje
    std::cout << "------------------------------------------------------------------\n";
}


// Kör Dijkstra för att hitta tidigaste ankomsttider från en startflygplats
void computeTravelPlans() {

    // Kontrollera att grafen finns
    if (graph.empty()) {
        std::cout << "No graph loaded.\n";
        return;
    }

    // Startflygplatsens kod, exempelvis ARN
    std::string startCode;

    // Starttid som text, exempelvis 05:00
    std::string timeString;

    // Fråga användaren efter startflygplats
    std::cout << "Start airport? ";
    std::cin >> startCode;

    // Fråga användaren efter starttid
    std::cout << "Start time (HH:MM)? ";
    std::cin >> timeString;

    // Kontrollera att flygplatskoden finns i vår map
    if (airportToIndex.find(startCode) == airportToIndex.end()) {
        std::cout << "Unknown airport.\n";
        return;
    }

    // Spara startflygplatsens index
    startAirport = airportToIndex[startCode];

    // Konvertera starttid till minuter efter midnatt
    startTime = timeToMinutes(timeString);

    // Antal flygplatser
    int n = graph.size();

    // ---------------------------------------------------------- ----------- O(V) + O(E)

    // dist[i] = tidigaste kända ankomsttid till flygplats i
    // Från början är alla oändligt långt bort
    dist.assign(n, INF);

    // previousAirport[i] = flygplatsen vi kom ifrån för att nå i
    // -1 betyder att vi ännu inte har någon föregående flygplats
    previousAirport.assign(n, -1);

    // previousFlight[i] = flyget vi tog för att komma till i
    previousFlight.assign(
        n,
        Flight{ -1, -1, 0, 0, "", "" }
    );

    // --------------------------------------------------------
    // Ett State i kön består av:
    // first = tidigaste ankomsttid
    // second = flygplatsens index
    using State = std::pair<int, int>;

    // Priority queue används som min-heap.
    // Den flygplats med lägst ankomsttid behandlas först.
    std::priority_queue<
        State,
        std::vector<State>,
        std::greater<State>
    > pq;

    // Till startflygplatsen är vi framme vid starttiden
    dist[startAirport] = startTime;

    // Lägg in startflygplatsen i kön
    pq.push({ startTime, startAirport }); // --------------------------// O(1) eftersom kön är tom

    // Dijkstra-algoritmen
    while (!pq.empty()) {

        // Hämta flygplatsen med lägst/tidigast känd ankomsttid
        State top = pq.top();  
        pq.pop(); // ------------------------------ at worst pop E edges from heap, each pop takes O(log E) = E log E

        // Plocka ut tid och flygplatsindex
        int currentTime = top.first;
        int currentAirport = top.second;

        // Om detta är ett gammalt värde hoppar vi över det
        // Det kan hända eftersom samma flygplats kan ha lagts in flera gånger i kön
        if (currentTime != dist[currentAirport]) {
            continue;
        }

        // ----------------------------------------------------------------------- Total för for loopen O(E log E)
        // Gå igenom alla flyg som avgår från nuvarande flygplats
        for (const auto& flight : graph[currentAirport]) { // --------------------- for loop Runs O(E) ,flight = Edge

            // Relaxation-villkor:
            // 1. Flyget måste avgå efter eller exakt när vi har anlänt
            // 2. Flyget måste ge en tidigare ankomst än tidigare kända bästa tid
            if (
                flight.departure >= currentTime &&
                flight.arrival < dist[flight.to]
                ) {

                // Uppdatera tidigaste ankomsttid till destinationen
                dist[flight.to] = flight.arrival;

                // Spara vilken flygplats vi kom ifrån
                previousAirport[flight.to] = currentAirport;

                // Spara vilket flyg vi tog
                previousFlight[flight.to] = flight;

                // Lägg in destinationen i kön med sin nya bästa ankomsttid
                pq.push( //--------------------------------------------------------- Pushar edge  alltså O(log E)
                    {
                        dist[flight.to],
                        flight.to
                    }
                );
            }
        }
    }

    // Markera att resplaner har beräknats
    plansComputed = true;

    std::cout << "Travel plans computed.\n";
}

// Skriver ut resvägen till en specifik destination
void printTravelPlanTo(int destination) {

    // Om destinationen fortfarande har INF finns ingen möjlig resväg
    if (dist[destination] == INF) {
        std::cout << "No reachable travel plan.\n";
        return;
    }

    // Skriv rubrik
    std::cout << "\n** Travel plan from "
        << indexToAirport[startAirport]
        << " to "
        << indexToAirport[destination]
        << " **\n";

    // Om destinationen är startflygplatsen behövs inga flyg
    if (destination == startAirport) {
        std::cout << minutesToTime(startTime)
            << " "
            << indexToAirport[startAirport]
            << "\n";
        return;
    }

    // route lagrar alla flyg i resvägen
    std::vector<Flight> route;

    // Börja bakifrån vid destinationen
    int current = destination;

    // Följ previousAirport bakåt tills vi når startflygplatsen
    while (current != startAirport && previousAirport[current] != -1) {

        // Lägg till flyget som användes för att nå current
        route.push_back(previousFlight[current]);

        // Gå bakåt till föregående flygplats
        current = previousAirport[current];
    }

    // Rutten byggdes baklänges, så vi vänder den
    std::reverse(route.begin(), route.end());

    // Skriv starttid och startflygplats
    std::cout << minutesToTime(startTime)
        << " "
        << indexToAirport[startAirport];

    // Skriv alla flyg i rutten i rätt ordning
    for (const auto& flight : route) {
        std::cout << " "
            << minutesToTime(flight.departure)
            << " "
            << flight.flightNo
            << " --> "
            << minutesToTime(flight.arrival)
            << " "
            << flight.toCode;
    }

    std::cout << "\n";
}


// Frågar användaren efter destination och skriver ut resvägen dit
void showTravelPlanToDestination() {

    // Kontrollera att Dijkstra har körts
    if (!plansComputed) {
        std::cout << "Compute travel plans first.\n";
        return;
    }

    // Destinationens flygplatskod
    std::string destinationCode;

    // Fråga användaren
    std::cout << "Destination airport? ";
    std::cin >> destinationCode;

    // Kontrollera att destinationen finns
    if (
        airportToIndex.find(destinationCode) ==
        airportToIndex.end()
        ) {
        std::cout << "Unknown airport.\n";
        return;
    }

    // Hämta destinationens index
    int destination = airportToIndex[destinationCode];

    // Skriv ut resvägen
    printTravelPlanTo(destination);
}

// Skriver ut resplaner till alla nåbara destinationer
void showAllReachablePlans() {

    // Kontrollera att Dijkstra har körts
    if (!plansComputed) {
        std::cout << "Compute travel plans first.\n";
        return;
    }

    // order lagrar index till alla nåbara destinationer
    std::vector<int> order;

    // Gå igenom alla flygplatser
    for (int i = 0; i < graph.size(); ++i) {

        // Ta bara med flygplatser som:
        // 1. inte är startflygplatsen
        // 2. faktiskt går att nå
        if (i != startAirport && dist[i] != INF) {
            order.push_back(i);
        }
    }

    // Sortera destinationerna alfabetiskt
    std::sort(order.begin(), order.end(), [](int a, int b) {
        return indexToAirport[a] < indexToAirport[b];
        });

    // Skriv ut resplan till varje nåbar destination
    for (int i : order) {
        printTravelPlanTo(i);
    }
}

// Skriver ut meny =================================================
int menu() {
    std::cout << "\n=========== Menu ============\n";
    std::cout << "1. Read Graph\n";
    std::cout << "2. Print Graph\n";
    std::cout << "3. Dijkstra\n";
    std::cout << "4. Travel Plan\n";
    std::cout << "5. Print Earliest Arrival Paths\n";
    std::cout << "9. Quit\n";
    std::cout << "==============================\n";

    int choice;
    std::cout << "Your choice ? ";
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