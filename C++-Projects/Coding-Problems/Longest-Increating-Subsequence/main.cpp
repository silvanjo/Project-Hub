#include <iostream>
#include <vector>
#include <algorithm>

// Function to calculate the length of the LIS
int lengthOfLIS(const std::vector<int>& nums) {
    if (nums.empty())
        return 0;

    std::vector<int> tails;
    for (int num : nums) {
        auto it = std::lower_bound(tails.begin(), tails.end(), num);
        if (it == tails.end())
            tails.push_back(num);
        else
            *it = num;
    }
    return tails.size();
}

int main() {
    std::vector<int> nums = {10, 9, 2, 5, 3, 7, 101, 18};

    int length = lengthOfLIS(nums);
    std::cout << "Length of Longest Increasing Subsequence: " << length << std::endl;

    return 0;
}

