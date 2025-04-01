
```markdown
# Parallel QuickSort Project

## Mô tả

Dự án này triển khai thuật toán QuickSort song song bằng cách sử dụng các công nghệ sau:
- **Pthreads**: Sử dụng thư viện POSIX Threads để thực hiện QuickSort song song.
- **MPI**: Sử dụng giao thức Message Passing Interface để phân chia công việc giữa các tiến trình.
- **OpenMP**: Sử dụng thư viện OpenMP để thực hiện QuickSort song song trên các luồng.
- **Sequential**: Triển khai thuật toán QuickSort tuần tự để so sánh hiệu suất.

Dự án bao gồm các chương trình được tối ưu hóa để chạy trên các hệ thống đa luồng và đa tiến trình.

---

## Cấu trúc dự án

- `pthread.c`: Triển khai thuật toán QuickSort song song bằng Pthreads.
- `quicksort_mpi.c`: Triển khai thuật toán QuickSort song song bằng MPI.
- `quicksort_openmp.c`: Triển khai thuật toán QuickSort song song bằng OpenMP.
- `quicksort_sequential.c`: Triển khai thuật toán QuickSort tuần tự.
- `define.c`: Định nghĩa kích thước mảng (`SIZE`) và các thông số chung.
- `run_experiments.sh`: Script Bash để chạy các chương trình và ghi kết quả vào file CSV.
- `output_pthread.csv`: File CSV chứa kết quả thời gian thực thi của chương trình Pthreads.

---

## Yêu cầu hệ thống

- **Hệ điều hành**: Linux
- **Cấu hình**: 12 thread (Tối thiểu)
- **Trình biên dịch**: GCC (hỗ trợ `-pthread`, `-fopenmp`, và `mpicc`)
- **Thư viện**:
  - Pthreads
  - OpenMP
  - MPI (OpenMPI hoặc MPICH)

---

## Cách chạy

### 1. **Biên dịch chương trình**

Sử dụng các lệnh sau để biên dịch từng chương trình:

- **Pthreads**:
  ```bash
  gcc -pthread pthread.c -o pthread -O2
  ```

- **MPI**:
  ```bash
  mpicc quicksort_mpi.c -o mpi -O2
  ```

- **OpenMP**:
  ```bash
  gcc -fopenmp quicksort_openmp.c -o openmp -O2
  ```

- **Sequential**:
  ```bash
  gcc quicksort_sequential.c -o sequential -O2
  ```

---

### 2. **Chạy chương trình**

#### **Pthreads**
Chạy chương trình Pthreads với số lượng luồng khác nhau:
```bash
./pthread <num_threads>
```
Ví dụ:
```bash
./pthread 4
```

#### **MPI**
Chạy chương trình MPI với số lượng tiến trình khác nhau:
```bash
mpirun --oversubscribe -np <num_processes> ./mpi
```
Ví dụ:
```bash
mpirun --oversubscribe -np 4 ./mpi
```

#### **OpenMP**
Chạy chương trình OpenMP với số lượng luồng khác nhau:
```bash
./openmp <num_threads>
```
Ví dụ:
```bash
./openmp 4
```

#### **Sequential**
Chạy chương trình tuần tự:
```bash
./sequential
```

---

### 3. **Chạy script để ghi kết quả vào CSV**

Sử dụng script `run_csv.sh` để chạy chương trình Pthreads với các số lượng luồng khác nhau và ghi kết quả vào file CSV:
```bash
chmod +x run_csv.sh
run_csv.sh
```

Kết quả sẽ được lưu vào file `output.csv` với định dạng:
```
STT,Program,Threads,Time (seconds)
1,Sequential,1,3.979709
1,OpenMP,2,3.400825
1,OpenMP,4,2.073214
1,OpenMP,6,1.654534
1,OpenMP,8,2.179912
1,OpenMP,12,1.893082
1,Pthreads,2,4.926762
1,Pthreads,4,4.721931
```

---

## Ghi chú

- Đảm bảo rằng thư viện đã được cài đặt trước khi chạy chương trình.
- Kích thước mảng (`SIZE`) được định nghĩa trong file `define.c`. Bạn có thể thay đổi giá trị này để kiểm tra hiệu suất với các kích thước mảng khác nhau.
- Nếu gặp lỗi segmentation fault, hãy kiểm tra việc cấp phát bộ nhớ trong các chương trình.

---

## Tác giả

Dự án được thực hiện bởi Vi Đăng Quang, Nguyễn Quang Hệ, Trần Minh Hiếu, Phạm Hoàng Anh - Lập trình song song-1-2-24(N01).
```
