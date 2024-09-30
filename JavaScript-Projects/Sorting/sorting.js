// Bubble Sort in JavaScript
function bubbleSort(arr) {
    let n = arr.length;
    for (let i = 0; i < n - 1; i++) {
        // Last i elements are already sorted
        for (let j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                // Swap if the element is greater than the next element
                [arr[j], arr[j + 1]] = [arr[j + 1], arr[j]];
            }
        }
    }
}

// Selection Sort in JavaScript
function selectionSort(arr) {
    let n = arr.length;
    for (let i = 0; i < n - 1; i++) {
        // Find the minimum element in unsorted array
        let min_idx = i;
        for (let j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        // Swap the found minimum element with the first element
        [arr[i], arr[min_idx]] = [arr[min_idx], arr[i]];
    }
}

// Insertion Sort in JavaScript
function insertionSort(arr) {
    let n = arr.length;
    for (let i = 1; i < n; i++) {
        let key = arr[i];
        let j = i - 1;
        // Move elements greater than key to one position ahead
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Merge Sort in JavaScript
function merge(left, right) {
    let result = [];
    let i = 0, j = 0;
    // Merge the two arrays while comparing elements
    while (i < left.length && j < right.length) {
        if (left[i] < right[j]) {
            result.push(left[i]);
            i++;
        } else {
            result.push(right[j]);
            j++;
        }
    }
    // Concatenate the remaining elements
    return result.concat(left.slice(i)).concat(right.slice(j));
}

function mergeSort(arr) {
    if (arr.length <= 1) {
        return arr;
    }
    // Split the array into halves
    let mid = Math.floor(arr.length / 2);
    let left = arr.slice(0, mid);
    let right = arr.slice(mid);
    // Recursively sort both halves and merge them
    return merge(mergeSort(left), mergeSort(right));
}

// Quick Sort in JavaScript
function partition(arr, low, high) {
    let pivot = arr[high]; // Pivot element
    let i = low - 1;       // Index of smaller element
    for (let j = low; j < high; j++) {
        // If current element is smaller than or equal to pivot
        if (arr[j] <= pivot) {
            i++;
            [arr[i], arr[j]] = [arr[j], arr[i]];
        }
    }
    // Swap the pivot element with the element at i + 1
    [arr[i + 1], arr[high]] = [arr[high], arr[i + 1]];
    return i + 1;
}

function quickSort(arr, low = 0, high = arr.length - 1) {
    if (low < high) {
        // Partitioning index
        let pi = partition(arr, low, high);
        // Recursively sort elements before and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Heap Sort in JavaScript
function heapify(arr, n, i) {
    let largest = i;       // Initialize largest as root
    let l = 2 * i + 1;     // Left child
    let r = 2 * i + 2;     // Right child
    // If left child is larger than root
    if (l < n && arr[l] > arr[largest]) {
        largest = l;
    }
    // If right child is larger than largest so far
    if (r < n && arr[r] > arr[largest]) {
        largest = r;
    }
    // If largest is not root
    if (largest !== i) {
        [arr[i], arr[largest]] = [arr[largest], arr[i]];
        // Recursively heapify the affected sub-tree
        heapify(arr, n, largest);
    }
}

function heapSort(arr) {
    let n = arr.length;
    // Build a maxheap
    for (let i = Math.floor(n / 2) - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
    // Extract elements one by one
    for (let i = n - 1; i > 0; i--) {
        // Move current root to end
        [arr[0], arr[i]] = [arr[i], arr[0]];
        // Call max heapify on the reduced heap
        heapify(arr, i, 0);
    }
}

// Counting Sort in JavaScript
function countingSort(arr) {
    let min = Math.min(...arr);
    let max = Math.max(...arr);
    let range = max - min + 1;
    let count = Array(range).fill(0);
    let output = Array(arr.length).fill(0);
    // Store count of each element
    for (let i = 0; i < arr.length; i++) {
        count[arr[i] - min]++;
    }
    // Change count[i] so that count[i] contains position of this element in output array
    for (let i = 1; i < count.length; i++) {
        count[i] += count[i - 1];
    }
    // Build the output array
    for (let i = arr.length - 1; i >= 0; i--) {
        output[count[arr[i] - min] - 1] = arr[i];
        count[arr[i] - min]--;
    }
    // Copy the output array to arr
    for (let i = 0; i < arr.length; i++) {
        arr[i] = output[i];
    }
}

// Radix Sort in JavaScript
function countingSortForRadix(arr, exp) {
    let n = arr.length;
    let output = Array(n).fill(0);
    let count = Array(10).fill(0);
    // Store count of occurrences
    for (let i = 0; i < n; i++) {
        let index = Math.floor(arr[i] / exp) % 10;
        count[index]++;
    }
    // Change count[i] so that count[i] contains actual position of this digit in output[]
    for (let i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }
    // Build the output array
    for (let i = n - 1; i >= 0; i--) {
        let index = Math.floor(arr[i] / exp) % 10;
        output[count[index] - 1] = arr[i];
        count[index]--;
    }
    // Copy the output array to arr[]
    for (let i = 0; i < n; i++) {
        arr[i] = output[i];
    }
}

function radixSort(arr) {
    // Find the maximum number to know the number of digits
    let max = Math.max(...arr);
    // Do counting sort for every digit
    for (let exp = 1; Math.floor(max / exp) > 0; exp *= 10) {
        countingSortForRadix(arr, exp);
    }
}

// Bucket Sort in JavaScript
function bucketSort(arr) {
    if (arr.length === 0) {
        return arr;
    }
    // Determine minimum and maximum values
    let minValue = Math.min(...arr);
    let maxValue = Math.max(...arr);
    // Initialize buckets
    let bucketSize = 5; // You can set this value based on your input
    let bucketCount = Math.floor((maxValue - minValue) / bucketSize) + 1;
    let buckets = Array.from({ length: bucketCount }, () => []);
    // Distribute input array values into buckets
    for (let i = 0; i < arr.length; i++) {
        let bucketIndex = Math.floor((arr[i] - minValue) / bucketSize);
        buckets[bucketIndex].push(arr[i]);
    }
    // Sort individual buckets and concatenate
    arr.length = 0;
    for (let i = 0; i < buckets.length; i++) {
        insertionSort(buckets[i]); // Using insertion sort for simplicity
        arr.push(...buckets[i]);
    }
}

// Helper insertion sort function for bucket sort
function insertionSort(arr) {
    for (let i = 1; i < arr.length; i++) {
        let key = arr[i];
        let j = i - 1;
        // Move elements greater than key to one position ahead
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Shell Sort in JavaScript
function shellSort(arr) {
    let n = arr.length;
    // Start with a big gap, then reduce the gap
    for (let gap = Math.floor(n / 2); gap > 0; gap = Math.floor(gap / 2)) {
        // Do a gapped insertion sort for this gap size
        for (let i = gap; i < n; i++) {
            let temp = arr[i];
            let j = i;
            // Shift earlier gap-sorted elements up until the correct location for arr[i] is found
            while (j >= gap && arr[j - gap] > temp) {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            // Put temp (the original arr[i]) in its correct location
            arr[j] = temp;
        }
    }
}

function printArray(arr) {
    console.log(arr.join(' '));
}

function generateRandomArray(size, min, max) {
    return Array.from({ length: size }, () => Math.floor(Math.random() * (max - min + 1)) + min);
}

function runSortingDemo() {
    const SIZE = 10;
    const MIN = 1;
    const MAX = 100;

    console.log("Demonstrating various sorting algorithms:\n");

    // Bubble Sort
    let arr = generateRandomArray(SIZE, MIN, MAX);
    console.log("Bubble Sort");
    console.log("Original array:");
    printArray(arr);
    bubbleSort(arr);
    console.log("Sorted array:");
    printArray(arr);
    console.log();

    // Selection Sort
    arr = generateRandomArray(SIZE, MIN, MAX);
    console.log("Selection Sort");
    console.log("Original array:");
    printArray(arr);
    selectionSort(arr);
    console.log("Sorted array:");
    printArray(arr);
    console.log();

    // Insertion Sort
    arr = generateRandomArray(SIZE, MIN, MAX);
    console.log("Insertion Sort");
    console.log("Original array:");
    printArray(arr);
    insertionSort(arr);
    console.log("Sorted array:");
    printArray(arr);
    console.log();

    // Merge Sort
    arr = generateRandomArray(SIZE, MIN, MAX);
    console.log("Merge Sort");
    console.log("Original array:");
    printArray(arr);
    arr = mergeSort(arr);  // Note: mergeSort returns a new sorted array
    console.log("Sorted array:");
    printArray(arr);
    console.log();

    // Quick Sort
    arr = generateRandomArray(SIZE, MIN, MAX);
    console.log("Quick Sort");
    console.log("Original array:");
    printArray(arr);
    quickSort(arr);
    console.log("Sorted array:");
    printArray(arr);
    console.log();

    // Heap Sort
    arr = generateRandomArray(SIZE, MIN, MAX);
    console.log("Heap Sort");
    console.log("Original array:");
    printArray(arr);
    heapSort(arr);
    console.log("Sorted array:");
    printArray(arr);
    console.log();

    // Counting Sort
    arr = generateRandomArray(SIZE, MIN, MAX);
    console.log("Counting Sort");
    console.log("Original array:");
    printArray(arr);
    countingSort(arr);
    console.log("Sorted array:");
    printArray(arr);
    console.log();

    // Radix Sort
    arr = generateRandomArray(SIZE, MIN, MAX);
    console.log("Radix Sort");
    console.log("Original array:");
    printArray(arr);
    radixSort(arr);
    console.log("Sorted array:");
    printArray(arr);
    console.log();

    // Bucket Sort
    arr = Array.from({ length: SIZE }, () => Math.random());  // Generate random floats between 0 and 1
    console.log("Bucket Sort");
    console.log("Original array:");
    printArray(arr);
    bucketSort(arr);
    console.log("Sorted array:");
    printArray(arr);
    console.log();

    // Shell Sort
    arr = generateRandomArray(SIZE, MIN, MAX);
    console.log("Shell Sort");
    console.log("Original array:");
    printArray(arr);
    shellSort(arr);
    console.log("Sorted array:");
    printArray(arr);
    console.log();
}

runSortingDemo();