#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "define.c" // Đảm bảo define.c có định nghĩa SIZE

typedef struct
{
    int *arr;
    int left;
    int right;
} QuickSortArgs;

pthread_t *thread_pool;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int task_count = 0;
int task_capacity = 1000; // Kích thước ban đầu của task_queue
QuickSortArgs *task_queue;
int THREAD_POOL_SIZE;
int active_threads = 0; // Đếm số luồng đang chạy

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int low, int high)
{
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quicksort_sequential(int arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);
        quicksort_sequential(arr, low, pi - 1);
        quicksort_sequential(arr, pi + 1, high);
    }
}

void *worker_thread(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&mutex);
        while (task_count == 0)
        {
            if (active_threads == 0)
            {
                pthread_mutex_unlock(&mutex);
                pthread_exit(NULL); // Kết thúc khi không còn công việc
            }
            pthread_cond_wait(&cond, &mutex);
        }

        QuickSortArgs task = task_queue[--task_count];
        pthread_mutex_unlock(&mutex);

        if (task.left >= task.right)
            continue; // Bỏ qua task không hợp lệ

        int pi = partition(task.arr, task.left, task.right);

        QuickSortArgs left_task = {task.arr, task.left, pi - 1};
        QuickSortArgs right_task = {task.arr, pi + 1, task.right};

        pthread_mutex_lock(&mutex);
        if (task_count + 2 >= task_capacity)
        {
            task_capacity *= 2;
            task_queue = realloc(task_queue, task_capacity * sizeof(QuickSortArgs));
        }
        task_queue[task_count++] = left_task;
        task_queue[task_count++] = right_task;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void quicksort_parallel(int arr[], int low, int high)
{
    if (high - low < 10000)
    {
        quicksort_sequential(arr, low, high);
        return;
    }

    int pi = partition(arr, low, high);

    QuickSortArgs left_task = {arr, low, pi - 1};
    QuickSortArgs right_task = {arr, pi + 1, high};

    pthread_mutex_lock(&mutex);
    task_queue[task_count++] = left_task;
    task_queue[task_count++] = right_task;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <num_threads>\n", argv[0]);
        return 1;
    }

    THREAD_POOL_SIZE = atoi(argv[1]);
    thread_pool = (pthread_t *)malloc(THREAD_POOL_SIZE * sizeof(pthread_t));
    task_queue = (QuickSortArgs *)malloc(task_capacity * sizeof(QuickSortArgs));

    int *arr = (int *)malloc(SIZE * sizeof(int));
    srand(time(NULL));
    for (int i = 0; i < SIZE; i++)
    {
        arr[i] = rand() % SIZE;
    }

    int *arr_copy = (int *)malloc(SIZE * sizeof(int));
    for (int j = 0; j < SIZE; j++)
    {
        arr_copy[j] = arr[j];
    }

    // Khởi tạo thread pool
    for (int i = 0; i < THREAD_POOL_SIZE; i++)
    {
        pthread_create(&thread_pool[i], NULL, worker_thread, NULL);
        active_threads++;
    }

    clock_t start = clock();
    quicksort_parallel(arr_copy, 0, SIZE - 1);
    clock_t end = clock();

    printf("%lf\n", (double)(end - start) / CLOCKS_PER_SEC);

    // Dọn dẹp
    pthread_mutex_lock(&mutex);
    active_threads = 0;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);

    for (int i = 0; i < THREAD_POOL_SIZE; i++)
    {
        pthread_join(thread_pool[i], NULL);
    }

    free(arr_copy);
    free(arr);
    free(thread_pool);
    free(task_queue);
    return 0;
}
