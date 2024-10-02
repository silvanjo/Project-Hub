#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#define DATABASE "tasks.db"

// Function declarations
void initialize_db();
void add_task(sqlite3 *db, const char *description, const char *detailed_description, const char *due_date);
void mark_task_completed(sqlite3 *db, int id);
void list_tasks(sqlite3 *db, int completed);
void view_task(sqlite3 *db, int id);

int main() {
    sqlite3 *db;
    int rc;
    char option;
    char description[256];
    char detailed_description[512];
    char due_date[20];
    int task_id;

    // Open the database
    rc = sqlite3_open(DATABASE, &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    // Initialize the database
    initialize_db(db);

    // Main loop
    while (1) {
        printf("\nTask Manager:\n");
        printf("1. Add task\n");
        printf("2. Mark task as completed\n");
        printf("3. List pending tasks\n");
        printf("4. List completed tasks\n");
        printf("5. View task details\n");
        printf("6. Exit\n");
        printf("Choose an option: ");
        scanf(" %c", &option);

        switch (option) {
            case '1':
                printf("Enter task description: ");
                getchar();  // To consume leftover newline
                fgets(description, 256, stdin);
                description[strcspn(description, "\n")] = 0;  // Remove newline
                printf("Enter detailed description: ");
                fgets(detailed_description, 512, stdin);
                detailed_description[strcspn(detailed_description, "\n")] = 0;  // Remove newline
                printf("Enter due date (YYYY-MM-DD): ");
                scanf("%s", due_date);
                add_task(db, description, detailed_description, due_date);
                break;
            case '2':
                printf("Enter task ID to mark as completed: ");
                scanf("%d", &task_id);
                mark_task_completed(db, task_id);
                break;
            case '3':
                printf("\nPending tasks:\n");
                list_tasks(db, 0);
                break;
            case '4':
                printf("\nCompleted tasks:\n");
                list_tasks(db, 1);
                break;
            case '5':
                printf("Enter task ID to view details: ");
                scanf("%d", &task_id);
                view_task(db, task_id);
                break;
            case '6':
                sqlite3_close(db);
                return 0;
            default:
                printf("Invalid option, please try again.\n");
        }
    }
}

// Initialize the database by creating the table if it doesn't exist
void initialize_db(sqlite3 *db) {
    char *err_msg = 0;
    const char *sql = "CREATE TABLE IF NOT EXISTS tasks ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "description TEXT NOT NULL, "
                      "detailed_description TEXT, "
                      "due_date TEXT, "
                      "is_completed INTEGER DEFAULT 0);";

    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("Database initialized successfully.\n");
    }
}

// Add a new task to the database
void add_task(sqlite3 *db, const char *description, const char *detailed_description, const char *due_date) {
    char *err_msg = 0;
    char sql[1024];

    snprintf(sql, sizeof(sql), 
             "INSERT INTO tasks (description, detailed_description, due_date) VALUES ('%s', '%s', '%s');", 
             description, detailed_description, due_date);

    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("Task added successfully.\n");
    }
}

// Mark a task as completed by updating the 'is_completed' field
void mark_task_completed(sqlite3 *db, int id) {
    char *err_msg = 0;
    char sql[128];

    snprintf(sql, sizeof(sql), "UPDATE tasks SET is_completed = 1 WHERE id = %d;", id);

    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("Task %d marked as completed.\n", id);
    }
}

// List tasks based on completion status (0 = pending, 1 = completed)
void list_tasks(sqlite3 *db, int completed) {
    const char *sql = "SELECT id, description, due_date FROM tasks WHERE is_completed = ?;";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch tasks: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Bind the completed flag (0 or 1)
    sqlite3_bind_int(stmt, 1, completed);

    printf("\nID\tDescription\t\t\tDue Date\n");
    printf("---------------------------------------------------\n");

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        printf("%d\t%s\t\t%s\n", sqlite3_column_int(stmt, 0),
                                  sqlite3_column_text(stmt, 1),
                                  sqlite3_column_text(stmt, 2));
    }

    sqlite3_finalize(stmt);
}

// View detailed information about a specific task
void view_task(sqlite3 *db, int id) {
    const char *sql = "SELECT description, detailed_description, due_date, is_completed FROM tasks WHERE id = ?;";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch task details: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Bind the task ID
    sqlite3_bind_int(stmt, 1, id);

    if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const char *description = (const char *)sqlite3_column_text(stmt, 0);
        const char *detailed_description = (const char *)sqlite3_column_text(stmt, 1);
        const char *due_date = (const char *)sqlite3_column_text(stmt, 2);
        int is_completed = sqlite3_column_int(stmt, 3);

        printf("\nTask ID: %d\n", id);
        printf("Description: %s\n", description);
        printf("Detailed Description: %s\n", detailed_description);
        printf("Due Date: %s\n", due_date);
        printf("Completed: %s\n", is_completed ? "Yes" : "No");
    } else {
        printf("Task with ID %d not found.\n", id);
    }

    sqlite3_finalize(stmt);
}

