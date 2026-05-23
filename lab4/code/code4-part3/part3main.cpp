#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <limits>
#include <algorithm>

struct Flight {
    int from;
    int to;
    int departure;
    int arrival;
    std::string fromCode;
    std::string toCode;
};

const int INF = std::numeric_limits<int>::max();

std::vector<std::vector<Flight>> graph;
std::unordered_map<std::string, int> airportToIndex;
std::vector<std::string> indexToAirport;

std::vector<int> dist;
std::vector<int> previousAirport;
std::vector<Flight> previousFlight;

int startAirport = -1;
int startTime = 0;
bool plansComputed = false;

int timeToMinutes(const std::string& time) {
    int hours = std::stoi(time.substr(0, 2));
    int minutes = std::stoi(time.substr(3, 2));

    return hours * 60 + minutes;
}

std::string minutesToTime(int minutes) {
    int hours = minutes / 60;
    int mins = minutes % 60;

    std::string h =
        (hours < 10 ? "0" : "") + std::to_string(hours);

    std::string m =
        (mins < 10 ? "0" : "") + std::to_string(mins);

    return h + ":" + m;
}

void loadFlightData() {

    std::string fileName;

    std::cout << "File name? ";

    std::getline(std::cin >> std::ws, fileName);

    std::ifstream file(fileName);

    if (!file) {
        std::cout << "Could not open file.\n";
        return;
    }

    graph.clear();
    airportToIndex.clear();
    indexToAirport.clear();

    int n;

    file >> n;

    graph.resize(n);
    indexToAirport.resize(n);

    for (int i = 0; i < n; ++i) {

        std::string airport;

        file >> airport;

        airportToIndex[airport] = i;
        indexToAirport[i] = airport;
    }

    std::string date;

    file >> date;

    std::string line;

    std::getline(file, line);

    while (std::getline(file, line)) {

        if (line.empty()) {
            continue;
        }

        std::stringstream ss(line);

        std::string fromCode;
        std::string toCode;
        std::string departureTime;
        std::string arrivalTime;

        ss >> fromCode >> toCode >> departureTime >> arrivalTime;

        if (airportToIndex.find(fromCode) == airportToIndex.end()) {
            continue;
        }

        if (airportToIndex.find(toCode) == airportToIndex.end()) {
            continue;
        }

        int from = airportToIndex[fromCode];
        int to = airportToIndex[toCode];

        Flight flight;

        flight.from = from;
        flight.to = to;
        flight.departure = timeToMinutes(departureTime);
        flight.arrival = timeToMinutes(arrivalTime);
        flight.fromCode = fromCode;
        flight.toCode = toCode;

        graph[from].push_back(flight);
    }

    std::cout << "Flight data loaded.\n";
}

void displayGraph() {

    if (graph.empty()) {
        std::cout << "No graph loaded.\n";
        return;
    }

    for (int i = 0; i < graph.size(); ++i) {

        std::cout << indexToAirport[i] << ":\n";

        for (const auto& flight : graph[i]) {

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

void computeTravelPlans() {

    if (graph.empty()) {
        std::cout << "No graph loaded.\n";
        return;
    }

    std::string startCode;
    std::string timeString;

    std::cout << "Start airport? ";
    std::cin >> startCode;

    std::cout << "Start time (HH:MM)? ";
    std::cin >> timeString;

    if (airportToIndex.find(startCode) == airportToIndex.end()) {
        std::cout << "Unknown airport.\n";
        return;
    }

    startAirport = airportToIndex[startCode];

    startTime = timeToMinutes(timeString);

    int n = graph.size();

    dist.assign(n, INF);

    previousAirport.assign(n, -1);

    previousFlight.assign(
        n,
        Flight{ -1, -1, 0, 0, "", "" }
    );

    using State = std::pair<int, int>;
    // arrival time, airport

    std::priority_queue<
        State,
        std::vector<State>,
        std::greater<State>
    > pq;

    dist[startAirport] = startTime;

    pq.push({ startTime, startAirport });

    while (!pq.empty()) {

        State top = pq.top();

        pq.pop();

        int currentTime = top.first;
        int currentAirport = top.second;

        if (currentTime != dist[currentAirport]) {
            continue;
        }

        for (const auto& flight : graph[currentAirport]) {

            if (
                flight.departure >= currentTime &&
                flight.arrival < dist[flight.to]
                ) {

                dist[flight.to] = flight.arrival;

                previousAirport[flight.to] = currentAirport;

                previousFlight[flight.to] = flight;

                pq.push(
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

void printTravelPlanTo(int destination) {

    if (dist[destination] == INF) {

        std::cout
            << "No reachable travel plan.\n";

        return;
    }

    std::vector<Flight> route;

    int current = destination;

    while (
        current != startAirport &&
        previousAirport[current] != -1
        ) {

        route.push_back(previousFlight[current]);

        current = previousAirport[current];
    }

    std::reverse(route.begin(), route.end());

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

    std::cout
        << "Earliest arrival: "
        << minutesToTime(dist[destination])
        << "\n";
}

void showTravelPlanToDestination() {

    if (!plansComputed) {

        std::cout
            << "Compute travel plans first.\n";

        return;
    }

    std::string destinationCode;

    std::cout << "Destination airport? ";

    std::cin >> destinationCode;

    if (
        airportToIndex.find(destinationCode) ==
        airportToIndex.end()
        ) {

        std::cout << "Unknown airport.\n";

        return;
    }

    int destination =
        airportToIndex[destinationCode];

    printTravelPlanTo(destination);
}

void showAllReachablePlans() {

    if (!plansComputed) {

        std::cout
            << "Compute travel plans first.\n";

        return;
    }

    for (int i = 0; i < graph.size(); ++i) {

        if (
            i != startAirport &&
            dist[i] != INF
            ) {

            std::cout
                << "\nTravel plan to "
                << indexToAirport[i]
                << ":\n";

            printTravelPlanTo(i);
        }
    }
}

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

int main() {

    int choice = 0;

    while (choice != 9) {

        choice = menu();

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