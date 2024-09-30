#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <string>
#include <algorithm>
#include <iomanip>

// Enumeration for item rarity
enum class Rarity {
    Common,
    Uncommon,
    Rare,
    Epic,
    Legendary
};

// Function to convert Rarity enum to string
std::string rarityToString(Rarity rarity) {
    switch (rarity) {
        case Rarity::Common: return "Common";
        case Rarity::Uncommon: return "Uncommon";
        case Rarity::Rare: return "Rare";
        case Rarity::Epic: return "Epic";
        case Rarity::Legendary: return "Legendary";
        default: return "Unknown";
    }
}

// Item structure
struct Item {
    int id;
    std::string name;
    std::string type;
    int value;
    Rarity rarity;

    // Overload < operator for priority queue (based on rarity)
    bool operator<(const Item& other) const {
        return static_cast<int>(rarity) < static_cast<int>(other.rarity);
    }
};

// Inventory class
class Inventory {
private:
    std::vector<Item> items;                             // Stores all items
    std::map<std::string, std::vector<Item>> typeMap;    // Categorizes items by type
    int nextItemId;

public:
    Inventory() : nextItemId(1) {}

    // Add an item to the inventory
    void addItem(const std::string& name, const std::string& type, int value, Rarity rarity) {
        Item newItem = { nextItemId++, name, type, value, rarity };
        items.push_back(newItem);
        typeMap[type].push_back(newItem);
        std::cout << "Added item: " << name << " (ID: " << newItem.id << ")\n";
    }

    // Remove an item by ID
    bool removeItem(int itemId) {
        auto it = std::find_if(items.begin(), items.end(), [itemId](const Item& item) {
            return item.id == itemId;
        });

        if (it != items.end()) {
            // Remove from typeMap
            auto& typeVec = typeMap[it->type];
            typeVec.erase(std::remove_if(typeVec.begin(), typeVec.end(), [itemId](const Item& item) {
                return item.id == itemId;
            }), typeVec.end());

            // Remove from items
            items.erase(it);
            std::cout << "Removed item with ID: " << itemId << "\n";
            return true;
        } else {
            std::cout << "Item with ID " << itemId << " not found.\n";
            return false;
        }
    }

    // Display all items
    void displayAllItems() const {
        std::cout << "\nAll Items in Inventory:\n";
        std::cout << std::left << std::setw(5) << "ID"
                  << std::setw(20) << "Name"
                  << std::setw(15) << "Type"
                  << std::setw(10) << "Value"
                  << std::setw(12) << "Rarity" << "\n";
        std::cout << "-------------------------------------------------------------\n";
        for (const auto& item : items) {
            std::cout << std::left << std::setw(5) << item.id
                      << std::setw(20) << item.name
                      << std::setw(15) << item.type
                      << std::setw(10) << item.value
                      << std::setw(12) << rarityToString(item.rarity) << "\n";
        }
    }

    // Display items sorted by rarity
    void displayItemsByRarity() const {
        // Create a max heap priority queue based on rarity
        auto cmp = [](const Item& a, const Item& b) {
            return a < b; // Use the overloaded operator<
        };
        std::priority_queue<Item, std::vector<Item>, decltype(cmp)> pq(cmp, items);

        std::cout << "\nItems Sorted by Rarity (Highest to Lowest):\n";
        std::cout << std::left << std::setw(5) << "ID"
                  << std::setw(20) << "Name"
                  << std::setw(15) << "Type"
                  << std::setw(10) << "Value"
                  << std::setw(12) << "Rarity" << "\n";
        std::cout << "-------------------------------------------------------------\n";
        while (!pq.empty()) {
            Item item = pq.top();
            pq.pop();
            std::cout << std::left << std::setw(5) << item.id
                      << std::setw(20) << item.name
                      << std::setw(15) << item.type
                      << std::setw(10) << item.value
                      << std::setw(12) << rarityToString(item.rarity) << "\n";
        }
    }

    // Display items sorted by value
    void displayItemsByValue() const {
        // Copy items to a vector and sort by value
        std::vector<Item> sortedItems = items;
        std::sort(sortedItems.begin(), sortedItems.end(), [](const Item& a, const Item& b) {
            return b.value < a.value; // Sort in descending order
        });

        std::cout << "\nItems Sorted by Value (Highest to Lowest):\n";
        std::cout << std::left << std::setw(5) << "ID"
                  << std::setw(20) << "Name"
                  << std::setw(15) << "Type"
                  << std::setw(10) << "Value"
                  << std::setw(12) << "Rarity" << "\n";
        std::cout << "-------------------------------------------------------------\n";
        for (const auto& item : sortedItems) {
            std::cout << std::left << std::setw(5) << item.id
                      << std::setw(20) << item.name
                      << std::setw(15) << item.type
                      << std::setw(10) << item.value
                      << std::setw(12) << rarityToString(item.rarity) << "\n";
        }
    }

    // Display items by type
    void displayItemsByType(const std::string& type) const {
        auto it = typeMap.find(type);
        if (it != typeMap.end()) {
            std::cout << "\nItems of Type: " << type << "\n";
            std::cout << std::left << std::setw(5) << "ID"
                      << std::setw(20) << "Name"
                      << std::setw(10) << "Value"
                      << std::setw(12) << "Rarity" << "\n";
            std::cout << "---------------------------------------------\n";
            for (const auto& item : it->second) {
                std::cout << std::left << std::setw(5) << item.id
                          << std::setw(20) << item.name
                          << std::setw(10) << item.value
                          << std::setw(12) << rarityToString(item.rarity) << "\n";
            }
        } else {
            std::cout << "No items of type " << type << " found.\n";
        }
    }
};

int main() {
    Inventory inventory;
    int choice;

    do {
        std::cout << "\nGame Inventory System\n";
        std::cout << "1. Add Item\n";
        std::cout << "2. Remove Item\n";
        std::cout << "3. Display All Items\n";
        std::cout << "4. Display Items by Rarity\n";
        std::cout << "5. Display Items by Value\n";
        std::cout << "6. Display Items by Type\n";
        std::cout << "7. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        std::string name, type;
        int value, rarityInput, itemId;
        Rarity rarity;

        switch (choice) {
            case 1:
                std::cout << "Enter item name: ";
                std::cin.ignore();
                std::getline(std::cin, name);
                std::cout << "Enter item type (e.g., Weapon, Armor, Potion): ";
                std::getline(std::cin, type);
                std::cout << "Enter item value: ";
                std::cin >> value;
                std::cout << "Enter item rarity (0: Common, 1: Uncommon, 2: Rare, 3: Epic, 4: Legendary): ";
                std::cin >> rarityInput;
                if (rarityInput >= 0 && rarityInput <= 4) {
                    rarity = static_cast<Rarity>(rarityInput);
                    inventory.addItem(name, type, value, rarity);
                } else {
                    std::cout << "Invalid rarity input.\n";
                }
                break;
            case 2:
                std::cout << "Enter item ID to remove: ";
                std::cin >> itemId;
                inventory.removeItem(itemId);
                break;
            case 3:
                inventory.displayAllItems();
                break;
            case 4:
                inventory.displayItemsByRarity();
                break;
            case 5:
                inventory.displayItemsByValue();
                break;
            case 6:
                std::cout << "Enter item type to display: ";
                std::cin.ignore();
                std::getline(std::cin, type);
                inventory.displayItemsByType(type);
                break;
            case 7:
                std::cout << "Exiting inventory system.\n";
                break;
            default:
                std::cout << "Invalid choice.\n";
        }

    } while (choice != 7);

    return 0;
}

