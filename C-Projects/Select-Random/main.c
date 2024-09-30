#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <time.h>

#define DATABASE "elements.db"

// Function declarations
void initialize_db();
void add_element(sqlite3 *db, const char *element);
void delete_element(sqlite3 *db, const char *element);
void list_elements(sqlite3 *db);
void pick_random_element(sqlite3 *db);

int main() {
    sqlite3 *db;
    int rc;
    char option;
    char element[100];

    // Open database
    rc = sqlite3_open(DATABASE, &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }
    
    initialize_db(db);

    while (1) {
        printf("\nOptions:\n");
        printf("1. Add element\n");
        printf("2. Delete element\n");
        printf("3. List elements\n");
        printf("4. Pick random element\n");
        printf("5. Exit\n");
        printf("Choose an option: ");
        scanf(" %c", &option);

        switch (option) {
            case '1':
                printf("Enter element to add: ");
                scanf("%s", element);
                add_element(db, element);
                break;
            case '2':
                printf("Enter element to delete: ");
                scanf("%s", element);
                delete_element(db, element);
                break;
            case '3':
                list_elements(db);
                break;
            case '4':
                pick_random_element(db);
                break;
            case '5':
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
    const char *sql = "CREATE TABLE IF NOT EXISTS elements (id INTEGER PRIMARY KEY, name TEXT UNIQUE);";
    
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("Database initialized successfully.\n");
    }
}

// Add an element to the database
void add_element(sqlite3 *db, const char *element) {
    char *err_msg = 0;
    char sql[256];
    
    sprintf(sql, "INSERT INTO elements (name) VALUES ('%s');", element);
    
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("Element added successfully.\n");
    }
}

// Delete an element from the database
void delete_element(sqlite3 *db, const char *element) {
    char *err_msg = 0;
    char sql[256];
    
    sprintf(sql, "DELETE FROM elements WHERE name = '%s';", element);
    
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("Element deleted successfully.\n");
    }
}

// List all elements in the database
void list_elements(sqlite3 *db) {
    const char *sql = "SELECT name FROM elements;";
    sqlite3_stmt *stmt;
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch elements: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    printf("\nElements:\n");
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        printf("%s\n", sqlite3_column_text(stmt, 0));
    }
    
    sqlite3_finalize(stmt);
}

// Pick a random element from the database
void pick_random_element(sqlite3 *db) {
    const char *sql = "SELECT name FROM elements ORDER BY RANDOM() LIMIT 1;";
    sqlite3_stmt *stmt;
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch random element: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        printf("Random element: %s\n", sqlite3_column_text(stmt, 0));
    } else {
        printf("No elements in the database.\n");
    }
    
    sqlite3_finalize(stmt);
}

