#include <iostream>
#include <vector>
#include <algorithm>

// Recursive function to solve the Knapsack problem
int knapsackRecursive(const std::vector<int>& weights, const std::vector<int>& values,
                      int capacity, int n) {
    // Base case: No items left or capacity is 0
    if (n == 0 || capacity == 0)
        return 0;

    // If weight of the nth item is more than the knapsack capacity, it cannot be included
    if (weights[n - 1] > capacity)
        return knapsackRecursive(weights, values, capacity, n - 1);
    else {
        // Return the maximum of including or excluding the nth item
        int includeItem = values[n - 1] + knapsackRecursive(weights, values, capacity - weights[n - 1], n - 1);
        int excludeItem = knapsackRecursive(weights, values, capacity, n - 1);
        return std::max(includeItem, excludeItem);
    }
}

// DP function to solve the Knapsack problem
int knapsackDP(const std::vector<int>& weights, const std::vector<int>& values,
               int capacity, int n) {
    // Create a DP table
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(capacity + 1));

    // Build table dp[][] in bottom-up manner
    for (int i = 0; i <= n; ++i) {            // Items
        for (int w = 0; w <= capacity; ++w) { // Capacities
            if (i == 0 || w == 0)
                dp[i][w] = 0;  // Base case
            else if (weights[i - 1] <= w)
                dp[i][w] = std::max(
                    values[i - 1] + dp[i - 1][w - weights[i - 1]],  // Include item
                    dp[i - 1][w]                                    // Exclude item
                );
            else
                dp[i][w] = dp[i - 1][w];  // Cannot include item
        }
    }

    return dp[n][capacity];
}

int main() {
    std::vector<int> weights = {2, 3, 4, 5};  // Weights of items
    std::vector<int> values = {3, 4, 5, 6};   // Values of items
    int capacity = 5;                          // Knapsack capacity
    int n = weights.size();                    // Number of items

    int maxValue = knapsackDP(weights, values, capacity, n);
    std::cout << "Maximum value (Recursive): " << maxValue << std::endl;

    return 0;
}

