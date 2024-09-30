#include <stdio.h>
#include <stdlib.h>

// Define the node structure for the stack
struct StackNode {
    int data;
    struct StackNode* next;
};

// Function to create a new stack node
struct StackNode* createStackNode(int data) {
    struct StackNode* newNode = (struct StackNode*)malloc(sizeof(struct StackNode));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// Function to check if the stack is empty
int isStackEmpty(struct StackNode* root) {
    return !root;
}

// Function to push an item to the stack
void push(struct StackNode** root, int data) {
    struct StackNode* newNode = createStackNode(data);
    newNode->next = *root;
    *root = newNode;
    printf("%d pushed to stack\n", data);
}

// Function to pop an item from the stack
int pop(struct StackNode** root) {
    if (isStackEmpty(*root))
        return -1;
    struct StackNode* temp = *root;
    *root = (*root)->next;
    int popped = temp->data;
    free(temp);
    return popped;
}

// Function to peek at the top item of the stack
int peek(struct StackNode* root) {
    if (isStackEmpty(root))
        return -1;
    return root->data;
}

// Example usage
int main() {
    struct StackNode* stack = NULL;

    push(&stack, 10);
    push(&stack, 20);
    push(&stack, 30);

    printf("Top of stack: %d\n", peek(stack));
    printf("Popped from stack: %d\n", pop(&stack));
    printf("Top of stack after pop: %d\n", peek(stack));

    return 0;
}

