#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

struct Student {
    int id;
    std::string name;
    int grade;
};

// Function to add a student record to the file
void addStudent() {
    Student student;
    std::cout << "Enter student ID: ";
    std::cin >> student.id;
    std::cout << "Enter student name: ";
    std::cin.ignore();  // To ignore newline left in the input buffer
    std::getline(std::cin, student.name);
    std::cout << "Enter student grade: ";
    std::cin >> student.grade;

    std::ofstream outFile("students.txt", std::ios::app); // Open file in append mode
    if (outFile.is_open()) {
        outFile << "ID: " << student.id << ", Name: " << student.name << ", Grade: " << student.grade << "\n";
        outFile.close();
        std::cout << "Student added successfully!\n";
    } else {
        std::cout << "Unable to open file.\n";
    }
}

// Function to view all students
void viewStudents() {
    std::ifstream inFile("students.txt");
    if (inFile.is_open()) {
        std::string line;
        while (getline(inFile, line)) {
            std::cout << line << "\n";
        }
        inFile.close();
    } else {
        std::cout << "Unable to open file.\n";
    }
}

// Function to search for a student by ID
void searchStudent() {
    int searchID;
    std::cout << "Enter the student ID to search: ";
    std::cin >> searchID;

    std::ifstream inFile("students.txt");
    if (inFile.is_open()) {
        std::string line;
        bool found = false;
        while (getline(inFile, line)) {
            if (line.find("ID: " + std::to_string(searchID)) != std::string::npos) {
                std::cout << "Record found: " << line << "\n";
                found = true;
                break;
            }
        }
        inFile.close();
        if (!found) {
            std::cout << "Student with ID " << searchID << " not found.\n";
        }
    } else {
        std::cout << "Unable to open file.\n";
    }
}

// Function to update a student's record
void updateStudent() {
    int searchID;
    std::cout << "Enter the student ID to update: ";
    std::cin >> searchID;

    std::ifstream inFile("students.txt");
    std::ofstream tempFile("temp.txt");
    if (inFile.is_open() && tempFile.is_open()) {
        std::string line;
        bool found = false;
        while (getline(inFile, line)) {
            if (line.find("ID: " + std::to_string(searchID)) != std::string::npos) {
                std::cout << "Current Record: " << line << "\n";
                Student student;
                std::cout << "Enter new name: ";
                std::cin.ignore();
                std::getline(std::cin, student.name);
                std::cout << "Enter new grade: ";
                std::cin >> student.grade;
                tempFile << "ID: " << searchID << ", Name: " << student.name << ", Grade: " << student.grade << "\n";
                found = true;
            } else {
                tempFile << line << "\n";
            }
        }
        inFile.close();
        tempFile.close();

        // Replace original file with updated one
        remove("students.txt");
        rename("temp.txt", "students.txt");

        if (found) {
            std::cout << "Student updated successfully.\n";
        } else {
            std::cout << "Student with ID " << searchID << " not found.\n";
        }
    } else {
        std::cout << "Unable to open file.\n";
    }
}

// Function to delete a student's record
void deleteStudent() {
    int searchID;
    std::cout << "Enter the student ID to delete: ";
    std::cin >> searchID;

    std::ifstream inFile("students.txt");
    std::ofstream tempFile("temp.txt");
    if (inFile.is_open() && tempFile.is_open()) {
        std::string line;
        bool found = false;
        while (getline(inFile, line)) {
            if (line.find("ID: " + std::to_string(searchID)) != std::string::npos) {
                std::cout << "Deleting record: " << line << "\n";
                found = true;
            } else {
                tempFile << line << "\n";
            }
        }
        inFile.close();
        tempFile.close();

        // Replace original file with updated one
        remove("students.txt");
        rename("temp.txt", "students.txt");

        if (found) {
            std::cout << "Student deleted successfully.\n";
        } else {
            std::cout << "Student with ID " << searchID << " not found.\n";
        }
    } else {
        std::cout << "Unable to open file.\n";
    }
}

int main() {
    int choice;
    do {
        std::cout << "\nStudent Management System\n";
        std::cout << "1. Add Student\n";
        std::cout << "2. View Students\n";
        std::cout << "3. Search Student\n";
        std::cout << "4. Update Student\n";
        std::cout << "5. Delete Student\n";
        std::cout << "6. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                viewStudents();
                break;
            case 3:
                searchStudent();
                break;
            case 4:
                updateStudent();
                break;
            case 5:
                deleteStudent();
                break;
            case 6:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 6);

    return 0;
}

