#!/bin/bash

# Xóa file output cũ nếu có
rm -f output.csv

# Tạo tiêu đề cho file CSV
echo "STT,Program,Threads,Time (seconds)" > output.csv

gcc quick_sort.c -o sequential -O2
gcc -fopenmp quicksort_openmp.c -o openmp -O2
gcc -pthread quicksort_pthreads.c -o pthread -O2
mpicc quicksort_mpi.c -o mpi -O2

# Chạy 100 lần cho mỗi chương trình
for i in {1..1}
do      
    echo "🔹 $i"

    # echo "🔹 Chương trình tuần tự"
    time=$(./sequential)
    echo "$i,Sequential,1,$time" >> output.csv

    # echo "🔹 Chương trình OpenMP"
    for threads in 2 4 6 8 12
    do
        time=$(./openmp $threads)
        echo "$i,OpenMP,$threads,$time" >> output.csv
    done

    echo "🔹 Chương trình Pthreads"
    for threads in 2 4 6 8 12
    do
        time=$(./pthread $threads)
        echo "$i,Pthreads,$threads,$time" >> output.csv
    done

    # echo "🔹 Chương trình MPI"
    for procs in 2 4 6 8 12
    do
        time=$(mpirun --oversubscribe -np $procs ./mpi)
        echo "$i,MPI,$procs,$time" >> output.csv
    done
done

# Xóa các file out sau khi chạy xong
rm -f sequential openmp pthread mpi

echo "✅ Hoàn thành! Kết quả được lưu vào output.csv"