#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// Function to compute the edit distance between two strings
int editDistance(const std::string& s1, const std::string& s2) {
    int m = s1.size();
    int n = s2.size();

    // DP table to store the distances
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));

    // Initialize the table
    for (int i = 0; i <= m; ++i) {      // For all prefixes of s1
        for (int j = 0; j <= n; ++j) {  // For all prefixes of s2
            if (i == 0)
                dp[i][j] = j;  // Min operations = j (insertions)
            else if (j == 0)
                dp[i][j] = i;  // Min operations = i (deletions)
            else if (s1[i - 1] == s2[j - 1])
                dp[i][j] = dp[i - 1][j - 1];  // Characters match
            else
                dp[i][j] = 1 + std::min({ dp[i - 1][j],     // Deletion
                                           dp[i][j - 1],     // Insertion
                                           dp[i - 1][j - 1]  // Substitution
                                         });
        }
    }

    return dp[m][n];
}

int main() {
    std::string s1 = "kitten";
    std::string s2 = "sitting";

    int distance = editDistance(s1, s2);
    std::cout << "Edit Distance between \"" << s1 << "\" and \"" << s2 << "\": " << distance << std::endl;

    return 0;
}

