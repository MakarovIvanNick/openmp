#include <iostream>
#include <chrono>
#include <omp.h>

void print_matrix (uint64_t** matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

uint64_t** create (int size, bool empty) {
    uint64_t** matrix = new uint64_t*[size];
    for (int i = 0; i < size; ++i) {
        matrix[i] = new uint64_t[size];
        for (int j = 0; j < size; ++j) {
            if (empty) {
                matrix[i][j] = 0;
            } else {
                matrix[i][j] = rand() % 10;
            }
        }
    }
    return matrix;
}

uint64_t** multi (uint64_t** m1, uint64_t** m2, int size) {
    uint64_t** res = create(size, true);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            for (int k = 0; k < size; ++k) {
                res[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
    return res;
}

uint64_t** multiMP (uint64_t** m1, uint64_t** m2, int size) {
    uint64_t** res = create(size, true);
    #pragma omp parallel for
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            for (int k = 0; k < size; ++k) {
                res[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
    return res;
}

uint64_t** transpose_m (uint64_t** matrix, int size) {
    uint64_t** res = create(size, true);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            res[i][j] = matrix[j][i];
        }
    }
    return res;
}

bool is_equal_matrix (uint64_t** m1, uint64_t** m2, int size) {
    bool equal = true;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (m1[i][j] != m2[i][j]) {
                equal = false;
            }
        }
    }
    return equal;
}

int main() {
    int size = 4096;
//    for (int i = 2; i <=10; ++i) {
//        std::cout << "Ex. " << i << "\n";
    uint64_t **Am = create(size, false);
    uint64_t **Bm = create(size, false);
    uint64_t **one_thread_arr = create(size, true);
    uint64_t **multiple_threads_arr = create(size, true);
//    print_matrix(Am, size);
    std::cout << "\n";
//    print_matrix(Bm, size);
    std::cout << "1 thread: \n";
    auto start_1 = std::chrono::high_resolution_clock::now();
    one_thread_arr = multi(Am, Bm, size);
    auto end_1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> dur1 = (end_1 - start_1);
    std::cout << "Time: " << dur1.count() << " milliseconds\n\n";
//    print_matrix(one_thread_arr, size);
    std::cout << "8 threads: \n";
    auto start_2 = std::chrono::high_resolution_clock::now();
    multiple_threads_arr = multiMP(Am, Bm, size);
    auto end_2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> dur2 = (end_2 - start_2);
    std::cout << "Time: " << dur2.count() << " milliseconds\n\n";
    if (is_equal_matrix(one_thread_arr, multiple_threads_arr, size)) {
        std::cout << "Matrix is equal\n";
    } else {
        std::cout << "Matrix not equal\n";
    }

//    print_matrix(multiple_threads_arr, size);

    for (int i = 0; i < size; i++) {
        delete[] Am[i];
        delete[] Bm[i];
        delete[] one_thread_arr[i];
        delete[] multiple_threads_arr[i];
    }

    delete[] Am;
    delete[] Bm;
    delete[] one_thread_arr;
    delete[] multiple_threads_arr;
//    }

    return 0;
}
