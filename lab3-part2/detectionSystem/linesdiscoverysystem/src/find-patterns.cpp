#include <format>
#include <compare>
#include <fstream>
#include <filesystem>
#include <vector> // jag la till
#include <algorithm> // jag la till
#include <limits> // jag la till


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

    std::ifstream in(pointsFile);
    std::ofstream out(segmentsFile);

    int n;
    in >> n;

    std::vector<Point> points;

    for (int i = 0; i < n; i++) {
        int x, y;
        in >> x >> y;
        points.emplace_back(x, y);
    }

    for (const Point& p : points) {
        std::vector<std::pair<double, Point>> slopes;

        for (const Point& q : points) {
            if (p == q) {
                continue;
            }

            double slope;

            if (q.x_ == p.x_) {
                slope = std::numeric_limits<double>::infinity();
            } else {
                slope = static_cast<double>(q.y_ - p.y_) / (q.x_ - p.x_);
            }

            slopes.push_back({slope, q});
        }

        std::sort(slopes.begin(), slopes.end(),
                  [](const auto& a, const auto& b) { return a.first < b.first; });

        int startIndex = 0;

        while (startIndex < slopes.size()) {
            int endIndex = startIndex + 1;

            while (endIndex < slopes.size() && slopes[endIndex].first == slopes[startIndex].first) {
                endIndex++;
            }

            int groupSize = endIndex - startIndex;

            if (groupSize >= 3) {
                std::vector<Point> linePoints;
                linePoints.push_back(p);

                for (int i = startIndex; i < endIndex; i++) {
                    linePoints.push_back(slopes[i].second);
                }

                std::sort(linePoints.begin(), linePoints.end());

                Point start = linePoints.front();
                Point end = linePoints.back();

                out << start.x_ << " " << start.y_ << " " << end.x_ << " " << end.y_ << "\n";
            }

            startIndex = endIndex;
        }
    }
}

void analyseData(const std::string& name) {
    std::filesystem::path points_name = name;
    std::filesystem::path segments_name = "segments-" + name;

    analyseData(data_dir / points_name, data_dir / "output" / segments_name);
}
