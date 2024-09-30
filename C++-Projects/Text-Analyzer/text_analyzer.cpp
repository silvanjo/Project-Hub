#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cctype>

// Function to remove punctuation and convert to lowercase
std::string sanitizeWord(const std::string& word) {
    std::string sanitized;
    for (char ch : word) {
        if (std::isalpha(static_cast<unsigned char>(ch))) {
            sanitized += std::tolower(static_cast<unsigned char>(ch));
        }
    }
    return sanitized;
}

int main() {
    std::unordered_map<std::string, int> wordCount;
    std::string filename;

    // Ask user for the filename
    std::cout << "Enter the filename: ";
    std::getline(std::cin, filename);

    // Open the file
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return 1;
    }

    // Read words from the file and count occurrences
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string word;
        while (ss >> word) {
            word = sanitizeWord(word);
            if (!word.empty()) {
                ++wordCount[word];
            }
        }
    }
    file.close();

    // Transfer the word counts to a vector
    std::vector<std::pair<std::string, int>> wordFreq(wordCount.begin(), wordCount.end());

    // Sort the vector by frequency in descending order
    std::sort(wordFreq.begin(), wordFreq.end(),
        [](const auto& a, const auto& b) {
            return b.second > a.second; // Sort by frequency descending
        });

    // Display the words and their frequencies
    std::cout << "\nWord Frequencies:\n";
    for (const auto& [word, count] : wordFreq) {
        std::cout << word << ": " << count << std::endl;
    }

    return 0;
}

