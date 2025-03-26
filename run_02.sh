#!/bin/bash

# Xóa file output cũ nếu có
rm -f output_02_thread.csv

# Tạo tiêu đề cho file CSV
echo "SIZE,Program,Threads,Time (seconds)" > output_02_thread.csv

# Các giá trị SIZE cần chạy
sizes=(100 500 20000 40000 60000 80000)

# Hàm thay đổi giá trị SIZE trong file define.c
update_size_in_define() {           
    sed -i "s/^#define SIZE .*/#define SIZE $1/" define.c
}

# Lặp qua từng giá trị SIZE
for size in "${sizes[@]}"
do
    echo "🔹 Đang chạy với SIZE = $size"

    # Cập nhật SIZE trong file define.c
    update_size_in_define $size

    # Biên dịch lại các chương trình
    gcc quick_sort.c -o sequential -O2
    gcc -fopenmp quicksort_openmp.c -o openmp -O2
    gcc -pthread quicksort_pthreads.c -o pthread -O2
    mpicc quicksort_mpi.c -o mpi -O2

    # Chạy mỗi chương trình 10 lần và tính trung bình thời gian
    for program in sequential openmp pthread mpi
    do
        total_time=0
        for run in {1..10}
        do
            if [ "$program" == "openmp" ]; then
                time=$(./$program 2)
            elif [ "$program" == "pthread" ]; then
                time=$(./$program 2)
            elif [ "$program" == "mpi" ]; then
                time=$(mpirun --oversubscribe -np 2 ./$program)
            else
                time=$(./$program)
            fi
            total_time=$(echo "$total_time + $time" | bc)
        done
        avg_time=$(echo "scale=6; $total_time / 10" | bc)
        if [ "$program" == "sequential" ]; then
            echo "$size,Sequential,1,$avg_time" >> output_02_thread.csv
        elif [ "$program" == "openmp" ]; then
            echo "$size,OpenMP,2,$avg_time" >> output_02_thread.csv
        elif [ "$program" == "pthread" ]; then
            echo "$size,Pthreads,2,$avg_time" >> output_02_thread.csv
        elif [ "$program" == "mpi" ]; then
            echo "$size,MPI,2,$avg_time" >> output_02_thread.csv
        fi
    done
done

# Xóa các file thực thi sau khi chạy xong
rm -f sequential openmp pthread mpi

echo "✅ Hoàn thành! Kết quả được lưu vào output_02_thread.csv"