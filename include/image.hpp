#ifndef IMAGE_HPP_
#define IMAGE_HPP_

#include <string>

#include <opencv2/opencv.hpp>

void imwrite(const std::string &filepath, const cv::Mat img);

class Image {
 private:
    cv::Mat img;

 public:
    int cols;
    int rows;

    Image(void) {}

    explicit Image(const cv::Mat img) {
        this->img = img.clone();
        this->cols = this->img.cols;
        this->rows = this->img.rows;
    }

    explicit Image(const std::string &filepath);

    ~Image(void) {}

    cv::Mat get_img(void) {
        return this->img;
    }

    void set_image(const cv::Mat img) {
        this->img = img;
    }

    cv::Vec3b get_pixel(int x, int y) {
        cv::Mat3b ptImg = this->img;
        return ptImg(cv::Point(x, y));
    }

    void set_pixel(int x, int y, cv::Vec3b pixel) {
        img.at<cv::Vec3b>(y, x) = pixel;
    }

    void resize(double fx, double fy) {
        cv::resize(this->img, this->img, cv::Size(), fx, fy);
        this->cols = img.cols;
        this->rows = img.rows;
        std::cout << "resize: " << this->cols << "x" << this->rows
                  << std::endl;
    }

    void matToUChar(const cv::Mat &src, unsigned char *dst);
    void uCharToMat(int src_width, int src_height,
        unsigned char *src, const cv::Mat &dst);
};

#endif  // IMAGE_HPP_
