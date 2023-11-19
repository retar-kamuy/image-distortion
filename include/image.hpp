#ifndef IMAGE_HPP_
#define IMAGE_HPP_
#include <string>

#include <opencv2/opencv.hpp>

class Image {
 private:
    cv::Mat m_mat;
 public:
    explicit Image(const std::string &filepath) {
        m_mat = imread(filepath);
    }

    int cols(void) {
        return m_mat.cols;
    }

    int rows(void) {
        return m_mat.rows;
    }

    cv::Vec3b get_pixel(int x, int y) {
        cv::Mat3b ptImg = m_mat;
        return ptImg(cv::Point(x, y));
    }

    void set_pixel(int x, int y, cv::Vec3b pixel) {
        m_mat.at<cv::Vec3b>(y, x) = pixel;
    }

    void resize(double f) {
        cv::Mat dst;
        cv::resize(m_mat, dst, cv::Size(), f, f);
        m_mat = dst;
    }

    cv::Mat imread(const std::string &filepath);
    void write(const std::string &filepath) {
        imwrite(filepath, m_mat);
    }
    void imwrite(const std::string &filepath, cv::Mat img);
    void matToUChar(const cv::Mat &src, unsigned char *dst);
    void uCharToMat(int src_width, int src_height, unsigned char *src, const cv::Mat &dst);
};

#endif  // IMAGE_HPP_
