#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include "define.c"

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quicksort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

int main(int argc, char *argv[]) {
    int rank, size;
    int *arr = NULL;
    int local_N, *local_arr;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        arr = (int *)malloc(SIZE * sizeof(int));
        srand(time(NULL));
        for (int i = 0; i < SIZE; i++) {
            arr[i] = rand() % SIZE;
        }
    }

    local_N = SIZE / size;
    local_arr = (int *)malloc(local_N * sizeof(int));

    MPI_Scatter(arr, local_N, MPI_INT, local_arr, local_N, MPI_INT, 0, MPI_COMM_WORLD);

    double start = MPI_Wtime();
    quicksort(local_arr, 0, local_N - 1);
    double end = MPI_Wtime();

    MPI_Gather(local_arr, local_N, MPI_INT, arr, local_N, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("%lf\n", end - start);
        free(arr);
    }

    free(local_arr);
    MPI_Finalize();
    return 0;
}