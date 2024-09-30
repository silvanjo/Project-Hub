#include <iostream>
#include <vector>

class SegmentTree {
private:
    std::vector<int> tree;
    int n;

    // Build the tree recursively
    void build(const std::vector<int>& arr, int node, int start, int end) {
        if (start == end) {
            // Leaf node will have a single element
            tree[node] = arr[start];
        } else {
            int mid = (start + end) / 2;
            // Recurse on the left child
            build(arr, 2 * node, start, mid);
            // Recurse on the right child
            build(arr, 2 * node + 1, mid + 1, end);
            // Internal node will have the sum of both of its children
            tree[node] = tree[2 * node] + tree[2 * node + 1];
        }
    }

    // Query in range [l, r]
    int query(int node, int start, int end, int l, int r) {
        if (r < start || end < l) {
            // Range represented by a node is completely outside the given range
            return 0; // Identity value for sum
        }
        if (l <= start && end <= r) {
            // Range represented by a node is completely inside the given range
            return tree[node];
        }
        // Overlapping ranges
        int mid = (start + end) / 2;
        int sum_left = query(2 * node, start, mid, l, r);
        int sum_right = query(2 * node + 1, mid + 1, end, l, r);
        return sum_left + sum_right;
    }

    // Update the value at index idx to val
    void update(int node, int start, int end, int idx, int val) {
        if (start == end) {
            // Leaf node
            tree[node] = val;
        } else {
            int mid = (start + end) / 2;
            if (start <= idx && idx <= mid) {
                // If idx is in the left child
                update(2 * node, start, mid, idx, val);
            } else {
                // If idx is in the right child
                update(2 * node + 1, mid + 1, end, idx, val);
            }
            // Internal node will have the sum of both of its children
            tree[node] = tree[2 * node] + tree[2 * node + 1];
        }
    }

public:
    SegmentTree(const std::vector<int>& arr) {
        n = arr.size();
        // Allocate memory for the segment tree
        tree.resize(4 * n);
        build(arr, 1, 0, n - 1);
    }

    // Public query function
    int query(int l, int r) {
        return query(1, 0, n - 1, l, r);
    }

    // Public update function
    void update(int idx, int val) {
        update(1, 0, n - 1, idx, val);
    }
};

int main() {
    std::vector<int> arr = {1, 3, 5, 7, 9, 11};

    SegmentTree segTree(arr);

    std::cout << "Sum of values in range [1, 3]: " << segTree.query(1, 3) << std::endl;

    // Update index 1 to value 10
    segTree.update(1, 10);

    std::cout << "After update, sum of values in range [1, 3]: " << segTree.query(1, 3) << std::endl;

    return 0;
}

