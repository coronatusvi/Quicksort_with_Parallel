#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "define.c"

// Tạo dãy số ngẫu nhiên
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
    int pivot = arr[high]; // Chọn phần tử cuối cùng làm pivot
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

// Hàm quicksort
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int main() {
    // Cấp phát bộ nhớ động cho mảng
    int *arr = (int *)malloc(SIZE * sizeof(int));
    if (arr == NULL) {
        printf("Lỗi cấp phát bộ nhớ!\n");
        return 1;
    }

    // Khởi tạo bộ sinh số ngẫu nhiên
    srand(time(NULL));

    // Tạo mảng ngẫu nhiên
    generateRandomArray(arr, SIZE);

    // Bắt đầu đo thời gian
    clock_t start = clock();

    // Sắp xếp tăng dần
    quickSort(arr, 0, SIZE - 1);

    // Kết thúc đo thời gian
    clock_t end = clock();
    
    // Tính thời gian thực thi
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("%lf", time_taken);

    // Giải phóng bộ nhớ
    free(arr);

    return 0;
}