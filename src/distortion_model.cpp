#include "image.hpp"
#include "distortion_model.hpp"

float DistortionModel::distortion_x(float x, float y) {
    float pow_x = std::pow(x, 2.0);
    float pow_y = std::pow(y, 2.0);

    return this->p_2 * (3.0 * pow_x + pow_y)
        + x * (this->k_2 * (pow_x + pow_y) * (pow_x + pow_y)
        + this->k_1 * (pow_x + pow_y) + 1.0)
        + 2.0 * this->p_1 * x * y;
}

float DistortionModel::distortion_y(float x, float y) {
    float pow_x = std::pow(x, 2.0);
    float pow_y = std::pow(y, 2.0);

    return this->p_1 * (pow_x + 3.0 * pow_y)
        + y * (this->k_2 * (pow_x + pow_y) * (pow_x + pow_y)
        + this->k_1 * (pow_x + pow_y) + 1.0)
        + 2.0 * this->p_2 * x * y;
}

cv::Mat DistortionModel::distortion(cv::Mat img) {
    Image resized_img(img);
    Image dst(img);

    double max_distortion_x = this->distortion_x(1.0, 1.0);
    double max_distortion_y = this->distortion_y(1.0, 1.0);
    resized_img.resize(max_distortion_x, max_distortion_y);

    float offset_x = static_cast<float>(img.cols) / 2.0;
    float offset_y = static_cast<float>(img.rows) / 2.0;

    for (int i = 0; i < img.rows; i++) {
        float y = (static_cast<float>(i) - offset_y) / offset_y;
        for (int j = 0; j < img.cols; j++) {
            float x = (static_cast<float>(j) - offset_x) / offset_x;
            float d_x = this->distortion_x(x, y);
            float d_y = this->distortion_y(x, y);

            float re_ofst_x = static_cast<float>(resized_img.cols) / 2.0;
            float re_ofst_y = static_cast<float>(resized_img.rows) / 2.0;

            int d_i = static_cast<int>(d_y * offset_y)
                    + static_cast<int>(re_ofst_y);
            int d_j = static_cast<int>(d_x * offset_x)
                    + static_cast<int>(re_ofst_x);

            cv::Vec3b distortion_pixel = resized_img.get_pixel(d_j, d_i);
            dst.set_pixel(j, i, distortion_pixel);
        }
    }
    return dst.get_img();
}

void DistortionModel::plot_distortion(float spacing) {
    int plot_number = static_cast<int>(2.0 / spacing) + 2;

    std::vector<float> x, y;
    std::vector<float> u, v;

    float row = -1.0;
    float col = -1.0;
    for (int i = 0; i < plot_number; i++) {
        col = -1.0;
        for (int j = 0; j < plot_number; j++) {
            float d_x = distortion_x(col, row);
            float d_y = distortion_y(col, row);
            x.push_back(col);
            y.push_back(row);
            u.push_back(d_x - col);
            v.push_back(d_y - row);
            col += spacing;
        }
        row += spacing;
    }

    plt::xlim(-1.75, 1.75);
    plt::ylim(-1.75, 1.75);
    plt::quiver(x, y, u, v);
    plt::scatter(x, y, 30.0, {{"c", "blue"}});
    plt::show();
}
