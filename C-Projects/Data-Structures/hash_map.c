#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 10

// Define the structure for a hash map node (key-value pair)
struct HashNode {
    char* key;
    int value;
    struct HashNode* next;
};

// Define the structure for the hash map
struct HashMap {
    struct HashNode* table[TABLE_SIZE];
};

// Function to create a new hash node
struct HashNode* createNode(const char* key, int value) {
    struct HashNode* newNode = (struct HashNode*)malloc(sizeof(struct HashNode));
    newNode->key = strdup(key);  // Duplicate the key string
    newNode->value = value;
    newNode->next = NULL;
    return newNode;
}

// Hash function to generate an index for the given key
unsigned int hashFunction(const char* key) {
    unsigned long int hash = 0;
    int i = 0;

    // Simple hash function that adds the ASCII values of the characters in the key
    while (key[i] != '\0') {
        hash = hash + key[i];
        i++;
    }

    return hash % TABLE_SIZE;
}

// Function to initialize the hash map
struct HashMap* createHashMap() {
    struct HashMap* map = (struct HashMap*)malloc(sizeof(struct HashMap));
    for (int i = 0; i < TABLE_SIZE; i++) {
        map->table[i] = NULL;
    }
    return map;
}

// Function to insert a key-value pair into the hash map
void insert(struct HashMap* map, const char* key, int value) {
    unsigned int index = hashFunction(key);
    struct HashNode* newNode = createNode(key, value);
    struct HashNode* head = map->table[index];

    // Check if the key already exists in the chain
    struct HashNode* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->key, key) == 0) {
            // If the key already exists, update the value
            temp->value = value;
            free(newNode->key);
            free(newNode);
            return;
        }
        temp = temp->next;
    }

    // Insert the new node at the beginning of the list (chaining)
    newNode->next = head;
    map->table[index] = newNode;
}

// Function to search for a value by key in the hash map
int search(struct HashMap* map, const char* key) {
    unsigned int index = hashFunction(key);
    struct HashNode* head = map->table[index];

    struct HashNode* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->key, key) == 0) {
            return temp->value;
        }
        temp = temp->next;
    }

    // Return -1 if the key is not found
    return -1;
}

// Function to remove a key-value pair from the hash map
void delete(struct HashMap* map, const char* key) {
    unsigned int index = hashFunction(key);
    struct HashNode* head = map->table[index];
    struct HashNode* temp = head;
    struct HashNode* prev = NULL;

    while (temp != NULL) {
        if (strcmp(temp->key, key) == 0) {
            if (prev == NULL) {
                map->table[index] = temp->next;  // Remove the head node
            } else {
                prev->next = temp->next;  // Bypass the current node
            }
            free(temp->key);
            free(temp);
            printf("Key '%s' deleted from the hash map.\n", key);
            return;
        }
        prev = temp;
        temp = temp->next;
    }

    printf("Key '%s' not found in the hash map.\n", key);
}

// Function to print the entire hash map
void printHashMap(struct HashMap* map) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        struct HashNode* temp = map->table[i];
        printf("Index %d: ", i);
        while (temp != NULL) {
            printf("(%s -> %d) ", temp->key, temp->value);
            temp = temp->next;
        }
        printf("\n");
    }
}

// Example usage
int main() {
    struct HashMap* map = createHashMap();

    insert(map, "apple", 100);
    insert(map, "banana", 200);
    insert(map, "cherry", 300);
    insert(map, "date", 400);
    insert(map, "elderberry", 500);

    printf("Value associated with 'banana': %d\n", search(map, "banana"));
    printf("Value associated with 'apple': %d\n", search(map, "apple"));

    printHashMap(map);

    delete(map, "banana");
    delete(map, "fig");  // Attempting to delete a non-existent key

    printHashMap(map);

    return 0;
}

