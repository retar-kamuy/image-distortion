#ifndef INVERSE_DISTORTION_HPP_
#define INVERSE_DISTORTION_HPP_

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

#include "matplotlibcpp.hpp"
#include "image.hpp"

namespace plt = matplotlibcpp;

class InverseDistortion {
 private:
    float k_1;

 public:
    InverseDistortion(void) {
        this->k_1 = 0.15;
    }

    explicit InverseDistortion(float k_1) {
        this->k_1 = k_1;
    }

    ~InverseDistortion(void) {}

    bool is_zero(float a);
    float sigma_1(float x, float y);
    float sigma_2(float x, float y);
    void inverse_distortion(
        float x, float y, float *u_x, float *u_y);
    cv::Mat inverse_distortion(cv::Mat img);
    void plot(float spacing);
};

#endif  // INVERSE_DISTORTION_HPP_
