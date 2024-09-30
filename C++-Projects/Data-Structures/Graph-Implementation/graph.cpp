#include <iostream>
#include <vector>
#include <queue>
#include <stack>

class Graph {
private:
    int numVertices;                            // Number of vertices in the graph
    std::vector<std::vector<int>> adjLists;     // Adjacency lists

public:
    // Constructor
    Graph(int vertices) : numVertices(vertices) {
        adjLists.resize(vertices);
    }

    // Function to add an edge to the graph (undirected by default)
    void addEdge(int src, int dest) {
        adjLists[src].push_back(dest);
        adjLists[dest].push_back(src); // Comment this line for a directed graph
    }

    // Breadth-First Search (BFS)
    void BFS(int startVertex) {
        std::vector<bool> visited(numVertices, false);
        std::queue<int> queue;

        visited[startVertex] = true;
        queue.push(startVertex);

        std::cout << "BFS Traversal starting from vertex " << startVertex << ": ";

        while (!queue.empty()) {
            int currentVertex = queue.front();
            queue.pop();
            std::cout << currentVertex << " ";

            for (int neighbor : adjLists[currentVertex]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    queue.push(neighbor);
                }
            }
        }
        std::cout << std::endl;
    }

    // Depth-First Search (DFS) using a stack
    void DFS(int startVertex) {
        std::vector<bool> visited(numVertices, false);
        std::stack<int> stack;

        stack.push(startVertex);

        std::cout << "DFS Traversal starting from vertex " << startVertex << ": ";

        while (!stack.empty()) {
            int currentVertex = stack.top();
            stack.pop();

            if (!visited[currentVertex]) {
                visited[currentVertex] = true;
                std::cout << currentVertex << " ";

                // Push all unvisited neighbors onto the stack
                // Reverse iterate to maintain order similar to recursive DFS
                for (auto it = adjLists[currentVertex].rbegin(); it != adjLists[currentVertex].rend(); ++it) {
                    if (!visited[*it]) {
                        stack.push(*it);
                    }
                }
            }
        }
        std::cout << std::endl;
    }
};

int main() {
    int vertices, edges;
    std::cout << "Enter the number of vertices: ";
    std::cin >> vertices;
    Graph graph(vertices);

    std::cout << "Enter the number of edges: ";
    std::cin >> edges;

    std::cout << "Enter edges (source destination):\n";
    for (int i = 0; i < edges; ++i) {
        int src, dest;
        std::cin >> src >> dest;
        graph.addEdge(src, dest);
    }

    int startVertex;
    std::cout << "Enter the starting vertex for BFS and DFS: ";
    std::cin >> startVertex;

    graph.BFS(startVertex);
    graph.DFS(startVertex);

    return 0;
}

