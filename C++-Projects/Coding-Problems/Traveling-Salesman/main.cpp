#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <map>

class TSPSolver {
private:
    int N;
    std::vector<std::vector<double>> dist;
    std::vector<std::pair<double, double>> coordinates;
    std::map<std::pair<int, int>, double> memo;

    double euclideanDistance(int i, int j) {
        double dx = coordinates[i].first - coordinates[j].first;
        double dy = coordinates[i].second - coordinates[j].second;
        return sqrt(dx * dx + dy * dy);
    }

    double tsp(int pos, int visited) {
        if (visited == ((1 << N) - 1)) {
            return dist[pos][0]; // Return to starting city
        }
        std::pair<int, int> key = {pos, visited};
        if (memo.find(key) != memo.end()) {
            return memo[key];
        }

        double minCost = std::numeric_limits<double>::infinity();
        for (int city = 0; city < N; ++city) {
            if ((visited & (1 << city)) == 0) {
                double newCost = dist[pos][city] + tsp(city, visited | (1 << city));
                if (newCost < minCost) {
                    minCost = newCost;
                }
            }
        }
        memo[key] = minCost;
        return minCost;
    }

public:
    TSPSolver(const std::vector<std::pair<double, double>>& coords) : coordinates(coords) {
        N = coordinates.size();
        dist.resize(N, std::vector<double>(N));
        // Precompute distances
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                dist[i][j] = euclideanDistance(i, j);
    }

    double solve() {
        return tsp(0, 1); // Start from city 0 with visited bitmask 1
    }
};

int main() {
    int N;
    std::cout << "Enter the number of cities: ";
    std::cin >> N;
    std::vector<std::pair<double, double>> coordinates(N);

    std::cout << "Enter the coordinates of the cities (x y):" << std::endl;
    for (int i = 0; i < N; ++i) {
        std::cin >> coordinates[i].first >> coordinates[i].second;
    }

    if (N > 20) {
        std::cout << "Number of cities too large for dynamic programming solution." << std::endl;
        return 0;
    }

    TSPSolver solver(coordinates);
    double minCost = solver.solve();
    std::cout << "Minimum tour cost: " << minCost << std::endl;

    return 0;
}

