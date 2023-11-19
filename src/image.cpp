#include "image.hpp"

cv::Mat Image::imread(const std::string &filepath) {
    cv::Mat orginalImage = cv::imread(filepath, cv::IMREAD_UNCHANGED);
    std::cout << "Read '" << filepath
              << "'(" << orginalImage.rows << " x " << orginalImage.cols << ")"
              << std::endl;

    return orginalImage;
}

void Image::imwrite(const std::string &filepath, cv::Mat img) {
    cv::imwrite(filepath, img);
    std::cout << "Write '" << filepath
              << "'(" << img.rows << " x " << img.cols << ")"
              << std::endl;
}

void Image::matToUChar(const cv::Mat &src, unsigned char *dst) {
    for (int col = 0; col < src.cols; col++) {
        for (int row = 0; row < src.rows; row++) {
            // mat.dat is 3 channels(Blue, Green, Red)
            // dst is stored Blue channel only
            dst[col * src.rows + row] = src.data[col * (src.rows) + row];
        }
    }
}

void Image::uCharToMat(int src_width, int src_height, unsigned char *src, const cv::Mat &dst) {
    for (int i = 0; i < src_width; i++) {
        int src_col = i * src_height;
        int dst_col = i * src_height * 3;
        for (int j = 0; j < src_height; j++) {
            // 3 channels of mat.data are stored same data
            // dst is gray scale image
            for (int k = 0; k < 3; k++) {
                dst.data[dst_col + j * 3 + k] = src[src_col + j];
            }
        }
    }
}

// int main(void) {
// 
//     cv::Mat grayScaled;
//     cv::cvtColor(originalImage, grayScaled, cv::COLOR_RGB2GRAY);
// 
//     cv::Mat paddingImage;
//     padding(grayScaled, &paddingImage, 1, 1, 1, 1);
// 
//     unsigned char paddingData[paddingImage.cols * paddingImage.rows];
//     matToUChar(paddingImage, paddingData);
// 
//     unsigned char filteredData[grayScaled.cols * grayScaled.rows];
//     medianFilter(paddingData, grayScaled.cols, grayScaled.rows, filteredData);
//     std::cout << "Median filter end" << std::endl;
// 
//     cv::Mat filteredImage;
//     originalImage.copyTo(filteredImage);
//     uCharToMat(grayScaled.cols, grayScaled.rows, filteredData, filteredImage);
// 
//     imwrite("Lenna_output.png", filteredImage);
// 
//     return 0;
// }