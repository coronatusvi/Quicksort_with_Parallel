#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define SIZE 100000000

typedef struct {
    int *arr;
    int left;
    int right;
    int depth;
} QuickSortArgs;

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

void *quicksort(void *args) {
    QuickSortArgs *arg = (QuickSortArgs *)args;
    int *arr = arg->arr;
    int low = arg->left;
    int high = arg->right;
    int depth = arg->depth;

    if (low < high) {
        int pi = partition(arr, low, high);

        QuickSortArgs leftArgs = {arr, low, pi - 1, depth + 1};
        QuickSortArgs rightArgs = {arr, pi + 1, high, depth + 1};

        if (depth < 2) { 
            pthread_t left_thread, right_thread;
            pthread_create(&left_thread, NULL, quicksort, &leftArgs);
            pthread_create(&right_thread, NULL, quicksort, &rightArgs);

            pthread_join(left_thread, NULL);
            pthread_join(right_thread, NULL);
        } else {
            quicksort(&leftArgs);
            quicksort(&rightArgs);
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <num_threads>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);
    int *arr = (int *)malloc(SIZE * sizeof(int));
    srand(time(NULL));

    for (int i = 0; i < SIZE; i++) {
        arr[i] = rand() % SIZE;
    }

    int *arr_copy = (int *)malloc(SIZE * sizeof(int));
    for (int j = 0; j < SIZE; j++) {
        arr_copy[j] = arr[j];
    }

    QuickSortArgs args = {arr_copy, 0, SIZE - 1, 0};

    clock_t start = clock();
    pthread_t main_thread;
    pthread_create(&main_thread, NULL, quicksort, &args);
    pthread_join(main_thread, NULL);
    clock_t end = clock();

    printf("%lf\n", (double)(end - start) / CLOCKS_PER_SEC);

    free(arr_copy);
    free(arr);
    return 0;
}