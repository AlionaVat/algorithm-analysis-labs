# Lab 1: Sorting Algorithms (Bubble Sort vs Merge Sort)

A C++ benchmark that compares a quadratic sorting algorithm (**bubble sort**) with a fast one (**merge sort**) on random integer arrays of growing size. It is Laboratory Work No. 1 of the course *Analysis and Complexity of Algorithms* (variant 4).

## Task

1. Implement two functions that sort an array of integers: one from the class of quadratic algorithms and one from the class of fast algorithms.
2. Study their computational efficiency. The program creates an array filled with random numbers, calls the sorting function and measures its running time with the standard library. If one sort is too fast to measure, array generation and sorting are repeated in a loop and the average time is used.
3. Report sorting times of both methods for array sizes from 1,000 to 100,000 (7 sizes are used).
4. Measure time with `clock()` from `time.h`.

## Algorithms

| Algorithm | Time complexity | Extra memory | Notes |
|---|---|---|---|
| Bubble sort | O(n²) | O(1) | Works in place; stops early if a pass makes no swaps |
| Merge sort | O(n log n) | O(n) | Divide and conquer, T(n) = 2T(n/2) + O(n) |

## How it works

- `bubbleSort()` and `mergeSort()` sort an `int` array in place.
- `measure()` fills an array with `rand()` numbers, times only the sort with `clock()`, repeats until the total time reaches 0.3 s and returns the average time of one sort in seconds. Both algorithms receive identical arrays (`srand(12345)`).
- `isSorted()` verifies that every result is correctly sorted.
- `main()` runs both algorithms for 7 sizes and prints a table.

## Build and run

macOS / Linux:

```bash
clang++ -std=c++17 -O2 -o lab1 sorting_benchmark.cpp   # or g++
./lab1
```

On macOS, if `clang++` is missing, run `xcode-select --install` first.

Windows (Visual Studio): create a *Console App (C++)* project, paste `sorting_benchmark.cpp`, select **Release** and **x64**, then press **Ctrl+F5**.

Always build with optimization (`-O2` or Release). In Debug mode bubble sort for n = 100,000 can take several minutes. A full run takes about a minute.

## Results

Measured on a MacBook Pro (macOS) with `clang++ -std=c++17 -O2`. Average time of one sort, in seconds. Raw data: [`results.csv`](results.csv).

| n | Bubble sort, s | Merge sort, s | Ratio (bubble / merge) |
|---:|---:|---:|---:|
| 1,000 | 0.000293 | 0.000021 | 14.2 |
| 2,000 | 0.000983 | 0.000046 | 21.3 |
| 5,000 | 0.005789 | 0.000126 | 45.9 |
| 10,000 | 0.022821 | 0.000266 | 85.6 |
| 20,000 | 0.094260 | 0.000584 | 161.4 |
| 50,000 | 0.638168 | 0.001596 | 399.8 |
| 100,000 | 3.610475 | 0.003458 | 1044.0 |

Absolute times depend on the computer; the growth trend does not.

## Conclusions

- **Bubble sort is quadratic.** When n doubles, the time grows about four times (5,000 to 10,000: x3.9; 10,000 to 20,000: x4.1), which matches O(n²). At the largest sizes growth is slightly faster (50,000 to 100,000: x5.7), most likely because the array no longer fits in the processor cache.
- **Merge sort grows like n log n.** When n grows 100 times (1,000 to 100,000), the time grows about 165 times, close to the theoretical value of about 167.
- **The gap widens with size.** Merge sort is 14 times faster at n = 1,000 and more than 1,000 times faster at n = 100,000.
- **Trade-off.** Merge sort needs O(n) extra memory, while bubble sort sorts in place with O(1) memory. For large arrays a fast O(n log n) algorithm is the right choice.

## Project structure

```
lab1-sorting-algorithms/
├── sorting_benchmark.cpp   # source code with detailed comments
├── results.csv             # measured times
└── README.md
```

## Author

Aliona Vataman
