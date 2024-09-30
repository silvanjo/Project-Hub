#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>
#include <cstdlib>

// Bubble Sort in C++
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; ++i) {
        // Last i elements are already in place
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                // Swap adjacent elements if they are in decreasing order
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Selection Sort in C++
void selectionSort(int arr[], int n) {
    for (int i = 0; i < n - 1; ++i) {
        // Find the minimum element in unsorted array
        int min_idx = i;
        for (int j = i + 1; j < n; ++j)
            if (arr[j] < arr[min_idx])
                min_idx = j;
        // Swap the found minimum element with the first element
        std::swap(arr[min_idx], arr[i]);
    }
}

// Insertion Sort in C++
void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;
        // Move elements greater than key to one position ahead
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

// Merge Sort in C++
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    // Temporary arrays
    int* L = new int[n1];
    int* R = new int[n2];
    // Copy data to temp arrays L[] and R[]
    for (int i = 0; i < n1; ++i)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[m + 1 + j];
    // Merge the temp arrays back into arr[l..r]
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }
    // Copy remaining elements of L[], if any
    while (i < n1)
        arr[k++] = L[i++];
    // Copy remaining elements of R[], if any
    while (j < n2)
        arr[k++] = R[j++];
    delete[] L;
    delete[] R;
}

void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        // Avoid overflow for large l and h
        int m = l + (r - l) / 2;
        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// Quick Sort in C++
int partition(int arr[], int low, int high) {
    int pivot = arr[high]; // Pivot element
    int i = low - 1;       // Index of smaller element
    for (int j = low; j <= high - 1; ++j) {
        // If current element is smaller than pivot
        if (arr[j] < pivot) {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        // Partitioning index
        int pi = partition(arr, low, high);
        // Recursively sort elements before and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Heap Sort in C++
void heapify(int arr[], int n, int i) {
    int largest = i;      // Initialize largest as root
    int l = 2 * i + 1;    // Left child
    int r = 2 * i + 2;    // Right child
    // If left child is larger than root
    if (l < n && arr[l] > arr[largest])
        largest = l;
    // If right child is larger than largest so far
    if (r < n && arr[r] > arr[largest])
        largest = r;
    // If largest is not root
    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        // Recursively heapify the affected sub-tree
        heapify(arr, n, largest);
    }
}

void heapSort(int arr[], int n) {
    // Build max heap
    for (int i = n / 2 - 1; i >= 0; --i)
        heapify(arr, n, i);
    // Extract elements from heap one by one
    for (int i = n - 1; i >= 0; --i) {
        // Move current root to end
        std::swap(arr[0], arr[i]);
        // Call max heapify on the reduced heap
        heapify(arr, i, 0);
    }
}

// Counting Sort in C++
void countingSort(int arr[], int n) {
    // Find the maximum element
    int max = *std::max_element(arr, arr + n);
    // Initialize count array
    int* count = new int[max + 1]{0};
    // Store the count of each element
    for (int i = 0; i < n; ++i)
        ++count[arr[i]];
    // Modify count array by adding previous counts
    for (int i = 1; i <= max; ++i)
        count[i] += count[i - 1];
    // Build the output array
    int* output = new int[n];
    for (int i = n - 1; i >= 0; --i) {
        output[count[arr[i]] - 1] = arr[i];
        --count[arr[i]];
    }
    // Copy the output array to arr[]
    for (int i = 0; i < n; ++i)
        arr[i] = output[i];
    delete[] count;
    delete[] output;
}

// Radix Sort in C++
int getMax(int arr[], int n) {
    return *std::max_element(arr, arr + n);
}

void countingSortForRadix(int arr[], int n, int exp) {
    int* output = new int[n];
    int count[10] = {0};
    // Store count of occurrences
    for (int i = 0; i < n; ++i)
        ++count[(arr[i] / exp) % 10];
    // Modify count array
    for (int i = 1; i < 10; ++i)
        count[i] += count[i - 1];
    // Build the output array
    for (int i = n - 1; i >= 0; --i) {
        int idx = (arr[i] / exp) % 10;
        output[count[idx] - 1] = arr[i];
        --count[idx];
    }
    // Copy output array to arr[]
    for (int i = 0; i < n; ++i)
        arr[i] = output[i];
    delete[] output;
}

void radixSort(int arr[], int n) {
    int m = getMax(arr, n);
    // Apply counting sort for every digit
    for (int exp = 1; m / exp > 0; exp *= 10)
        countingSortForRadix(arr, n, exp);
}

// Bucket Sort in C++
void bucketSort(float arr[], int n) {
    // Create n empty buckets
    std::vector<float> buckets[n];
    // Put array elements in different buckets
    for (int i = 0; i < n; ++i) {
        int idx = n * arr[i]; // Index in buckets
        buckets[idx].push_back(arr[i]);
    }
    // Sort individual buckets
    for (int i = 0; i < n; ++i)
        std::sort(buckets[i].begin(), buckets[i].end());
    // Concatenate all buckets into arr[]
    int index = 0;
    for (int i = 0; i < n; ++i)
        for (float val : buckets[i])
            arr[index++] = val;
}

// Shell Sort in C++
void shellSort(int arr[], int n) {
    // Start with a big gap, reduce the gap
    for (int gap = n / 2; gap > 0; gap /= 2) {
        // Perform a gapped insertion sort
        for (int i = gap; i < n; ++i) {
            int temp = arr[i];
            int j;
            // Shift elements of arr[0..i-gap] that are greater than temp
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap)
                arr[j] = arr[j - gap];
            // Put temp in its correct location
            arr[j] = temp;
        }
    }
}

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++)
        std::cout << arr[i] << " ";
    std::cout << std::endl;
}

int main() {
    const int SIZE = 10;
    int arr[SIZE];

    // Seed for random number generation
    std::srand(std::time(nullptr));

    std::cout << "Demonstrating various sorting algorithms:\n\n";

    // Bubble Sort
    for (int i = 0; i < SIZE; i++) arr[i] = std::rand() % 100;
    std::cout << "Bubble Sort\nOriginal array: ";
    printArray(arr, SIZE);
    bubbleSort(arr, SIZE);
    std::cout << "Sorted array:   ";
    printArray(arr, SIZE);
    std::cout << std::endl;

    // Selection Sort
    for (int i = 0; i < SIZE; i++) arr[i] = std::rand() % 100;
    std::cout << "Selection Sort\nOriginal array: ";
    printArray(arr, SIZE);
    selectionSort(arr, SIZE);
    std::cout << "Sorted array:   ";
    printArray(arr, SIZE);
    std::cout << std::endl;

    // Insertion Sort
    for (int i = 0; i < SIZE; i++) arr[i] = std::rand() % 100;
    std::cout << "Insertion Sort\nOriginal array: ";
    printArray(arr, SIZE);
    insertionSort(arr, SIZE);
    std::cout << "Sorted array:   ";
    printArray(arr, SIZE);
    std::cout << std::endl;

    // Merge Sort
    for (int i = 0; i < SIZE; i++) arr[i] = std::rand() % 100;
    std::cout << "Merge Sort\nOriginal array: ";
    printArray(arr, SIZE);
    mergeSort(arr, 0, SIZE - 1);
    std::cout << "Sorted array:   ";
    printArray(arr, SIZE);
    std::cout << std::endl;

    // Quick Sort
    for (int i = 0; i < SIZE; i++) arr[i] = std::rand() % 100;
    std::cout << "Quick Sort\nOriginal array: ";
    printArray(arr, SIZE);
    quickSort(arr, 0, SIZE - 1);
    std::cout << "Sorted array:   ";
    printArray(arr, SIZE);
    std::cout << std::endl;

    // Heap Sort
    for (int i = 0; i < SIZE; i++) arr[i] = std::rand() % 100;
    std::cout << "Heap Sort\nOriginal array: ";
    printArray(arr, SIZE);
    heapSort(arr, SIZE);
    std::cout << "Sorted array:   ";
    printArray(arr, SIZE);
    std::cout << std::endl;

    // Counting Sort
    for (int i = 0; i < SIZE; i++) arr[i] = std::rand() % 100;
    std::cout << "Counting Sort\nOriginal array: ";
    printArray(arr, SIZE);
    countingSort(arr, SIZE);
    std::cout << "Sorted array:   ";
    printArray(arr, SIZE);
    std::cout << std::endl;

    // Radix Sort
    for (int i = 0; i < SIZE; i++) arr[i] = std::rand() % 100;
    std::cout << "Radix Sort\nOriginal array: ";
    printArray(arr, SIZE);
    radixSort(arr, SIZE);
    std::cout << "Sorted array:   ";
    printArray(arr, SIZE);
    std::cout << std::endl;

    // Bucket Sort (for float values)
    float floatArr[SIZE];
    for (int i = 0; i < SIZE; i++) floatArr[i] = static_cast<float>(std::rand()) / RAND_MAX;
    std::cout << "Bucket Sort\nOriginal array: ";
    for (int i = 0; i < SIZE; i++) std::cout << floatArr[i] << " ";
    std::cout << std::endl;
    bucketSort(floatArr, SIZE);
    std::cout << "Sorted array:   ";
    for (int i = 0; i < SIZE; i++) std::cout << floatArr[i] << " ";
    std::cout << std::endl << std::endl;

    // Shell Sort
    for (int i = 0; i < SIZE; i++) arr[i] = std::rand() % 100;
    std::cout << "Shell Sort\nOriginal array: ";
    printArray(arr, SIZE);
    shellSort(arr, SIZE);
    std::cout << "Sorted array:   ";
    printArray(arr, SIZE);
    std::cout << std::endl;

    return 0;
}
