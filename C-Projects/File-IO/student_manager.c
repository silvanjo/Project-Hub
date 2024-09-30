#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student {
    int id;
    char name[100];
    int grade;
};

// Function to add a student record to the file
void addStudent() {
    struct Student student;
    printf("Enter student ID: ");
    scanf("%d", &student.id);
    printf("Enter student name: ");
    getchar(); // To consume newline character left by scanf
    fgets(student.name, sizeof(student.name), stdin);
    student.name[strcspn(student.name, "\n")] = 0; // Remove newline character
    printf("Enter student grade: ");
    scanf("%d", &student.grade);

    FILE *outFile = fopen("students.txt", "a");
    if (outFile != NULL) {
        fprintf(outFile, "ID: %d, Name: %s, Grade: %d\n", student.id, student.name, student.grade);
        fclose(outFile);
        printf("Student added successfully!\n");
    } else {
        printf("Unable to open file.\n");
    }
}

// Function to view all students
void viewStudents() {
    FILE *inFile = fopen("students.txt", "r");
    if (inFile != NULL) {
        char line[256];
        while (fgets(line, sizeof(line), inFile)) {
            printf("%s", line);
        }
        fclose(inFile);
    } else {
        printf("Unable to open file.\n");
    }
}

// Function to search for a student by ID
void searchStudent() {
    int searchID;
    printf("Enter the student ID to search: ");
    scanf("%d", &searchID);

    FILE *inFile = fopen("students.txt", "r");
    if (inFile != NULL) {
        char line[256];
        int found = 0;
        while (fgets(line, sizeof(line), inFile)) {
            if (strstr(line, "ID:") != NULL && atoi(&line[4]) == searchID) {
                printf("Record found: %s", line);
                found = 1;
                break;
            }
        }
        fclose(inFile);
        if (!found) {
            printf("Student with ID %d not found.\n", searchID);
        }
    } else {
        printf("Unable to open file.\n");
    }
}

// Function to update a student's record
void updateStudent() {
    int searchID;
    printf("Enter the student ID to update: ");
    scanf("%d", &searchID);

    FILE *inFile = fopen("students.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (inFile != NULL && tempFile != NULL) {
        char line[256];
        int found = 0;
        while (fgets(line, sizeof(line), inFile)) {
            if (strstr(line, "ID:") != NULL && atoi(&line[4]) == searchID) {
                printf("Current Record: %s", line);
                struct Student student;
                printf("Enter new name: ");
                getchar();
                fgets(student.name, sizeof(student.name), stdin);
                student.name[strcspn(student.name, "\n")] = 0; // Remove newline
                printf("Enter new grade: ");
                scanf("%d", &student.grade);
                fprintf(tempFile, "ID: %d, Name: %s, Grade: %d\n", searchID, student.name, student.grade);
                found = 1;
            } else {
                fputs(line, tempFile);
            }
        }
        fclose(inFile);
        fclose(tempFile);

        remove("students.txt");
        rename("temp.txt", "students.txt");

        if (found) {
            printf("Student updated successfully.\n");
        } else {
            printf("Student with ID %d not found.\n", searchID);
        }
    } else {
        printf("Unable to open file.\n");
    }
}

// Function to delete a student's record
void deleteStudent() {
    int searchID;
    printf("Enter the student ID to delete: ");
    scanf("%d", &searchID);

    FILE *inFile = fopen("students.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (inFile != NULL && tempFile != NULL) {
        char line[256];
        int found = 0;
        while (fgets(line, sizeof(line), inFile)) {
            if (strstr(line, "ID:") != NULL && atoi(&line[4]) == searchID) {
                printf("Deleting record: %s", line);
                found = 1;
            } else {
                fputs(line, tempFile);
            }
        }
        fclose(inFile);
        fclose(tempFile);

        remove("students.txt");
        rename("temp.txt", "students.txt");

        if (found) {
            printf("Student deleted successfully.\n");
        } else {
            printf("Student with ID %d not found.\n");
        }
    } else {
        printf("Unable to open file.\n");
    }
}

int main() {
    int choice;
    do {
        printf("\nStudent Management System\n");
        printf("1. Add Student\n");
        printf("2. View Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

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
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);

    return 0;
}

