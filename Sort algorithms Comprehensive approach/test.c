#include <stdio.h>
#include <time.h>
#include <math.h>

void selectionSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIdx])
                minIdx = j;
        }
        // Swap
        int temp = arr[minIdx];
        arr[minIdx] = arr[i];
        arr[i] = temp;
    }
}

void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                // Swap
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main() {
    int sizes[] = {100, 500, 1000, 5000, 10000}; // Different sizes for comparison
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);

    FILE *fp = fopen("sorting_times.dat", "w"); // File to store data for gnuplot

    for (int i = 0; i < numSizes; i++) {
        int n = sizes[i];

        int arr1[n], arr2[n];
        // Initialize the arrays with random data for both sorting algorithms
        for (int j = 0; j < n; j++) {
            int value = rand() % 1000;
            arr1[j] = value;
            arr2[j] = value;
        }

        // Measure time for Bubble Sort
        clock_t start = clock();
        bubbleSort(arr1, n);
        clock_t end = clock();
        double bubbleTime = (double)(end - start) / CLOCKS_PER_SEC;

        // Measure time for Selection Sort
        start = clock();
        selectionSort(arr2, n);
        end = clock();
        double selectionTime = (double)(end - start) / CLOCKS_PER_SEC;

        // Write the size and times to the file
        fprintf(fp, "%d %f %f\n", n, bubbleTime, selectionTime);
    }

    fclose(fp);

    printf("Data saved to sorting_times.dat for gnuplot.\n");
    return 0;
}
