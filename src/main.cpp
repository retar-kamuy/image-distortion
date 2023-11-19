#include <cmath>
#include <iostream>

#include "matplotlibcpp.hpp"
#include "image.hpp"

namespace plt = matplotlibcpp;

float distortion_x(float k_1, float k_2, float p_1, float p_2, float x, float y) {
    float pow_x = std::pow(x, 2.0);
    float pow_y = std::pow(y, 2.0);

    return p_2 * (3 * pow_x + pow_y) + x * (k_2 * (pow_x + pow_y) * (pow_x + pow_y) + k_1 * (pow_x + pow_y) + 1) + 2 * p_1 * x * y;
}

float distortion_y(float k_1, float k_2, float p_1, float p_2, float x, float y) {
    float pow_x = std::pow(x, 2.0);
    float pow_y = std::pow(y, 2.0);

    return p_1 * (pow_x + 3 * pow_y) + y * (k_2 * (pow_x + pow_y) * (pow_x + pow_y) + k_1 * (pow_x + pow_y) + 1) + 2 * p_2 * x * y;
}

float pitch(int pixel) {
    return 2.0 / float(pixel);
}

float max_distortion_value(float k_1, float k_2, float p_1, float p_2) {
    return distortion_x(k_1, k_2, p_1, p_2, 1.0, 1.0);
}

int main(void) {
    Image image("data/Lenna.png");
    Image resize_image("data/Lenna.png");

    float k_1 = 0.15;
    float k_2 = 0.0;
    float p_1 = 0.0;
    float p_2 = 0.0;

    float x = -1.0;
    float y = -1.0;

    float spacing = pitch(image.cols());

    double scale_factor = max_distortion_value(k_1, k_2, p_1, p_2);
    resize_image.resize(scale_factor);

    int offset = static_cast<int>((scale_factor - 1.0) * 0.5 * image.cols());
    // resize_image.write("Lenna_output.png");
    std::cout << "offset = " << offset << std::endl;

    std::vector<float> x_1, y_1;
    std::vector<float> x_2, y_2;
    int i = 0;
    int j = 0;
    while (x <= 1.0) {
        y = -1.0;
        j = 0;
        while (y <= 1.0) {
            x_1.push_back(x);
            y_1.push_back(y);

            float d_x = distortion_x(k_1, k_2, p_1, p_2, x, y);
            float d_y = distortion_y(k_1, k_2, p_1, p_2, x, y);
            int d_x_2 = static_cast<int>(d_x * static_cast<float>(image.cols() / 2) + static_cast<float>(resize_image.cols() / 2));
            int d_y_2 = static_cast<int>(d_y * static_cast<float>(image.rows() / 2) + static_cast<float>(resize_image.rows() / 2));
            std::cout << "distortion_axis = (" << d_x_2 << ", " << d_y_2 << ")" << std::endl;
            cv::Vec3b distortion_pixel = resize_image.get_pixel(d_x_2, d_y_2);

            image.set_pixel(i, j, distortion_pixel);

            x_2.push_back(distortion_x(k_1, k_2, p_1, p_2, x, y));
            y_2.push_back(distortion_y(k_1, k_2, p_1, p_2, x, y));

            // std::cout << "(" << x_2 << ", " << y_2 << ")" << std::endl;
            // std::cout << "(" << x << ", " << y << ") ";
            y += spacing;
            j++;
        }
        // std::cout << std::endl;
        x += spacing;
        i++;
    }

    image.write("Lenna_output.png");

    plt::scatter(x_1, y_1, 20.0, {{"c", "blue"}});
    plt::scatter(x_2, y_2, 20.0, {{"c", "red"}});
    plt::show();

    return 0;
}
