#include <format>
#include <compare>
#include <fstream>
#include <filesystem>
#include <vector> // jag la till, behövs för std::vector
#include <algorithm> // jag la till, behövs för std::sort
#include <limits> // jag la till, behövs för infinity()


#include <find-patterns.hpp>

// Sökvägen till data-mappen
const std::filesystem::path data_dir{DATA_DIR};

// Minsta antal punkter som måste ligga på samma linje
constexpr int minPoints = 4;

struct Point { // feel free to modify
public:
    // Konstruktor: skapar en punkt med x- och y-koordinater
    Point(int x = 0, int y = 0) : x_{x}, y_{y} {}

    // Gör att vi kan jämföra två punkter med ==
    bool operator==(const Point& p) const = default;

    // Gör att punkter kan sorteras.
    // Först jämförs y-koordinaten, och om y är samma jämförs x-koordinaten.
    std::strong_ordering operator<=>(Point p) const {
        if (y_ < p.y_) {
            return std::strong_ordering::less;
        }
        if (y_ > p.y_) {
            return std::strong_ordering::greater;
        }
        if (x_ < p.x_) {
            return std::strong_ordering::less;
        }
        if (x_ > p.x_) {
            return std::strong_ordering::greater;
        }

        // Punkterna har samma x och y
        return std::strong_ordering::equivalent;
    }

    // Returnerar punkten som en sträng, t.ex. "(3,5)"
    std::string toString() const { return std::format("({},{})", x_, y_); }

    int x_;
    int y_;
};

/* ***************************************************** */

int main() {

    // Frågar användaren vilken inputfil som ska användas
    std::cout << "Enter the name of input points file: ";
    std::string points_file;
    std::cin >> points_file;
   
    // Startar analysen av punkterna
    analyseData(points_file);
}

/* ***************************************************** */

void analyseData(const std::filesystem::path& pointsFile,
                 const std::filesystem::path& segmentsFile) {
    /*
     * Add code here
     * Feel free to modify the function signature
     * Break your code into small functions
     */
    
     // Öppnar filer
    std::ifstream in(pointsFile);
    std::ofstream out(segmentsFile);

    // Läser in alla punkter
    auto points = readPoints(in);

    // Går igenom varje punkt p
    for (const Point& p : points) {

        // Steg 1: beräkna lutningar från p
        auto slopes = computeSlopes(p, points);

        // Steg 2: sortera lutningar
        sortSlopes(slopes);

        // Steg 3: hitta linjesegment
        findSegmentsFromPoint(p, slopes, out);
    }


}

void analyseData(const std::string& name) {
    // Namnet på inputfilen
    std::filesystem::path points_name = name;

    // Namnet på outputfilen, t.ex. segments-points200.txt
    std::filesystem::path segments_name = "segments-" + name;

    // Kör analysen med fullständig sökväg till input och output
    analyseData(data_dir / points_name, data_dir / "output" / segments_name);
}

// Delar upp analyzeData till flera småfunktioner

std::vector<Point> readPoints(std::ifstream& in) {
    // Läser in antal punkter från filen
    int n;
    in >> n;

    std::vector<Point> points;

    // Läser in varje punkt (x, y) och sparar i vektorn
    for (int i = 0; i < n; i++) {
        int x, y;
        in >> x >> y;

        // Skapar en Point och lägger till i listan
        points.emplace_back(x, y);
    }

    // Returnerar alla punkter
    return points;
}

std::vector<std::pair<double, Point>> computeSlopes(const Point& p,
                                                    const std::vector<Point>& points) {

    // Här lagras (lutning, punkt)
    std::vector<std::pair<double, Point>> slopes;

    // Går igenom alla punkter q
    for (const Point& q : points) {

        // Hoppa över om det är samma punkt
        if (p == q) continue;

        double slope;

        // Om x är samma → vertikal linje → oändlig lutning
        if (q.x_ == p.x_) {
            slope = std::numeric_limits<double>::infinity();
        } else {
            // Vanlig lutning: (y2 - y1) / (x2 - x1)
            slope = static_cast<double>(q.y_ - p.y_) / (q.x_ - p.x_);
        }

        // Spara lutningen tillsammans med punkten
        slopes.push_back({slope, q});
    }

    return slopes;
}

void sortSlopes(std::vector<std::pair<double, Point>>& slopes) {

    // Sorterar alla punkter efter lutning relativt p
    // Punkter med samma lutning hamnar bredvid varandra
    std::sort(slopes.begin(), slopes.end(),
              [](const auto& a, const auto& b) { return a.first < b.first; });
}

void findSegmentsFromPoint(const Point& p, const std::vector<std::pair<double, Point>>& slopes,
                           std::ofstream& out) {

    // startIndex markerar början på en grupp med samma lutning
    int startIndex = 0;

    // Går igenom alla lutningar
    while (startIndex < slopes.size()) {

        // Hitta slutet på gruppen med samma lutning
        int endIndex = startIndex + 1;

        while (endIndex < slopes.size() && slopes[endIndex].first == slopes[startIndex].first) {
            endIndex++;
        }

        // Antal punkter i gruppen (exklusive p)
        int groupSize = endIndex - startIndex;

        // Om minst 3 punkter har samma lutning → tillsammans med p blir det ≥ 4
        if (groupSize >= 3) {

            std::vector<Point> linePoints;

            // Lägg till p först
            linePoints.push_back(p);

            // Lägg till alla punkter i gruppen
            for (int i = startIndex; i < endIndex; i++) {
                linePoints.push_back(slopes[i].second);
            }

            // Sortera punkterna för att hitta start och slut
            std::sort(linePoints.begin(), linePoints.end());

            Point start = linePoints.front();
            Point end = linePoints.back();

            // Endast skriv ut linjen om p är minsta punkten
            // → förhindrar dubbletter
            if (p == start) {
                out << start.x_ << " " << start.y_ << " " << end.x_ << " " << end.y_ << "\n";
            }
        }

        // Gå vidare till nästa grupp
        startIndex = endIndex;
    }
}