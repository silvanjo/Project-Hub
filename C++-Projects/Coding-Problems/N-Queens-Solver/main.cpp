#include <iostream>
#include <vector>

class NQueensSolver {
private:
    int N;
    int solutionsCount;
    std::vector<int> board; // board[i] = column position of queen in row i
    std::vector<std::vector<std::string>> allSolutions;

    bool isSafe(int row, int col) {
        for (int i = 0; i < row; ++i) {
            int qCol = board[i];
            if (qCol == col || abs(qCol - col) == abs(i - row)) {
                return false;
            }
        }
        return true;
    }

    void solve(int row) {
        if (row == N) {
            ++solutionsCount;
            saveSolution();
            return;
        }
        for (int col = 0; col < N; ++col) {
            if (isSafe(row, col)) {
                board[row] = col;
                solve(row + 1);
                // Backtrack
                board[row] = -1;
            }
        }
    }

    void saveSolution() {
        std::vector<std::string> solution(N, std::string(N, '.'));
        for (int i = 0; i < N; ++i) {
            solution[i][board[i]] = 'Q';
        }
        allSolutions.push_back(solution);
    }

public:
    NQueensSolver(int n) : N(n), solutionsCount(0), board(n, -1) {}

    void findSolutions() {
        solve(0);
    }

    int getSolutionsCount() const {
        return solutionsCount;
    }

    void printSolutions() const {
        for (const auto& solution : allSolutions) {
            for (const auto& row : solution) {
                std::cout << row << std::endl;
            }
            std::cout << std::endl;
        }
    }
};

int main() {
    int N;
    std::cout << "Enter the value of N for N-Queens problem: ";
    std::cin >> N;

    NQueensSolver solver(N);
    solver.findSolutions();

    std::cout << "Total number of solutions: " << solver.getSolutionsCount() << std::endl;
    std::cout << "Do you want to print all solutions? (y/n): ";
    char choice;
    std::cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        solver.printSolutions();
    }

    return 0;
}

