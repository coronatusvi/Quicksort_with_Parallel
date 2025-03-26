#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "define.c"

// Hàm tạo mảng số ngẫu nhiên
void generateRandomArray(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % SIZE;
    }
}

// Hàm hoán đổi hai số
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Hàm chia mảng và chọn pivot
int partition(int arr[], int low, int high) {
    int pivot = arr[high]; // Chọn pivot là phần tử cuối cùng
    int i = (low - 1);
    
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// Hàm QuickSort sử dụng OpenMP
void quickSortParallel(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        
        #pragma omp task // Tạo task song song cho bên trái pivot
        quickSortParallel(arr, low, pi - 1);
        
        #pragma omp task // Tạo task song song cho bên phải pivot
        quickSortParallel(arr, pi + 1, high);
    }
}

// Hàm gọi QuickSort trong môi trường song song
void quickSort(int arr[], int low, int high) {
    #pragma omp parallel
    {
        #pragma omp single nowait
        quickSortParallel(arr, low, high);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <num_threads>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);
    int *arr = (int *)malloc(SIZE * sizeof(int));
    if (arr == NULL) {
        printf("Lỗi cấp phát bộ nhớ!\n");
        return 1;
    }

    srand(time(NULL));
    generateRandomArray(arr, SIZE);

    int *arr_copy = (int *)malloc(SIZE * sizeof(int));
    if (arr_copy == NULL) {
        printf("Lỗi cấp phát bộ nhớ cho arr_copy!\n");
        free(arr);
        return 1;
    }

    for (int j = 0; j < SIZE; j++) {
        arr_copy[j] = arr[j];
    }

    omp_set_num_threads(num_threads);
    
    double start_time = omp_get_wtime();

    quickSort(arr_copy, 0, SIZE - 1);

    double end_time = omp_get_wtime(); // Kết thúc đo thời gian
    
    printf("%lf\n", end_time - start_time);

    free(arr_copy); // Giải phóng bộ nhớ của arr_copy
    free(arr); // Giải phóng bộ nhớ của mảng gốc

    return 0;
}