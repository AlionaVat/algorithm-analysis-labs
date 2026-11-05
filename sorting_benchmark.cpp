// =====================================================================
// LABORATORY WORK No. 1 - SORTING ALGORITHMS            (Variant 4)
// Course: Analysis and Complexity of Algorithms
// =====================================================================
//
// GOAL
//   Study several sorting algorithms and compare their properties.
//
// TASK (from the lab manual)
//   1. Write two functions that sort an array of integers: one from the class
//      of quadratic algorithms and one from the class of fast algorithms.
//      Variant 4: BUBBLE SORT (quadratic) and MERGE SORT (fast).
//   2. Study the computational efficiency of both algorithms. The program
//      creates an array filled with random numbers, calls the sorting function
//      and measures its running time with the standard library function
//      clock() (header time.h). If one sort is too fast to measure, the array
//      generation and sorting are repeated in a loop and the average time of
//      one sort is calculated.
//   3. Result: sorting times of both methods for array sizes from 1000 to
//      100000 (7 different sizes are used here).
//
// HOW THE PROGRAM WORKS
//   - bubbleSort()  : O(n^2) time, O(1) extra memory.
//   - mergeSort()   : O(n log n) time, O(n) extra memory
//                     (recurrence T(n) = 2T(n/2) + O(n)).
//   - measure()     : fills an array with rand() numbers, times ONLY the sort
//                     with clock(), repeats until the total time reaches 0.3 s,
//                     returns the average time of one sort in seconds.
//   - isSorted()    : checks that every result is really sorted.
//   - main()        : runs both algorithms for 7 sizes and prints a table
//                     (time of each algorithm and the ratio bubble / merge).
//
// HOW TO BUILD AND RUN
//   macOS or Linux (VS Code terminal: Terminal > New Terminal):
//       clang++ -std=c++17 -O2 -o lab1 sorting_benchmark.cpp        (or: g++ -std=c++17 -O2 -o lab1 sorting_benchmark.cpp)
//       ./lab1
//     On macOS, if "clang++: command not found", run:  xcode-select --install
//
//   Windows, Visual Studio:
//       File > New > Project > Console App (C++) > Create
//       Paste this code into the .cpp file, choose "Release" and "x64" in the
//       toolbar, then Debug > Start Without Debugging (Ctrl+F5).
//     If you get "unexpected end of file while looking for precompiled header":
//       Project > Properties > C/C++ > Precompiled Headers > Not Using
//       Precompiled Headers.
//
//   IMPORTANT: always build with optimization (-O2 / Release). In Debug mode
//   bubble sort for n = 100000 can take several minutes.
//
// EXPECTED OUTPUT (times depend on your computer; the trend must be the same)
//   - bubble sort: time grows about 4 times when n doubles (quadratic, O(n^2));
//   - merge sort : time grows slightly faster than n (O(n log n));
//   - for n = 100000 merge sort is thousands of times faster than bubble sort.
//   The whole run takes about 1 minute (most of it is bubble sort, n = 100000).
//
// Standard C++ only: builds with MSVC (Visual Studio), clang and g++.
// =====================================================================

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

using namespace std;

// ---------------------------------------------------------------------
// 1. Bubble sort. Complexity O(n^2), memory O(1).
// ---------------------------------------------------------------------
void bubbleSort(int a[], long n) {
    for (long i = 0; i < n - 1; i++) {
        bool swapped = false;                       // was there a swap in this pass?
        for (long j = 0; j < n - 1 - i; j++) {
            if (a[j] > a[j + 1]) {
                swap(a[j], a[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;                        // no swaps: array is already sorted
    }
}

// ---------------------------------------------------------------------
// 2. Merge sort. Complexity O(n log n), extra memory O(n).
//    T(n) = 2T(n/2) + O(n) = O(n log n)
// ---------------------------------------------------------------------
static int* buf = NULL;                             // helper array used when merging

// Merge sorted parts a[lb..split] and a[split+1..rb] into a[lb..rb]
static void mergeParts(int a[], long lb, long split, long rb) {
    long i = lb, j = split + 1, k = lb;
    while (i <= split && j <= rb) {
        if (a[i] <= a[j]) buf[k++] = a[i++];
        else              buf[k++] = a[j++];
    }
    while (i <= split) buf[k++] = a[i++];           // rest of the left part
    while (j <= rb)    buf[k++] = a[j++];           // rest of the right part
    for (k = lb; k <= rb; k++) a[k] = buf[k];       // copy the result back
}

// a - array to sort, lb - left border, rb - right border
static void mergeSortRec(int a[], long lb, long rb) {
    if (lb < rb) {                                  // more than one element
        long split = (lb + rb) / 2;                 // index where the array is divided
        mergeSortRec(a, lb, split);                 // sort the left half
        mergeSortRec(a, split + 1, rb);             // sort the right half
        mergeParts(a, lb, split, rb);               // merge the results
    }
}

void mergeSort(int a[], long n) {
    buf = new int[n];
    mergeSortRec(a, 0, n - 1);
    delete[] buf;
    buf = NULL;
}

// ---------------------------------------------------------------------
// 3. Time measurement with clock() from time.h
// ---------------------------------------------------------------------
typedef void (*SortFunc)(int[], long);

bool isSorted(const int a[], long n) {
    for (long i = 1; i < n; i++)
        if (a[i - 1] > a[i]) return false;
    return true;
}

// Average time (seconds) of ONE sort of an array of n random numbers.
// A new random array is created each time and only the sorting itself is timed.
// If one sort is too fast, the loop repeats until the total time reaches
// minTotal seconds, then the total is divided by the number of repetitions.
double measure(SortFunc sort, long n, double minTotal, long& reps) {
    srand(12345);                                   // same arrays for both algorithms
    int* a = new int[n];
    clock_t total = 0;                              // total clock ticks
    reps = 0;
    do {
        for (long i = 0; i < n; i++) a[i] = rand(); // fill with random numbers
        clock_t start = clock();
        sort(a, n);
        clock_t finish = clock();
        total += finish - start;
        reps++;
        if (!isSorted(a, n)) {                      // correctness check
            printf("ERROR: array is not sorted, n = %ld\n", n);
            exit(1);
        }
    } while ((double)total / CLOCKS_PER_SEC < minTotal);
    delete[] a;
    return (double)total / CLOCKS_PER_SEC / reps;   // ticks -> seconds, average
}

int main() {
    const long sizes[] = { 1000, 2000, 5000, 10000, 20000, 50000, 100000 };
    const int count = sizeof(sizes) / sizeof(sizes[0]);

    printf("Variant 4: bubble sort and merge sort\n");
    printf("(average time of one sort of a random array)\n\n");
    printf("%-9s | %-14s | %-7s | %-14s | %-7s | %s\n",
           "n", "bubble, s", "repeats", "merge, s", "repeats", "ratio");
    printf("----------+----------------+---------+----------------+---------+--------\n");

    for (int i = 0; i < count; i++) {
        long repsB, repsM;
        double tb = measure(bubbleSort, sizes[i], 0.3, repsB);
        double tm = measure(mergeSort, sizes[i], 0.3, repsM);
        printf("%-9ld | %-14.6f | %-7ld | %-14.6f | %-7ld | %.1f\n",
               sizes[i], tb, repsB, tm, repsM, tb / tm);
        fflush(stdout);
    }

    printf("\nDone. Press Enter to close the window...\n");
    getchar();                                      // keeps the console open
    return 0;
}
