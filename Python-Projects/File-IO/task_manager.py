import json
import os

# Load tasks from the JSON file
def load_tasks(filename="tasks.json"):
    if os.path.exists(filename):
        with open(filename, "r") as file:
            return json.load(file)
    return {"tasks": []}

# Save tasks to the JSON file
def save_tasks(tasks, filename="tasks.json"):
    with open(filename, "w") as file:
        json.dump(tasks, file, indent=4)

# Add a new task
def add_task(tasks):
    task_id = len(tasks["tasks"]) + 1
    description = input("Enter task description: ")
    deadline = input("Enter deadline (YYYY-MM-DD): ")
    priority = input("Enter priority (low, medium, high): ")
    status = "incomplete"
    
    tasks["tasks"].append({
        "id": task_id,
        "description": description,
        "deadline": deadline,
        "priority": priority,
        "status": status
    })
    print("Task added successfully!")

# View all tasks
def view_tasks(tasks):
    if not tasks["tasks"]:
        print("No tasks available.")
        return

    for task in tasks["tasks"]:
        print(f"ID: {task['id']}, Description: {task['description']}, Deadline: {task['deadline']}, "
              f"Priority: {task['priority']}, Status: {task['status']}")

# Update an existing task
def update_task(tasks):
    task_id = int(input("Enter task ID to update: "))
    for task in tasks["tasks"]:
        if task["id"] == task_id:
            print(f"Current Record: ID: {task['id']}, Description: {task['description']}, "
                  f"Deadline: {task['deadline']}, Priority: {task['priority']}, Status: {task['status']}")
            task["description"] = input("Enter new task description: ") or task["description"]
            task["deadline"] = input("Enter new deadline (YYYY-MM-DD): ") or task["deadline"]
            task["priority"] = input("Enter new priority (low, medium, high): ") or task["priority"]
            task["status"] = input("Enter new status (incomplete, complete): ") or task["status"]
            print("Task updated successfully!")
            return
    print("Task not found.")

# Delete a task
def delete_task(tasks):
    task_id = int(input("Enter task ID to delete: "))
    for task in tasks["tasks"]:
        if task["id"] == task_id:
            tasks["tasks"].remove(task)
            print("Task deleted successfully!")
            return
    print("Task not found.")

# Filter tasks by priority or status
def filter_tasks(tasks):
    filter_type = input("Filter by (priority/status): ").lower()
    if filter_type == "priority":
        priority = input("Enter priority (low, medium, high): ").lower()
        filtered_tasks = [task for task in tasks["tasks"] if task["priority"].lower() == priority]
    elif filter_type == "status":
        status = input("Enter status (incomplete, complete): ").lower()
        filtered_tasks = [task for task in tasks["tasks"] if task["status"].lower() == status]
    else:
        print("Invalid filter type.")
        return

    if filtered_tasks:
        for task in filtered_tasks:
            print(f"ID: {task['id']}, Description: {task['description']}, Deadline: {task['deadline']}, "
                  f"Priority: {task['priority']}, Status: {task['status']}")
    else:
        print(f"No tasks found for the given {filter_type}.")

# Main menu function
def main():
    tasks = load_tasks()

    while True:
        print("\nTask Management System")
        print("1. Add Task")
        print("2. View All Tasks")
        print("3. Update Task")
        print("4. Delete Task")
        print("5. Filter Tasks")
        print("6. Exit")

        choice = input("Enter your choice: ")

        if choice == "1":
            add_task(tasks)
        elif choice == "2":
            view_tasks(tasks)
        elif choice == "3":
            update_task(tasks)
        elif choice == "4":
            delete_task(tasks)
        elif choice == "5":
            filter_tasks(tasks)
        elif choice == "6":
            save_tasks(tasks)
            print("Tasks saved. Exiting...")
            break
        else:
            print("Invalid choice, please try again.")

if __name__ == "__main__":
    main()

