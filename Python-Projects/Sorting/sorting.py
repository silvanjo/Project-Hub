import random
import copy

# Bubble Sort in Python
def bubble_sort(arr):
    n = len(arr)
    for i in range(n - 1):
        # Last i elements are already sorted
        for j in range(n - i - 1):
            if arr[j] > arr[j + 1]:
                # Swap if the element is greater than the next element
                arr[j], arr[j + 1] = arr[j + 1], arr[j]


# Selection Sort in Python
def selection_sort(arr):
    n = len(arr)
    for i in range(n):
        # Find the minimum element in unsorted array
        min_idx = i
        for j in range(i + 1, n):
            if arr[j] < arr[min_idx]:
                min_idx = j
        # Swap the found minimum element with the first element
        arr[i], arr[min_idx] = arr[min_idx], arr[i]

# Insertion Sort in Python
def insertion_sort(arr):
    for i in range(1, len(arr)):
        key = arr[i]
        j = i - 1
        # Move elements greater than key to one position ahead
        while j >= 0 and arr[j] > key:
            arr[j + 1] = arr[j]
            j -= 1
        arr[j + 1] = key

# Merge Sort in Python
def merge_sort(arr):
    if len(arr) > 1:
        # Find the middle point and divide it
        mid = len(arr) // 2
        L = arr[:mid]
        R = arr[mid:]
        # Sort the first half
        merge_sort(L)
        # Sort the second half
        merge_sort(R)
        i = j = k = 0
        # Copy data to temp arrays L[] and R[]
        while i < len(L) and j < len(R):
            if L[i] < R[j]:
                arr[k] = L[i]
                i += 1
            else:
                arr[k] = R[j]
                j += 1
            k += 1
        # Checking if any element was left
        while i < len(L):
            arr[k] = L[i]
            i += 1
            k += 1
        while j < len(R):
            arr[k] = R[j]
            j += 1
            k += 1

# Quick Sort in Python
def partition(arr, low, high):
    pivot = arr[high]  # Pivot element
    i = low - 1        # Index of smaller element
    for j in range(low, high):
        # If current element is smaller than or equal to pivot
        if arr[j] <= pivot:
            i += 1
            arr[i], arr[j] = arr[j], arr[i]
    arr[i + 1], arr[high] = arr[high], arr[i + 1]
    return i + 1

def quick_sort(arr, low, high):
    if low < high:
        # pi is partitioning index
        pi = partition(arr, low, high)
        # Separately sort elements before and after partition
        quick_sort(arr, low, pi - 1)
        quick_sort(arr, pi + 1, high)

# Heap Sort in Python
def heapify(arr, n, i):
    largest = i        # Initialize largest as root
    l = 2 * i + 1      # Left child
    r = 2 * i + 2      # Right child
    # See if left child exists and is greater than root
    if l < n and arr[l] > arr[largest]:
        largest = l
    # See if right child exists and is greater than root
    if r < n and arr[r] > arr[largest]:
        largest = r
    # Change root if needed
    if largest != i:
        arr[i], arr[largest] = arr[largest], arr[i]
        # Heapify the root
        heapify(arr, n, largest)

def heap_sort(arr):
    n = len(arr)
    # Build a maxheap
    for i in range(n // 2 - 1, -1, -1):
        heapify(arr, n, i)
    # Extract elements one by one
    for i in range(n - 1, 0, -1):
        # Move current root to end
        arr[i], arr[0] = arr[0], arr[i]
        # Call max heapify on the reduced heap
        heapify(arr, i, 0)

# Counting Sort in Python
def counting_sort(arr):
    # Find the maximum and minimum elements
    max_elem = max(arr)
    min_elem = min(arr)
    range_of_elements = max_elem - min_elem + 1
    # Create count array and initialize to zero
    count_arr = [0] * range_of_elements
    output_arr = [0] * len(arr)
    # Store count of each character
    for i in arr:
        count_arr[i - min_elem] += 1
    # Change count_arr[i] so that count_arr[i] contains position of this element in output_arr
    for i in range(1, len(count_arr)):
        count_arr[i] += count_arr[i - 1]
    # Build the output array
    for i in reversed(arr):
        output_arr[count_arr[i - min_elem] - 1] = i
        count_arr[i - min_elem] -= 1
    # Copy the output array to arr
    for i in range(len(arr)):
        arr[i] = output_arr[i]

# Radix Sort in Python
def counting_sort_for_radix(arr, exp):
    n = len(arr)
    output = [0] * n
    count = [0] * 10
    # Store count of occurrences
    for i in arr:
        index = i // exp
        count[index % 10] += 1
    # Change count[i] so that count[i] contains actual position of this digit in output[]
    for i in range(1, 10):
        count[i] += count[i - 1]
    # Build the output array
    for i in reversed(arr):
        index = i // exp
        output[count[index % 10] - 1] = i
        count[index % 10] -= 1
    # Copy the output array to arr
    for i in range(len(arr)):
        arr[i] = output[i]

def radix_sort(arr):
    # Find the maximum number to know the number of digits
    max_elem = max(arr)
    # Do counting sort for every digit
    exp = 1
    while max_elem // exp > 0:
        counting_sort_for_radix(arr, exp)
        exp *= 10

# Bucket Sort in Python
def bucket_sort(arr):
    if len(arr) == 0:
        return arr
    # Determine minimum and maximum values
    min_value = min(arr)
    max_value = max(arr)
    # Initialize buckets
    bucket_range = (max_value - min_value) / len(arr)
    buckets = [[] for _ in range(len(arr))]
    # Distribute input array values into buckets
    for i in arr:
        index = int((i - min_value) / bucket_range)
        if index == len(arr):
            index -= 1
        buckets[index].append(i)
    # Sort individual buckets and concatenate
    arr.clear()
    for bucket in buckets:
        insertion_sort(bucket)
        arr.extend(bucket)

# Shell Sort in Python
def shell_sort(arr):
    n = len(arr)
    gap = n // 2
    # Start with a big gap, then reduce the gap
    while gap > 0:
        for i in range(gap, n):
            temp = arr[i]
            j = i
            # Shift earlier gap-sorted elements up until the correct location for arr[i] is found
            while j >= gap and arr[j - gap] > temp:
                arr[j] = arr[j - gap]
                j -= gap
            # Put temp (the original arr[i]) in its correct location
            arr[j] = temp
        gap //= 2

def print_list(arr):
    print(' '.join(map(str, arr)))

def main():
    SIZE = 10
    original_list = [random.randint(1, 100) for _ in range(SIZE)]

    print("Demonstrating various sorting algorithms:\n")

    # Bubble Sort
    arr = copy.copy(original_list)
    print("Bubble Sort")
    print("Original list:", end=" ")
    print_list(arr)
    bubble_sort(arr)
    print("Sorted list:  ", end=" ")
    print_list(arr)
    print()

    # Selection Sort
    arr = copy.copy(original_list)
    print("Selection Sort")
    print("Original list:", end=" ")
    print_list(arr)
    selection_sort(arr)
    print("Sorted list:  ", end=" ")
    print_list(arr)
    print()

    # Insertion Sort
    arr = copy.copy(original_list)
    print("Insertion Sort")
    print("Original list:", end=" ")
    print_list(arr)
    insertion_sort(arr)
    print("Sorted list:  ", end=" ")
    print_list(arr)
    print()

    # Merge Sort
    arr = copy.copy(original_list)
    print("Merge Sort")
    print("Original list:", end=" ")
    print_list(arr)
    merge_sort(arr)
    print("Sorted list:  ", end=" ")
    print_list(arr)
    print()

    # Quick Sort
    arr = copy.copy(original_list)
    print("Quick Sort")
    print("Original list:", end=" ")
    print_list(arr)
    quick_sort(arr, 0, len(arr) - 1)
    print("Sorted list:  ", end=" ")
    print_list(arr)
    print()

    # Heap Sort
    arr = copy.copy(original_list)
    print("Heap Sort")
    print("Original list:", end=" ")
    print_list(arr)
    heap_sort(arr)
    print("Sorted list:  ", end=" ")
    print_list(arr)
    print()

    # Counting Sort
    arr = copy.copy(original_list)
    print("Counting Sort")
    print("Original list:", end=" ")
    print_list(arr)
    counting_sort(arr)
    print("Sorted list:  ", end=" ")
    print_list(arr)
    print()

    # Radix Sort
    arr = copy.copy(original_list)
    print("Radix Sort")
    print("Original list:", end=" ")
    print_list(arr)
    radix_sort(arr)
    print("Sorted list:  ", end=" ")
    print_list(arr)
    print()

    # Bucket Sort
    arr = [random.random() for _ in range(SIZE)]  # Generate random floats between 0 and 1
    print("Bucket Sort")
    print("Original list:", end=" ")
    print_list(arr)
    bucket_sort(arr)
    print("Sorted list:  ", end=" ")
    print_list(arr)
    print()

    # Shell Sort
    arr = copy.copy(original_list)
    print("Shell Sort")
    print("Original list:", end=" ")
    print_list(arr)
    shell_sort(arr)
    print("Sorted list:  ", end=" ")
    print_list(arr)
    print()

if __name__ == "__main__":
    main()