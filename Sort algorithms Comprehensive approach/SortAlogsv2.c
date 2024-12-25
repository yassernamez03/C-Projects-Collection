#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


// Bubble Sort
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Selection Sort
void selectionSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIdx])
                minIdx = j;
        }
        int temp = arr[minIdx];
        arr[minIdx] = arr[i];
        arr[i] = temp;
    }
}

// Insertion Sort
void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Merge Sort
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    int L[n1], R[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// Quick Sort
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    return (i + 1);
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Heap Sort
void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
        heapify(arr, n, largest);
    }
}

void heapSort(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i > 0; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        heapify(arr, i, 0);
    }
}

// Radix Sort
int getMax(int arr[], int n) {
    int max = arr[0];
    for (int i = 1; i < n; i++)
        if (arr[i] > max)
            max = arr[i];
    return max;
}

void countingSort(int arr[], int n, int exp) {
    int output[n];
    int count[10] = {0};

    for (int i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++;

    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];

    for (int i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }

    for (int i = 0; i < n; i++)
        arr[i] = output[i];
}

void radixSort(int arr[], int n) {
    int max = getMax(arr, n);
    for (int exp = 1; max / exp > 0; exp *= 10)
        countingSort(arr, n, exp);
}

// Shell Sort
void shellSort(int arr[], int n) {
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap)
                arr[j] = arr[j - gap];
            arr[j] = temp;
        }
    }
}

// Generate Normally Distributed Random Numbers (Box-Muller Transform)
double generateNormalRandom(double mean, double stddev) {
    double u1 = (double)rand() / RAND_MAX;
    double u2 = (double)rand() / RAND_MAX;
    double z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
    return z0 * stddev + mean;
}

// Helper function to copy arrays
void copyArray(int src[], int dest[], int n) {
    for (int i = 0; i < n; i++)
        dest[i] = src[i];
}

int main() {
    int sizes[] = {1000, 2000, 4000, 8000, 16000, 32000, 64000};   
    // int sizes[] = {1000, 2000, 4000, 8000, 16000, 32000, 64000, 128000, 256000, 512000};       
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);
    int maxSize = 64000; // Maximum array size

    FILE *fp = fopen("normal_dist_sorting_times.dat", "w"); // File to save data for gnuplot

    // Allocate memory for original and temporary arrays
    double mean = 50.0, stddev = 10.0;
    int *arr = (int *)malloc(maxSize * sizeof(int));
    int *temp = (int *)malloc(maxSize * sizeof(int));

    srand(time(NULL)); // Seed for random number generation

    // Initialize array with normally distributed random values
    for (int i = 0; i < maxSize; i++)
        arr[i] = maxSize - i;

    // Measure times for each sorting algorithm
    for (int i = 0; i < numSizes; i++) {
        int n = sizes[i];

        // Bubble Sort
        copyArray(arr, temp, n);
        clock_t start = clock();
        bubbleSort(temp, n);
        clock_t end = clock();
        double bubbleTime = (double)(end - start) / CLOCKS_PER_SEC;

        // Selection Sort
        copyArray(arr, temp, n);
        start = clock();
        selectionSort(temp, n);
        end = clock();
        double selectionTime = (double)(end - start) / CLOCKS_PER_SEC;

        // Insertion Sort
        copyArray(arr, temp, n);
        start = clock();
        insertionSort(temp, n);
        end = clock();
        double insertionTime = (double)(end - start) / CLOCKS_PER_SEC;

        // Merge Sort
        copyArray(arr, temp, n);
        start = clock();
        mergeSort(temp, 0, n - 1);
        end = clock();
        double mergeTime = (double)(end - start) / CLOCKS_PER_SEC;

        // Quick Sort
        copyArray(arr, temp, n);
        start = clock();
        quickSort(temp, 0, n - 1);
        end = clock();
        double quickTime = (double)(end - start) / CLOCKS_PER_SEC;

        // Heap Sort
        copyArray(arr, temp, n);
        start = clock();
        heapSort(temp, n);
        end = clock();
        double heapTime = (double)(end - start) / CLOCKS_PER_SEC;

        // Radix Sort
        copyArray(arr, temp, n);
        start = clock();
        radixSort(temp, n);
        end = clock();
        double radixTime = (double)(end - start) / CLOCKS_PER_SEC;

        // Shell Sort
        copyArray(arr, temp, n);
        start = clock();
        shellSort(temp, n);
        end = clock();
        double shellTime = (double)(end - start) / CLOCKS_PER_SEC;

        // Save times for gnuplot
        fprintf(fp, "%d %f %f %f %f %f %f %f %f\n", n, bubbleTime, selectionTime, insertionTime, mergeTime, quickTime, heapTime, radixTime, shellTime);
    }

    fclose(fp);
    free(arr);
    free(temp);

    printf("Data saved to normal_dist_sorting_times.dat for gnuplot.\n");
    return 0;
}


// set title "Sorting Algorithm Performance on Normally Distributed Data"
// set xlabel "Array Size"
// set ylabel "Time (seconds)"
// set key outside right
// set logscale y 10  # Logarithmic scale for time (y-axis)
// plot "normal_dist_sorting_times.dat" using 1:2 with lines linewidth 2 linecolor rgb "red" title "Bubble Sort", \
//      "normal_dist_sorting_times.dat" using 1:3 with lines linewidth 2 linecolor rgb "blue" title "Selection Sort", \
//      "normal_dist_sorting_times.dat" using 1:4 with lines linewidth 2 linecolor rgb "green" title "Insertion Sort", \
//      "normal_dist_sorting_times.dat" using 1:5 with lines linewidth 2 linecolor rgb "orange" title "Merge Sort", \
//      "normal_dist_sorting_times.dat" using 1:6 with lines linewidth 2 linecolor rgb "purple" title "Quick Sort", \
//      "normal_dist_sorting_times.dat" using 1:7 with lines linewidth 2 linecolor rgb "brown" title "Heap Sort", \
//      "normal_dist_sorting_times.dat" using 1:8 with lines linewidth 2 linecolor rgb "cyan" title "Radix Sort", \
//      "normal_dist_sorting_times.dat" using 1:9 with lines linewidth 2 linecolor rgb "magenta" title "Shell Sort"