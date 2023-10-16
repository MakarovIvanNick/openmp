#include "opencv2/opencv.hpp"
#include <iostream>
#include <chrono>

int main() {
    #ifdef _OPENMP
        printf("OpenMP is supported\n");
    #endif
    for (int iter = 1; iter <= 10; iter++) {
    cv::Mat img1 = cv::imread("../src/city2560x1440.jpg");
    cv::Mat img2 = cv::imread("../src/river2560x1440.jpg");
    if (img1.empty() || img2.empty()) {
        printf("Images loading error\n");
    }
    auto start = std::chrono::high_resolution_clock::now();
    cv::Mat res = cv::Mat::zeros(img1.size(), CV_8UC1);
    #pragma omp parallel for
    for (int i = 0; i < img1.rows; i++){   
        for (int j = 0; j < img1.cols; j++){
            cv::Vec3b pixel1 = img1.at<cv::Vec3b>(i, j);
            cv::Vec3b pixel2 = img2.at<cv::Vec3b>(i, j);
            int intensity = (pixel1[2] + pixel1[1] + pixel1[0] + pixel2[2] + pixel2[1] + pixel2[0]) / 3;
            res.at<uchar>(i, j) = static_cast<uchar>(255 * intensity / 510);
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
    std::cout << "iter " << iter << " " << duration.count() << "\n";
    cv::imwrite("../res/ex"+std::to_string(iter)+".jpg", res);
    }
    return 0;
}