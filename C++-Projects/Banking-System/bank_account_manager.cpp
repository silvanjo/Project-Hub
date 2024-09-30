#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <iomanip>

// Account structure
struct Account {
    std::string ownerName;
    double balance;
};

// Bank class
class Bank {
private:
    std::map<int, Account> accounts;
    int nextAccountNumber;

public:
    Bank() : nextAccountNumber(1001) {}

    // Create a new account
    int createAccount(const std::string& ownerName, double initialDeposit) {
        int accountNumber = nextAccountNumber++;
        accounts[accountNumber] = Account{ownerName, initialDeposit};
        std::cout << "Account created successfully. Account Number: " << accountNumber << "\n";
        return accountNumber;
    }

    // Deposit funds into an account
    bool deposit(int accountNumber, double amount) {
        auto it = accounts.find(accountNumber);
        if (it != accounts.end() && amount > 0) {
            it->second.balance += amount;
            std::cout << "Deposited $" << amount << " into account " << accountNumber << ".\n";
            return true;
        } else {
            std::cout << "Deposit failed. Check account number and amount.\n";
            return false;
        }
    }

    // Withdraw funds from an account
    bool withdraw(int accountNumber, double amount) {
        auto it = accounts.find(accountNumber);
        if (it != accounts.end() && amount > 0 && it->second.balance >= amount) {
            it->second.balance -= amount;
            std::cout << "Withdrew $" << amount << " from account " << accountNumber << ".\n";
            return true;
        } else {
            std::cout << "Withdrawal failed. Check account number, amount, and balance.\n";
            return false;
        }
    }

    // Transfer funds between accounts
    bool transfer(int fromAccount, int toAccount, double amount) {
        auto fromIt = accounts.find(fromAccount);
        auto toIt = accounts.find(toAccount);
        if (fromIt != accounts.end() && toIt != accounts.end() && amount > 0 && fromIt->second.balance >= amount) {
            fromIt->second.balance -= amount;
            toIt->second.balance += amount;
            std::cout << "Transferred $" << amount << " from account " << fromAccount << " to account " << toAccount << ".\n";
            return true;
        } else {
            std::cout << "Transfer failed. Check account numbers, amount, and balance.\n";
            return false;
        }
    }

    // Get account details
    std::tuple<std::string, double> getAccountDetails(int accountNumber) const {
        auto it = accounts.find(accountNumber);
        if (it != accounts.end()) {
            return std::make_tuple(it->second.ownerName, it->second.balance);
        } else {
            return std::make_tuple("", -1.0);
        }
    }

    // Display all accounts
    void displayAllAccounts() const {
        std::cout << "\nAll Accounts:\n";
        std::cout << std::left << std::setw(15) << "Account Number"
                  << std::setw(20) << "Owner Name"
                  << std::setw(10) << "Balance" << "\n";
        std::cout << "----------------------------------------------\n";
        for (const auto& [accountNumber, account] : accounts) {
            std::cout << std::left << std::setw(15) << accountNumber
                      << std::setw(20) << account.ownerName
                      << std::setw(10) << account.balance << "\n";
        }
    }
};

// Main menu
int main() {
    Bank bank;
    int choice;

    do {
        std::cout << "\nBank Account Manager\n";
        std::cout << "1. Create Account\n";
        std::cout << "2. Deposit Funds\n";
        std::cout << "3. Withdraw Funds\n";
        std::cout << "4. Transfer Funds\n";
        std::cout << "5. View Account Details\n";
        std::cout << "6. Display All Accounts\n";
        std::cout << "7. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        int accountNumber, toAccount;
        double amount;
        std::string ownerName;

        switch (choice) {
            case 1:
                std::cout << "Enter owner's name: ";
                std::cin.ignore();
                std::getline(std::cin, ownerName);
                std::cout << "Enter initial deposit amount: ";
                std::cin >> amount;
                bank.createAccount(ownerName, amount);
                break;
            case 2:
                std::cout << "Enter account number: ";
                std::cin >> accountNumber;
                std::cout << "Enter deposit amount: ";
                std::cin >> amount;
                bank.deposit(accountNumber, amount);
                break;
            case 3:
                std::cout << "Enter account number: ";
                std::cin >> accountNumber;
                std::cout << "Enter withdrawal amount: ";
                std::cin >> amount;
                bank.withdraw(accountNumber, amount);
                break;
            case 4:
                std::cout << "Enter source account number: ";
                std::cin >> accountNumber;
                std::cout << "Enter destination account number: ";
                std::cin >> toAccount;
                std::cout << "Enter transfer amount: ";
                std::cin >> amount;
                bank.transfer(accountNumber, toAccount, amount);
                break;
            case 5:
                std::cout << "Enter account number: ";
                std::cin >> accountNumber;
                {
                    auto [name, balance] = bank.getAccountDetails(accountNumber);
                    if (balance != -1.0) {
                        std::cout << "\nAccount Details:\n";
                        std::cout << "Owner Name: " << name << "\n";
                        std::cout << "Balance: $" << balance << "\n";
                    } else {
                        std::cout << "Account not found.\n";
                    }
                }
                break;
            case 6:
                bank.displayAllAccounts();
                break;
            case 7:
                std::cout << "Exiting application.\n";
                break;
            default:
                std::cout << "Invalid choice.\n";
        }

    } while (choice != 7);

    return 0;
}

