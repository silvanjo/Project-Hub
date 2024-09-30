#include <iostream>
#include <vector>

#define UNASSIGNED 0
#define N 9

class SudokuSolver {
private:
    int grid[N][N];

    bool findUnassignedLocation(int &row, int &col) {
        for (row = 0; row < N; ++row)
            for (col = 0; col < N; ++col)
                if (grid[row][col] == UNASSIGNED)
                    return true;
        return false;
    }

    bool isSafe(int row, int col, int num) {
        return !usedInRow(row, num) &&
               !usedInCol(col, num) &&
               !usedInBox(row - row % 3, col - col % 3, num);
    }

    bool usedInRow(int row, int num) {
        for (int col = 0; col < N; ++col)
            if (grid[row][col] == num)
                return true;
        return false;
    }

    bool usedInCol(int col, int num) {
        for (int row = 0; row < N; ++row)
            if (grid[row][col] == num)
                return true;
        return false;
    }

    bool usedInBox(int boxStartRow, int boxStartCol, int num) {
        for (int row = 0; row < 3; ++row)
            for (int col = 0; col < 3; ++col)
                if (grid[row + boxStartRow][col + boxStartCol] == num)
                    return true;
        return false;
    }

    bool solveSudoku() {
        int row, col;
        if (!findUnassignedLocation(row, col))
            return true; // Puzzle solved

        for (int num = 1; num <= 9; ++num) {
            if (isSafe(row, col, num)) {
                grid[row][col] = num;

                if (solveSudoku())
                    return true;

                grid[row][col] = UNASSIGNED; // Backtrack
            }
        }
        return false; // Trigger backtracking
    }

    void printGrid() {
        for (int row = 0; row < N; ++row) {
            if (row % 3 == 0 && row != 0)
                std::cout << "------+-------+------" << std::endl;
            for (int col = 0; col < N; ++col) {
                if (col % 3 == 0 && col != 0)
                    std::cout << "| ";
                std::cout << grid[row][col] << " ";
            }
            std::cout << std::endl;
        }
    }

public:
    void inputGrid() {
        std::cout << "Enter the Sudoku puzzle (use 0 for empty cells):" << std::endl;
        for (int row = 0; row < N; ++row)
            for (int col = 0; col < N; ++col)
                std::cin >> grid[row][col];
    }

    void solve() {
        if (solveSudoku()) {
            std::cout << "Solved Sudoku puzzle:" << std::endl;
            printGrid();
        } else {
            std::cout << "No solution exists." << std::endl;
        }
    }
};

int main() {
    SudokuSolver solver;
    solver.inputGrid();
    solver.solve();
    return 0;
}

