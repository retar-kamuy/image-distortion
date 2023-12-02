#ifndef DISTORTION_MODEL_HPP_
#define DISTORTION_MODEL_HPP_

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

#include "matplotlibcpp.hpp"
#include "image.hpp"

namespace plt = matplotlibcpp;

class DistortionModel {
 private:
    float k_1;
    float k_2;
    float p_1;
    float p_2;

 public:
    DistortionModel(void) {
        this->k_1 = 0.15;
        this->k_2 = 0.0;
        this->p_1 = 0.0;
        this->p_2 = 0.0;
    }

    DistortionModel(float k_1, float k_2, float p_1, float p_2) {
        this->k_1 = k_1;
        this->k_2 = k_2;
        this->p_1 = p_1;
        this->p_2 = p_2;
    }

    ~DistortionModel(void) {}

    bool is_zero(float a);
    bool is_equal(float a, float b, float max_rel_diff);
    float distortion_x(float x, float y);
    float distortion_y(float x, float y);
    cv::Mat distortion(cv::Mat img);
    void get_distortion_filter(int width, int height, int *filter);
    void plot(float spacing);
};

#endif  // DISTORTION_MODEL_HPP_
