#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>

const std::string FILENAME = "passwords.dat";
const char KEY = 'X'; // Simple XOR encryption key

void encryptDecrypt(std::string& data) {
    for (char& c : data) {
        c ^= KEY;
    }
}

void savePasswords(const std::unordered_map<std::string, std::string>& passwords) {
    std::ofstream file(FILENAME, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file for writing.\n";
        return;
    }

    for (const auto& pair : passwords) {
        std::string line = pair.first + ":" + pair.second + "\n";
        encryptDecrypt(line);
        file << line;
    }
    file.close();
}

void loadPasswords(std::unordered_map<std::string, std::string>& passwords) {
    std::ifstream file(FILENAME, std::ios::binary);
    if (!file.is_open()) {
        // No passwords saved yet
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        encryptDecrypt(line);
        size_t delimiterPos = line.find(':');
        if (delimiterPos != std::string::npos) {
            std::string account = line.substr(0, delimiterPos);
            std::string password = line.substr(delimiterPos + 1);
            passwords[account] = password;
        }
    }
    file.close();
}

void addOrUpdatePassword(std::unordered_map<std::string, std::string>& passwords) {
    std::string account, password;
    std::cout << "Enter account name: ";
    std::cin >> account;
    std::cout << "Enter password: ";
    std::cin >> password;
    passwords[account] = password;
    savePasswords(passwords);
    std::cout << "Password saved successfully.\n";
}

void retrievePassword(const std::unordered_map<std::string, std::string>& passwords) {
    std::string account;
    std::cout << "Enter account name: ";
    std::cin >> account;
    auto it = passwords.find(account);
    if (it != passwords.end()) {
        std::cout << "Password for " << account << ": " << it->second << "\n";
    } else {
        std::cout << "Account not found.\n";
    }
}

void deletePassword(std::unordered_map<std::string, std::string>& passwords) {
    std::string account;
    std::cout << "Enter account name to delete: ";
    std::cin >> account;
    if (passwords.erase(account)) {
        savePasswords(passwords);
        std::cout << "Account deleted successfully.\n";
    } else {
        std::cout << "Account not found.\n";
    }
}

void listAccounts(const std::unordered_map<std::string, std::string>& passwords) {
    if (passwords.empty()) {
        std::cout << "No accounts stored.\n";
        return;
    }
    std::cout << "Stored accounts:\n";
    for (const auto& pair : passwords) {
        std::cout << "- " << pair.first << "\n";
    }
}

int main() {
    std::unordered_map<std::string, std::string> passwords;
    loadPasswords(passwords);

    int choice;
    do {
        std::cout << "\nPassword Manager\n";
        std::cout << "1. Add/Update Password\n";
        std::cout << "2. Retrieve Password\n";
        std::cout << "3. Delete Password\n";
        std::cout << "4. List Accounts\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                addOrUpdatePassword(passwords);
                break;
            case 2:
                retrievePassword(passwords);
                break;
            case 3:
                deletePassword(passwords);
                break;
            case 4:
                listAccounts(passwords);
                break;
            case 5:
                std::cout << "Exiting Password Manager.\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 5);

    return 0;
}

