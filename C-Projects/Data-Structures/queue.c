#include <stdio.h>
#include <stdlib.h>

// Define the node structure for the queue
struct QueueNode {
    int data;
    struct QueueNode* next;
};

// Define the queue structure
struct Queue {
    struct QueueNode* front;
    struct QueueNode* rear;
};

// Function to create a new node for the queue
struct QueueNode* createQueueNode(int data) {
    struct QueueNode* newNode = (struct QueueNode*)malloc(sizeof(struct QueueNode));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// Function to create an empty queue
struct Queue* createQueue() {
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    queue->front = queue->rear = NULL;
    return queue;
}

// Function to add an item to the queue
void enqueue(struct Queue* queue, int data) {
    struct QueueNode* newNode = createQueueNode(data);
    if (queue->rear == NULL) {
        queue->front = queue->rear = newNode;
        return;
    }
    queue->rear->next = newNode;
    queue->rear = newNode;
    printf("%d enqueued to queue\n", data);
}

// Function to remove an item from the queue
int dequeue(struct Queue* queue) {
    if (queue->front == NULL)
        return -1;
    struct QueueNode* temp = queue->front;
    queue->front = queue->front->next;
    if (queue->front == NULL)
        queue->rear = NULL;
    int dequeued = temp->data;
    free(temp);
    return dequeued;
}

// Example usage
int main() {
    struct Queue* queue = createQueue();

    enqueue(queue, 10);
    enqueue(queue, 20);
    enqueue(queue, 30);

    printf("Dequeued from queue: %d\n", dequeue(queue));
    printf("Dequeued from queue: %d\n", dequeue(queue));

    return 0;
}

