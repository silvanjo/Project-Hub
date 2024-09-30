#include <stdio.h>
#include <stdlib.h>

// Define the structure for an adjacency list node
struct AdjListNode {
    int dest;
    struct AdjListNode* next;
};

// Define the structure for an adjacency list
struct AdjList {
    struct AdjListNode* head;
};

// Define the structure for the graph
struct Graph {
    int numVertices;
    struct AdjList* array;
};

// Function to create a new adjacency list node
struct AdjListNode* createNode(int dest) {
    struct AdjListNode* newNode = (struct AdjListNode*)malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}

// Function to create a graph with a given number of vertices
struct Graph* createGraph(int numVertices) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->numVertices = numVertices;

    // Create an array of adjacency lists
    graph->array = (struct AdjList*)malloc(numVertices * sizeof(struct AdjList));

    // Initialize each adjacency list as empty
    for (int i = 0; i < numVertices; i++) {
        graph->array[i].head = NULL;
    }

    return graph;
}

// Function to add an edge to the graph (undirected graph)
void addEdge(struct Graph* graph, int src, int dest) {
    // Add an edge from src to dest
    struct AdjListNode* newNode = createNode(dest);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    // Since the graph is undirected, add an edge from dest to src
    newNode = createNode(src);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}

// Function to print the graph (adjacency list representation)
void printGraph(struct Graph* graph) {
    for (int v = 0; v < graph->numVertices; v++) {
        struct AdjListNode* temp = graph->array[v].head;
        printf("Vertex %d: ", v);
        while (temp) {
            printf("-> %d ", temp->dest);
            temp = temp->next;
        }
        printf("\n");
    }
}

// Helper function for DFS
void DFSUtil(struct Graph* graph, int v, int* visited) {
    visited[v] = 1;
    printf("%d ", v);

    struct AdjListNode* temp = graph->array[v].head;
    while (temp) {
        int connectedVertex = temp->dest;
        if (!visited[connectedVertex]) {
            DFSUtil(graph, connectedVertex, visited);
        }
        temp = temp->next;
    }
}

// Function to perform Depth-First Search (DFS)
void DFS(struct Graph* graph, int startVertex) {
    int* visited = (int*)malloc(graph->numVertices * sizeof(int));
    for (int i = 0; i < graph->numVertices; i++) {
        visited[i] = 0;
    }

    printf("DFS traversal starting from vertex %d: ", startVertex);
    DFSUtil(graph, startVertex, visited);
    printf("\n");

    free(visited);
}

// Function to perform Breadth-First Search (BFS)
void BFS(struct Graph* graph, int startVertex) {
    int* visited = (int*)malloc(graph->numVertices * sizeof(int));
    for (int i = 0; i < graph->numVertices; i++) {
        visited[i] = 0;
    }

    int* queue = (int*)malloc(graph->numVertices * sizeof(int));
    int front = 0, rear = 0;

    visited[startVertex] = 1;
    queue[rear++] = startVertex;

    printf("BFS traversal starting from vertex %d: ", startVertex);
    while (front != rear) {
        int currentVertex = queue[front++];
        printf("%d ", currentVertex);

        struct AdjListNode* temp = graph->array[currentVertex].head;
        while (temp) {
            int adjVertex = temp->dest;
            if (!visited[adjVertex]) {
                visited[adjVertex] = 1;
                queue[rear++] = adjVertex;
            }
            temp = temp->next;
        }
    }
    printf("\n");

    free(visited);
    free(queue);
}

// Example usage
int main() {
    int numVertices = 5;
    struct Graph* graph = createGraph(numVertices);

    addEdge(graph, 0, 1);
    addEdge(graph, 0, 4);
    addEdge(graph, 1, 2);
    addEdge(graph, 1, 3);
    addEdge(graph, 1, 4);
    addEdge(graph, 2, 3);
    addEdge(graph, 3, 4);

    printGraph(graph);

    DFS(graph, 0);  // Start DFS traversal from vertex 0
    BFS(graph, 0);  // Start BFS traversal from vertex 0

    return 0;
}

