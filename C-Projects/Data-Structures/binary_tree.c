#include <stdio.h>
#include <stdlib.h>

// Define the structure for a node in the binary tree
struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

// Function to create a new node
struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Function to insert a node in the binary tree
struct Node* insertNode(struct Node* root, int data) {
    if (root == NULL) {
        // If the tree is empty, create a new node
        return createNode(data);
    }

    // Recursively insert data in the left or right subtree
    if (data < root->data) {
        root->left = insertNode(root->left, data);
    } else {
        root->right = insertNode(root->right, data);
    }

    return root;
}

// In-order Traversal (Left, Root, Right)
void inorderTraversal(struct Node* root) {
    if (root == NULL) {
        return;
    }

    inorderTraversal(root->left);
    printf("%d ", root->data);
    inorderTraversal(root->right);
}

// Pre-order Traversal (Root, Left, Right)
void preorderTraversal(struct Node* root) {
    if (root == NULL) {
        return;
    }

    printf("%d ", root->data);
    preorderTraversal(root->left);
    preorderTraversal(root->right);
}

// Post-order Traversal (Left, Right, Root)
void postorderTraversal(struct Node* root) {
    if (root == NULL) {
        return;
    }

    postorderTraversal(root->left);
    postorderTraversal(root->right);
    printf("%d ", root->data);
}

// Function to search for a value in the binary tree
struct Node* search(struct Node* root, int data) {
    if (root == NULL || root->data == data) {
        return root;
    }

    if (data < root->data) {
        return search(root->left, data);
    }

    return search(root->right, data);
}

// Example usage
int main() {
    struct Node* root = NULL;

    // Insert nodes into the binary tree
    root = insertNode(root, 50);
    insertNode(root, 30);
    insertNode(root, 20);
    insertNode(root, 40);
    insertNode(root, 70);
    insertNode(root, 60);
    insertNode(root, 80);

    // Perform in-order traversal
    printf("In-order traversal: ");
    inorderTraversal(root);
    printf("\n");

    // Perform pre-order traversal
    printf("Pre-order traversal: ");
    preorderTraversal(root);
    printf("\n");

    // Perform post-order traversal
    printf("Post-order traversal: ");
    postorderTraversal(root);
    printf("\n");

    // Search for a value in the binary tree
    int key = 40;
    struct Node* result = search(root, key);
    if (result != NULL) {
        printf("Node with value %d found in the tree.\n", key);
    } else {
        printf("Node with value %d not found in the tree.\n", key);
    }

    return 0;
}

