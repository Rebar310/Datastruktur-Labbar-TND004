#include <format>
#include <compare>
#include <fstream>
#include <filesystem>

#include <find-patterns.hpp>

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

    // sortera punkterna i vektorn
    std::sort(points.begin(), points.end());

    // skapa utdata filen för segmenten och skriv ut segmenten i filen

                        


}

void analyseData(const std::string& name) {
    std::filesystem::path points_name = name;
    std::filesystem::path segments_name = "segments-" + name;

    analyseData(data_dir / points_name, data_dir / "output" / segments_name);
}
