import os

# Function to add a student record to the file
def add_student():
    student_id = input("Enter student ID: ")
    name = input("Enter student name: ")
    grade = input("Enter student grade: ")

    with open("students.txt", "a") as file:
        file.write(f"ID: {student_id}, Name: {name}, Grade: {grade}\n")

    print("Student added successfully!")

# Function to view all students
def view_students():
    if not os.path.exists("students.txt"):
        print("No students to display.")
        return
    
    with open("students.txt", "r") as file:
        for line in file:
            print(line, end="")
    
    print()

# Function to search for a student by ID
def search_student():
    search_id = input("Enter the student ID to search: ")

    if not os.path.exists("students.txt"):
        print("No students to search.")
        return
    
    found = False
    with open("students.txt", "r") as file:
        for line in file:
            if f"ID: {search_id}" in line:
                print(f"Record found: {line}", end="")
                found = True
                break
    
    if not found:
        print(f"Student with ID {search_id} not found.")

# Function to update a student's record
def update_student():
    search_id = input("Enter the student ID to update: ")

    if not os.path.exists("students.txt"):
        print("No students to update.")
        return
    
    found = False
    with open("students.txt", "r") as file:
        lines = file.readlines()
    
    with open("temp.txt", "w") as temp_file:
        for line in lines:
            if f"ID: {search_id}" in line:
                print(f"Current Record: {line}", end="")
                name = input("Enter new name: ")
                grade = input("Enter new grade: ")
                temp_file.write(f"ID: {search_id}, Name: {name}, Grade: {grade}\n")
                found = True
            else:
                temp_file.write(line)
    
    os.remove("students.txt")
    os.rename("temp.txt", "students.txt")

    if found:
        print("Student updated successfully.")
    else:
        print(f"Student with ID {search_id} not found.")

# Function to delete a student's record
def delete_student():
    search_id = input("Enter the student ID to delete: ")

    if not os.path.exists("students.txt"):
        print("No students to delete.")
        return
    
    found = False
    with open("students.txt", "r") as file:
        lines = file.readlines()
    
    with open("temp.txt", "w") as temp_file:
        for line in lines:
            if f"ID: {search_id}" in line:
                print(f"Deleting record: {line}", end="")
                found = True
            else:
                temp_file.write(line)
    
    os.remove("students.txt")
    os.rename("temp.txt", "students.txt")

    if found:
        print("Student deleted successfully.")
    else:
        print(f"Student with ID {search_id} not found.")

# Main function to run the menu
def main():
    while True:
        print("\nStudent Management System")
        print("1. Add Student")
        print("2. View Students")
        print("3. Search Student")
        print("4. Update Student")
        print("5. Delete Student")
        print("6. Exit")

        choice = input("Enter your choice: ")

        if choice == '1':
            add_student()
        elif choice == '2':
            view_students()
        elif choice == '3':
            search_student()
        elif choice == '4':
            update_student()
        elif choice == '5':
            delete_student()
        elif choice == '6':
            print("Exiting...")
            break
        else:
            print("Invalid choice. Please try again.")

if __name__ == "__main__":
    main()

