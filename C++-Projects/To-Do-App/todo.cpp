#include <iostream>
#include <vector>
#include <map>
#include <string>

// Task structure
struct Task {
    std::string description;
    int priority;
    std::string dueDate;
};

// Function to add a task
void addTask(std::vector<Task>& tasks) {
    Task newTask;
    std::cout << "Enter task description: ";
    std::getline(std::cin, newTask.description);
    std::cout << "Enter task priority (1-5): ";
    std::cin >> newTask.priority;
    std::cin.ignore(); // Clear newline character
    std::cout << "Enter due date (YYYY-MM-DD): ";
    std::getline(std::cin, newTask.dueDate);
    tasks.push_back(newTask);
}

// Function to display tasks categorized by priority
void displayTasksByPriority(const std::vector<Task>& tasks) {
    std::map<int, std::vector<Task>> priorityMap;
    for (const auto& task : tasks) {
        priorityMap[task.priority].push_back(task);
    }
    for (const auto& [priority, taskList] : priorityMap) {
        std::cout << "\nPriority " << priority << " Tasks:\n";
        for (const auto& task : taskList) {
            std::cout << "- " << task.description << " (Due: " << task.dueDate << ")\n";
        }
    }
}

// Function to display tasks categorized by due date
void displayTasksByDate(const std::vector<Task>& tasks) {
    std::map<std::string, std::vector<Task>> dateMap;
    for (const auto& task : tasks) {
        dateMap[task.dueDate].push_back(task);
    }
    for (const auto& [date, taskList] : dateMap) {
        std::cout << "\nTasks due on " << date << ":\n";
        for (const auto& task : taskList) {
            std::cout << "- " << task.description << " (Priority: " << task.priority << ")\n";
        }
    }
}

// Function to remove a task
void removeTask(std::vector<Task>& tasks) {
    int index;
    std::cout << "Enter the index of the task to remove: ";
    std::cin >> index;
    std::cin.ignore(); // Clear newline character
    if (index >= 0 && index < tasks.size()) {
        tasks.erase(tasks.begin() + index);
        std::cout << "Task removed successfully.\n";
    } else {
        std::cout << "Invalid index.\n";
    }
}

// Main menu
int main() {
    std::vector<Task> tasks;
    int choice;
    do {
        std::cout << "\nTo-Do List Application\n";
        std::cout << "1. Add Task\n";
        std::cout << "2. Display Tasks by Priority\n";
        std::cout << "3. Display Tasks by Due Date\n";
        std::cout << "4. Remove Task\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore(); // Clear newline character
        switch (choice) {
            case 1:
                addTask(tasks);
                break;
            case 2:
                displayTasksByPriority(tasks);
                break;
            case 3:
                displayTasksByDate(tasks);
                break;
            case 4:
                removeTask(tasks);
                break;
            case 5:
                std::cout << "Exiting application.\n";
                break;
            default:
                std::cout << "Invalid choice.\n";
        }
    } while (choice != 5);
    return 0;
}

