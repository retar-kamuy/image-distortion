#include "image.hpp"

Image::Image(const std::string &filepath) {
    this->img = cv::imread(filepath, cv::IMREAD_UNCHANGED);
    std::cout << "imread path: " << filepath << std::endl
              << "       size: " << this->img.cols << "x" << this->img.rows
              << std::endl;
}

void imwrite(const std::string &filepath, const cv::Mat img) {
    cv::imwrite(filepath, img);
    std::cout << "Write path: " << filepath << std::endl
              << "      size: " << img.cols << "x" << img.rows
              << std::endl;
}

cv::Vec3b get_pixel(int x, int y, const cv::Mat img) {
    cv::Mat3b ptImg = img;
    return ptImg(cv::Point(x, y));
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

void Image::uCharToMat(int src_width, int src_height,
    unsigned char *src, const cv::Mat &dst
) {
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
