#include <iostream>
#include "rsort.h"

// this file exists only for testing the "library"
int main() {

    int ar[10] = {1, 2, 12, 4, 32, 6, 75, 68, 9, 110};
    radix_bin_sort(ar,10);
    for (int i : ar) {
        std::string s = std::to_string(i);
        std::cout << s << std::endl;
    }
    return 0;
}include
