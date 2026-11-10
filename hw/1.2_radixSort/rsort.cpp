#include <string>
#include <vector>
#include <tuple>
#include <algorithm>

extern "C" {
    int find_min(const int* nums,const int n) {
        int min = nums[0];
        for (int i = 1; i < n; ++i) {
            if (nums[i] < min) {min = nums[i];}
        }
        return min;
    }

    int find_max(const int* nums, const int n) {
        int max = nums[0];
        for (int i = 1; i < n; ++i) {
            if (nums[i]>max) {max = nums[i];}
        }
        return max;
    }

    void radix_bin_sort(int* nums, const int n) { // msd-first, no recursion
    std::vector<std::tuple<int,int,int>> stack;
    stack.reserve(n);
    stack.emplace_back(0, n, static_cast<int>(sizeof(int))*8 - 1);
    while (!stack.empty()) {
        const auto [start, len, bit] = stack.back(); // start/end of current subarray, bit im sorting with
        stack.pop_back();
        if (1 >= len || 0 > bit) continue;
        int zeros = start;
        int ones = start + len - 1;
        while (ones >= zeros) { // ones being 'unchecked yet'
            if (((nums[zeros] >> bit) & 1) != 0) { // if bit it 1
                std::swap(nums[zeros], nums[ones]);
                --ones;
            } else {
                ++zeros;
            }
        }
        if (zeros - start > 0)
            stack.emplace_back(start, zeros - start, bit - 1);
        if ((start + len) - zeros > 0)
            stack.emplace_back(zeros, (start + len) - zeros, bit - 1);
    }
}
}
