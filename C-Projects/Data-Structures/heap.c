#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100  // Define maximum size of the heap

// Structure representing a Max Heap
struct MaxHeap {
    int size;
    int array[MAX_SIZE];
};

// Function to create a new Max Heap
struct MaxHeap* createMaxHeap() {
    struct MaxHeap* heap = (struct MaxHeap*)malloc(sizeof(struct MaxHeap));
    heap->size = 0;
    return heap;
}

// Function to swap two elements
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Function to heapify a subtree with root at index i (for max heap)
void heapify(struct MaxHeap* heap, int i) {
    int largest = i;           // Initialize largest as root
    int left = 2 * i + 1;      // Left child
    int right = 2 * i + 2;     // Right child

    // If the left child is larger than the root
    if (left < heap->size && heap->array[left] > heap->array[largest]) {
        largest = left;
    }

    // If the right child is larger than the current largest
    if (right < heap->size && heap->array[right] > heap->array[largest]) {
        largest = right;
    }

    // If the largest element is not the root
    if (largest != i) {
        swap(&heap->array[i], &heap->array[largest]);

        // Recursively heapify the affected subtree
        heapify(heap, largest);
    }
}

// Function to insert a new key into the heap
void insert(struct MaxHeap* heap, int key) {
    if (heap->size == MAX_SIZE) {
        printf("Heap overflow!\n");
        return;
    }

    // Insert the new key at the end
    heap->size++;
    int i = heap->size - 1;
    heap->array[i] = key;

    // Fix the heap property if violated
    while (i != 0 && heap->array[(i - 1) / 2] < heap->array[i]) {
        swap(&heap->array[(i - 1) / 2], &heap->array[i]);
        i = (i - 1) / 2;
    }
}

// Function to extract (remove) the maximum element from the heap
int extractMax(struct MaxHeap* heap) {
    if (heap->size <= 0) {
        printf("Heap underflow!\n");
        return -1;
    }
    if (heap->size == 1) {
        heap->size--;
        return heap->array[0];
    }

    // Store the maximum value and remove it from the heap
    int root = heap->array[0];
    heap->array[0] = heap->array[heap->size - 1];
    heap->size--;

    // Heapify the root to maintain the max heap property
    heapify(heap, 0);

    return root;
}

// Function to print the heap
void printHeap(struct MaxHeap* heap) {
    printf("Max Heap: ");
    for (int i = 0; i < heap->size; i++) {
        printf("%d ", heap->array[i]);
    }
    printf("\n");
}

// Example usage
int main() {
    struct MaxHeap* heap = createMaxHeap();

    // Insert elements into the heap
    insert(heap, 3);
    insert(heap, 5);
    insert(heap, 9);
    insert(heap, 6);
    insert(heap, 8);
    insert(heap, 20);

    // Print the heap
    printHeap(heap);

    // Extract the maximum element
    printf("Extracted max: %d\n", extractMax(heap));

    // Print the heap after extracting the max element
    printHeap(heap);

    // Extract another maximum element
    printf("Extracted max: %d\n", extractMax(heap));

    // Print the heap again
    printHeap(heap);

    return 0;
}

