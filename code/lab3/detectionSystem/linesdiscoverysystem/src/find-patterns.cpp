#include <format>
#include <compare>
#include <fstream>
#include <filesystem>

#include <find-patterns.hpp>

#include <algorithm>    //för std::sort
#include <vector>     //för std::vector

const std::filesystem::path data_dir{DATA_DIR};

constexpr int minPoints = 4;

struct Point { // feel free to modify
public:
    Point(int x = 0, int y = 0) : x_{x}, y_{y} {}

    bool operator==(const Point& p) const = default;

    // Compare by y-coordinates and break ties by x-coordinates
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
        return std::strong_ordering::equivalent;
    }

    std::string toString() const { return std::format("({},{})", x_, y_); }

    int x_;
    int y_;
};

/* ***************************************************** */

int main() {
    std::cout << "Enter the name of input points file: ";
    std::string points_file;
    std::cin >> points_file;

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

    // läs in filen med punkter
    std::ifstream pointsFileStream(pointsFile);
    if (!pointsFileStream   ) {
        std::cerr << "Error kan ej öppna filen: " << pointsFile << std::endl;
        return;
    }

    // börjar med att läsa in antalet punkter från filen
    int n;  //antal punkter
    if (!(pointsFileStream >> n)) return;  // om det inte går att läsa in antalet punkter, returnera

    std::vector<Point> points;      // läs in punkterna i en vektor från filen
    points.reserve(n);  // reserverar antalet punkter i vektorn

    // läser in punkterna från filen och lägger till dem i vektorn
    for (int i = 0; i < n; i++) {
        int x, y;
        pointsFileStream >> x >> y;
        points.push_back(Point(x, y)); 
    }

    // sortera punkterna i vektorn, O(n log n) operation, där n är antalet punkter
    std::sort(points.begin(), points.end());

    // skapa utdata filen för segmenten och skriv ut segmenten i filen
    std::ofstream segmentsFileStream(segmentsFile);
    if (!segmentsFileStream) {
        std::cerr << "Error kan ej öppna filen: " << segmentsFile << std::endl;
        return;
    }

    // Huvudalgorithmen, A sorting based algorithm , yttre loop n
    for (size_t i = 0; i < points.size(); i++) {
        Point p = points[i];

        // skapa en lista med alla punkter som ligger efter p i vektorn
        std::vector<Point> others;
       // others.erase(others.begin() + i);  // ta bort alla punkter som ligger före p i vektorn

        // skapar other vektorn O(n) operation, där n är antalet punkter
         for (size_t j = i + 1; j < points.size(); j++) {
             others.push_back(points[j]);
         }

        // sortera other baserad på lutningen mellan p och varje punkt i other, O(n log n)
         // operation, där n är antalet punkter i other
        std::sort(others.begin(), others.end(), [p](const Point& a, const Point& b) {
            // beräkna lutningen mellan p och a
            int dy_a = a.y_ - p.y_;
            int dx_a = a.x_ - p.x_;
            // beräkna lutningen mellan p och b
            int dy_b = b.y_ - p.y_;
            int dx_b = b.x_ - p.x_;

            // Specialhantering för vertikala linjer (dx = 0) vid sortering
            if (dx_a == 0 && dx_b == 0) return false;  // Båda är vertikala
            if (dx_a == 0) return false;               // a är vertikal (störst vinkel)
            if (dx_b == 0) return true;                // b är vertikal

            // jämför lutningarna
            return dy_a * dx_b < dy_b * dx_a;
        });

        // går igenom för att hitta grupper av 3> med samma lutning
        int startindex = 0;
        // iterativ, linjär O(n)
        while (startindex < others.size()) {
            int endindex = startindex + 1;

            while (endindex < others.size()) {
                // beräkna lutningen mellan p och startindex
                int dy_start = others[startindex].y_ - p.y_;
                int dx_start = others[startindex].x_ - p.x_;
                // beräkna lutningen mellan p och endindex
                int dy_end = others[endindex].y_ - p.y_;
                int dx_end = others[endindex].x_ - p.x_;
                // jämför lutningarna
                if (dy_start * dx_end == dy_end * dx_start) {  // om likamed lutning, öka endindex
                    endindex++;
                } else {
                    break;  // annars break
                }
            }
            int groupsize = endindex - startindex;

            // om över 3 punkter med samma lutning, skriv ut segmentet
            if (groupsize >= 3) {

                Point start = p;
                Point end = others[endindex - 1];

                //for (int k = startindex; k < endindex; k++) {
                //    if (others[k] < start) {
                //        start = others[k];
                //    }
                //    if (others[k] > end) {
                //        end = others[k];
                //    }
                //}

                // undviker dublicates
                if (p ==start) {  // skriv ut segmentet endast om p är den minsta punkten i segmentet
                    segmentsFileStream << start.x_ << " " << start.y_ << " " << end.x_ << " "
                                       << end.y_ << "\n";
                }
            }

            startindex = endindex;
        }
    }

}

void analyseData(const std::string& name) {
    std::filesystem::path points_name = name;
    std::filesystem::path segments_name = "segments-" + name;

    analyseData(data_dir / points_name, data_dir / "output" / segments_name);
}
