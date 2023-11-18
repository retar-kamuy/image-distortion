#include <cmath>
#include <iostream>

#include "matplotlibcpp.hpp"

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

int main(void) {
    float spacing = 0.2;
    float k_1 = 0.15;
    float k_2 = 0.0;
    float p_1 = 0.0;
    float p_2 = 0.0;

    float x = -1.0;
    float y = -1.0;

    std::vector<float> x_1, y_1;
    std::vector<float> x_2, y_2;
    while (x <= 1.0) {
        y = -1.0;
        while (y <= 1.0) {
            x_1.push_back(x);
            y_1.push_back(y);

            x_2.push_back(distortion_x(k_1, k_2, p_1, p_2, x, y));
            y_2.push_back(distortion_y(k_1, k_2, p_1, p_2, x, y));

            // std::cout << "(" << x_2 << ", " << y_2 << ")" << std::endl;
            std::cout << "(" << x << ", " << y << ") ";
            y += spacing;
        }
        std::cout << std::endl;
        x += spacing;
    }

    plt::scatter(x_1, y_1, 20.0, {{"c", "blue"}});
    plt::scatter(x_2, y_2, 20.0, {{"c", "red"}});
    plt::show();

    return 0;
}
