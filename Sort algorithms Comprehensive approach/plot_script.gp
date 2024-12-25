set title 'Sorting Algorithm Comparison'
set xlabel 'Array Size'
set ylabel 'Execution Time (seconds)'
set key outside
set grid
set term png size 1200,800
set output 'sorting_comparison.png'
plot 'sorting_data.txt' using 1:2 title 'Bubble Sort' with lines, 'sorting_data.txt' using 1:3 title 'Selection Sort' with lines, 'sorting_data.txt' using 1:4 title 'Insertion Sort' with lines, 'sorting_data.txt' using 1:5 title 'Merge Sort' with lines, 'sorting_data.txt' using 1:6 title 'Quick Sort' with lines