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
    // Öppnar inputfilen med punkter
    std::ifstream in(pointsFile);

    // Skapar/öppnar outputfilen där linjesegmenten ska skrivas
    std::ofstream out(segmentsFile);

    // Läser in antalet punkter från filen
    int n;
    in >> n;

    // Här sparas alla punkter från inputfilen
    std::vector<Point> points;

    // Läser in alla punkter från filen
    for (int i = 0; i < n; i++) {
        int x, y;
        in >> x >> y;

        // Skapar en Point och lägger in den i vektorn
        points.emplace_back(x, y);
    }

    // Går igenom varje punkt p.
    // p används som "utgångspunkt" för att hitta andra punkter
    // som ligger på samma linje som p.
    for (const Point& p : points) {

        // Här sparas lutningen mellan p och varje annan punkt.
        // Varje element består av:
        // first  = lutningen
        // second = punkten q
        std::vector<std::pair<double, Point>> slopes;

         // Går igenom alla andra punkter q
        for (const Point& q : points) {

            // Hoppa över om q är samma punkt som p
            if (p == q) {
                continue;
            }

            double slope;

            // Om x-koordinaterna är samma är linjen vertikal.
            // Då går det inte att räkna vanlig lutning eftersom
            // man hade dividerat med 0.
            if (q.x_ == p.x_) {
                slope = std::numeric_limits<double>::infinity();
            } else {
                // Beräknar lutningen mellan p och q:
                // slope = skillnad i y / skillnad i x
                slope = static_cast<double>(q.y_ - p.y_) / (q.x_ - p.x_);
            }

            // Sparar lutningen tillsammans med punkten q
            slopes.push_back({slope, q});
        }

        // Sorterar punkterna efter lutning relativt p.
        // Punkter med samma lutning hamnar bredvid varandra.
        std::sort(slopes.begin(), slopes.end(),
                  [](const auto& a, const auto& b) { return a.first < b.first; });

        // startIndex markerar början på en grupp med samma lutning
        int startIndex = 0;

        // Går igenom den sorterade slopes-vektorn
        while (startIndex < slopes.size()) {

            // endIndex används för att hitta slutet på gruppen
            // med samma lutning
            int endIndex = startIndex + 1;

            // Fortsätt så länge nästa punkt har samma lutning
            // som punkten vid startIndex
            while (endIndex < slopes.size() && slopes[endIndex].first == slopes[startIndex].first) {
                endIndex++;
            }

            // Antalet punkter i gruppen som har samma lutning till p
            int groupSize = endIndex - startIndex;

            // Om minst 3 andra punkter har samma lutning till p,
            // betyder det att dessa 3 punkter + p = minst 4 punkter
            // på samma linje.
            if (groupSize >= 3) {

                // Samlar alla punkter som ligger på samma linje
                std::vector<Point> linePoints;

                // Lägg först till utgångspunkten p
                linePoints.push_back(p);

                // Lägg till alla punkter i gruppen med samma lutning
                for (int i = startIndex; i < endIndex; i++) {
                    linePoints.push_back(slopes[i].second);
                }

                // Sorterar punkterna på linjen efter y och sedan x.
                // Då hamnar ena ändpunkten först och andra sist.
                std::sort(linePoints.begin(), linePoints.end());

                // Första punkten blir startpunkt för linjesegmentet
                Point start = linePoints.front();

                // Sista punkten blir slutpunkt för linjesegmentet
                Point end = linePoints.back();

                // Skriver bara ut linjen om p är den minsta punkten.
                // Detta gör att samma linje inte skrivs flera gånger,
                // eftersom samma linje annars hittas från flera olika p.
                if (p == start) {
                    out << start.x_ << " " << start.y_ << " " << end.x_ << " " << end.y_ << "\n";
                }
            }

            // Flytta startIndex till nästa grupp med ny lutning
            startIndex = endIndex;
        }
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
