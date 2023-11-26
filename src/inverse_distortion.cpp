#include "image.hpp"
#include "distortion_model.hpp"
#include "inverse_distortion.hpp"

float InverseDistortion::sigma_1(float x, float y) {
    float pow_x = std::pow(x, 2.0);
    float pow_y = std::pow(y, 2.0);
    float pow_r = pow_x + pow_y;

    float p = - pow_y / (3.0 * this->k_1 * pow_r);

    float s_2 = this->sigma_2(x, y);

    // std::cout << ", P = " << p << ", s_1 = " << s_2 + p / s_2 << std::endl;
    return s_2 + p / s_2;
}

float InverseDistortion::sigma_2(float x, float y) {
    float pow_x = std::pow(x, 2.0);
    float pow_y = std::pow(y, 2.0);
    float pow_r = pow_x + pow_y;

    float pow_y_3 = std::pow(y, 3.0);
    float q = pow_y_3 / (2.0 * this->k_1 * pow_r);

    float pow_y_6 = pow_y_3 * pow_y_3;
    float minus_d = pow_y_6
        / (27.0 * std::pow(this->k_1, 3.0) * std::pow(pow_r, 3.0))
        + pow_y_6 / (4.0 * std::pow(this->k_1, 2.0) * pow_r * pow_r);

    float s_2;
    if (q >= 0) {
        s_2 = std::cbrt(q + std::sqrt(minus_d));
    } else {
        s_2 = std::cbrt(q - std::sqrt(minus_d));
    }
    // std::cout << "x = " << x << ", y = " << y;
    // std::cout << ", q = " << q << ", -D = " << minus_d << ", s_2 = " << s_2;
    return s_2;
}

void InverseDistortion::inverse_distortion(
    float x, float y, float *u_x, float *u_y
) {
    if (fabs(x) <= FLT_EPSILON && fabs(y) <= FLT_EPSILON) {
        *u_x = 0.0;
        *u_y = 0.0;
    } else if (fabs(y) <= FLT_EPSILON) {
        *u_x = this->sigma_1(y, x);
        *u_y = 0.0;
    } else {
        *u_y = this->sigma_1(x, y);
        *u_x = x / y * *u_y;
    }
}

cv::Mat InverseDistortion::inverse_distortion(cv::Mat img) {
    Image dst(img);

    float offset_x = static_cast<float>(img.cols) / 2.0;
    float offset_y = static_cast<float>(img.rows) / 2.0;

    for (int i = 0; i < img.rows; i++) {
        float y = (static_cast<float>(i) - offset_y) / offset_y;
        for (int j = 0; j < img.cols; j++) {
            float x = (static_cast<float>(j) - offset_x) / offset_x;
            float d_y, d_x;
            this->inverse_distortion(x, y, &d_x, &d_y);

            int d_i = static_cast<int>(d_y * offset_y)
                    + static_cast<int>(offset_y);
            int d_j = static_cast<int>(d_x * offset_x)
                    + static_cast<int>(offset_x);

            cv::Vec3b distortion_pixel = get_pixel(d_j, d_i, img);
            dst.set_pixel(j, i, distortion_pixel);
        }
    }
    // dst.resize(1.0 / max_distortion_x, 1.0 / max_distortion_y);
    return dst.get_img();
}

void InverseDistortion::plot(float spacing) {
    float k_2 = 0.0;
    float p_1 = 0.0;
    float p_2 = 0.0;

    DistortionModel mdl(this->k_1, k_2, p_1, p_2);

    int plot_number = static_cast<int>(2.0 / spacing) + 2;

    std::vector<float> x, y;
    std::vector<float> u, v;

    float row = -1.0;
    float col = -1.0;
    for (int i = 0; i < plot_number; i++) {
        col = -1.0;
        for (int j = 0; j < plot_number; j++) {
            float d_x = mdl.distortion_x(col, row);
            float d_y = mdl.distortion_y(col, row);

            float u_y, u_x;
            this->inverse_distortion(d_x, d_y, &u_x, &u_y);
            // std::cout << "u_x = " << u_x << ", u_y = " << u_y << std::endl;
            x.push_back(d_x);
            y.push_back(d_y);
            u.push_back(u_x);
            v.push_back(u_y);
            col += spacing;
        }
        row += spacing;
    }

    plt::xlim(-1.75, 1.75);
    plt::ylim(-1.75, 1.75);
    // plt::quiver(x, y, u, v, {{"angles", "xy"}, {"scale_units", "xy"}, {"scale", "1"}});
    plt::scatter(x, y, 20.0, {{"c", "blue"}});
    plt::scatter(u, v, 20.0, {{"c", "red"}});
    plt::show();
}
