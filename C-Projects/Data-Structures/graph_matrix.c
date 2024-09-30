#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 5

// Function to initialize the adjacency matrix with zeros (no edges)
void initMatrix(int matrix[MAX_VERTICES][MAX_VERTICES]) {
    for (int i = 0; i < MAX_VERTICES; i++) {
        for (int j = 0; j < MAX_VERTICES; j++) {
            matrix[i][j] = 0;
        }
    }
}

// Function to add an edge to the adjacency matrix
void addEdge(int matrix[MAX_VERTICES][MAX_VERTICES], int src, int dest) {
    matrix[src][dest] = 1;
    matrix[dest][src] = 1;  // If it's an undirected graph
}

// Function to print the adjacency matrix
void printMatrix(int matrix[MAX_VERTICES][MAX_VERTICES]) {
    for (int i = 0; i < MAX_VERTICES; i++) {
        for (int j = 0; j < MAX_VERTICES; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int matrix[MAX_VERTICES][MAX_VERTICES];
    
    initMatrix(matrix);
    
    addEdge(matrix, 0, 1);
    addEdge(matrix, 0, 4);
    addEdge(matrix, 1, 2);
    addEdge(matrix, 1, 3);
    addEdge(matrix, 1, 4);
    addEdge(matrix, 2, 3);
    addEdge(matrix, 3, 4);

    printf("Adjacency Matrix:\n");
    printMatrix(matrix);

    return 0;
}

