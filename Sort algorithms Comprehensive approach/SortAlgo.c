#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_SIZE 10000
#define STEP_SIZE 1000
#define NUM_ALGORITHMS 4

void bubble_sort(int arr[], int n);
void insertion_sort(int arr[], int n);
void selection_sort(int arr[], int n);
void quick_sort(int arr[], int low, int high);
int partition(int arr[], int low, int high);
void generate_random_array(int arr[], int n);
void copy_array(int src[], int dest[], int n);
double measure_time(void (*sort_func)(int[], int), int arr[], int n);
void plot_results();

int main() {
    int original_array[MAX_SIZE];
    int temp_array[MAX_SIZE];
    double times[NUM_ALGORITHMS][MAX_SIZE / STEP_SIZE];
    char *algorithm_names[] = {"Bubble Sort", "Insertion Sort", "Selection Sort", "Quick Sort"};
    FILE *fp;

    srand(time(NULL));

    fp = fopen("sorting_times.dat", "w");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    fprintf(fp, "Size BubbleSort InsertionSort SelectionSort QuickSort\n");

    for (int size = STEP_SIZE; size <= MAX_SIZE; size += STEP_SIZE) {
        generate_random_array(original_array, size);
        
        printf("Processing array size: %d\n", size);

        // Bubble Sort
        copy_array(original_array, temp_array, size);
        times[0][(size / STEP_SIZE) - 1] = measure_time(bubble_sort, temp_array, size);

        // Insertion Sort
        copy_array(original_array, temp_array, size);
        times[1][(size / STEP_SIZE) - 1] = measure_time(insertion_sort, temp_array, size);

        // Selection Sort
        copy_array(original_array, temp_array, size);
        times[2][(size / STEP_SIZE) - 1] = measure_time(selection_sort, temp_array, size);

        // Quick Sort
        copy_array(original_array, temp_array, size);
        times[3][(size / STEP_SIZE) - 1] = measure_time(quick_sort, temp_array, size);

        fprintf(fp, "%d %.6f %.6f %.6f %.6f\n", size, 
                times[0][(size / STEP_SIZE) - 1],
                times[1][(size / STEP_SIZE) - 1],
                times[2][(size / STEP_SIZE) - 1],
                times[3][(size / STEP_SIZE) - 1]);
    }

    fclose(fp);
    plot_results();

    return 0;
}

void bubble_sort(int arr[], int n) {
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

void insertion_sort(int arr[], int n) {
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

void selection_sort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        int temp = arr[min_idx];
        arr[min_idx] = arr[i];
        arr[i] = temp;
    }
}

void quick_sort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi + 1, high);
    }
}

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

void generate_random_array(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 10000;
    }
}

void copy_array(int src[], int dest[], int n) {
    memcpy(dest, src, n * sizeof(int));
}

double measure_time(void (*sort_func)(int[], int), int arr[], int n) {
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    if (sort_func == quick_sort) {
        quick_sort(arr, 0, n - 1);
    } else {
        sort_func(arr, n);
    }
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    return cpu_time_used;
}

void plot_results() {
    FILE *gnuplot = popen("gnuplot -persistent", "w");
    if (gnuplot == NULL) {
        printf("Error opening gnuplot.\n");
        return;
    }

    fprintf(gnuplot, "set title 'Sorting Algorithm Comparison'\n");
    fprintf(gnuplot, "set xlabel 'Array Size'\n");
    fprintf(gnuplot, "set ylabel 'Time (seconds)'\n");
    fprintf(gnuplot, "set key outside\n");
    fprintf(gnuplot, "set grid\n");
    fprintf(gnuplot, "plot 'sorting_times.dat' using 1:2 with lines title 'Bubble Sort', \
                           '' using 1:3 with lines title 'Insertion Sort', \
                           '' using 1:4 with lines title 'Selection Sort', \
                           '' using 1:5 with lines title 'Quick Sort'\n");

    fclose(gnuplot);
}