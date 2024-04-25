#include <iostream>
#include <cstring>
using namespace std;

const int kMaxKey = 65535;
const int kMaxValueLength = 64;

struct Pair {
    int key;
    char value[kMaxValueLength + 1];
};

void CountingSort(Pair* pairs, int n) {
    // Step 1: Initialize counter array with all zeros.
    int count[kMaxKey + 1] = {0};

    // Step 2: Count the number of occurrences of each key.
    for (int i = 0; i < n; ++i) {
        ++count[pairs[i].key];
    }

    // Step 3: Modify count array so that each element stores the sum of previous counts.
    for (int i = 1; i <= kMaxKey; ++i) {
        count[i] += count[i - 1];
    }

    // Step 4: Build the output array.
    Pair sorted[n];
    for (int i = n - 1; i >= 0; --i) {
        int index = --count[pairs[i].key];
        sorted[index] = pairs[i];
    }

    // Step 5: Copy the output array back to the input array.
    memcpy(pairs, sorted, n * sizeof(Pair));
}

int main() {
    // Read input pairs from standard input into a dynamically allocated array.
    int n = 0;
    Pair* pairs = new Pair[1];
    int key;
    char value[kMaxValueLength + 1];
    while (cin >> key >> value) {
        if (strlen(value) < kMaxValueLength) {
            memset(value + strlen(value), 0, kMaxValueLength - strlen(value));
        }
        pairs[n].key = key;
        strncpy(pairs[n].value, value, kMaxValueLength);
        ++n;
        pairs = static_cast<Pair*>(realloc(pairs, (n + 1) * sizeof(Pair)));
    }

    // Sort the array of pairs using counting sort.
    CountingSort(pairs, n);

    // Print the sorted pairs to standard output.
    for (int i = 0; i < n; ++i) {
        cout << pairs[i].key << "\t" << pairs[i].value << "\n";
    }

    // Free dynamically allocated memory.
    delete[] pairs;

    return 0;
}
