#include <iostream>
#include <unordered_map>
#include <string>

class TrieNode {
public:
    bool isEndOfWord;
    std::unordered_map<char, TrieNode*> children;

    TrieNode() {
        isEndOfWord = false;
    }
};

class Trie {
private:
    TrieNode* root;

    void deleteTrie(TrieNode* node) {
        for (auto& pair : node->children) {
            deleteTrie(pair.second);
        }
        delete node;
    }

public:
    Trie() {
        root = new TrieNode();
    }

    ~Trie() {
        deleteTrie(root);
    }

    // Insert a word into the trie
    void insert(const std::string& word) {
        TrieNode* curr = root;
        for (const char& ch : word) {
            if (!curr->children.count(ch)) {
                curr->children[ch] = new TrieNode();
            }
            curr = curr->children[ch];
        }
        curr->isEndOfWord = true;
    }

    // Search for a word in the trie
    bool search(const std::string& word) {
        TrieNode* curr = root;
        for (const char& ch : word) {
            if (!curr->children.count(ch)) {
                return false;
            }
            curr = curr->children[ch];
        }
        return curr->isEndOfWord;
    }

    // Check if any word in the trie starts with the given prefix
    bool startsWith(const std::string& prefix) {
        TrieNode* curr = root;
        for (const char& ch : prefix) {
            if (!curr->children.count(ch)) {
                return false;
            }
            curr = curr->children[ch];
        }
        return true;
    }
};

int main() {
    Trie trie;

    trie.insert("apple");
    trie.insert("app");
    trie.insert("application");

    std::cout << std::boolalpha; // Print bools as true/false

    std::cout << "Search for 'app': " << trie.search("app") << std::endl;
    std::cout << "Search for 'apples': " << trie.search("apples") << std::endl;
    std::cout << "Starts with 'appl': " << trie.startsWith("appl") << std::endl;

    trie.insert("bat");
    trie.insert("batch");

    std::cout << "Search for 'bat': " << trie.search("bat") << std::endl;
    std::cout << "Starts with 'ba': " << trie.startsWith("ba") << std::endl;
    std::cout << "Starts with 'bad': " << trie.startsWith("bad") << std::endl;

    return 0;
}

