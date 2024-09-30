#include <iostream>
#include <vector>

class FenwickTree {
private:
    std::vector<int> tree;
    int n;

    // Internal method to get the least significant bit (LSB)
    int LSB(int x) {
        return x & (-x);
    }

public:
    FenwickTree(int size) {
        n = size;
        tree.resize(n + 1, 0); // 1-based indexing
    }

    // Build the tree from an initial array
    FenwickTree(const std::vector<int>& arr) {
        n = arr.size();
        tree.resize(n + 1, 0);
        for (int i = 0; i < n; ++i) {
            update(i, arr[i]);
        }
    }

    // Point update: Adds val to index idx
    void update(int idx, int val) {
        idx += 1; // Convert to 1-based indexing
        while (idx <= n) {
            tree[idx] += val;
            idx += LSB(idx);
        }
    }

    // Query the prefix sum up to index idx
    int query(int idx) {
        idx += 1; // Convert to 1-based indexing
        int result = 0;
        while (idx > 0) {
            result += tree[idx];
            idx -= LSB(idx);
        }
        return result;
    }

    // Query the sum in range [l, r]
    int rangeQuery(int l, int r) {
        return query(r) - query(l - 1);
    }
};

int main() {
    std::vector<int> arr = {3, 2, -1, 6, 5, 4, -3, 3, 7, 2, 3};

    FenwickTree fenwick(arr);

    std::cout << "Prefix sum up to index 5: " << fenwick.query(5) << std::endl;

    std::cout << "Sum in range [3, 8]: " << fenwick.rangeQuery(3, 8) << std::endl;

    // Update index 4 by adding 2
    fenwick.update(4, 2);

    std::cout << "After update, prefix sum up to index 5: " << fenwick.query(5) << std::endl;

    return 0;
}

