#!/bin/bash

# Xóa file output cũ nếu có
# rm -f output_pthread.csv

# Tạo tiêu đề cho file CSV
echo "STT,Program,Threads,Time (seconds)" > output_pthread.csv

gcc -pthread pthread.c -o pthread -O2

for i in {1..100}
do          
    echo "🔹 $i"
    for threads in 2 4 6 8 12       
    do
        runtime=$(./pthread $threads)  
        echo "$i,Pthreads,$threads,$runtime" >> output_pthread.csv
    done
done

rm -f pthread

echo "✅ Hoàn thành! Kết quả được lưu vào output_pthread.csv"
