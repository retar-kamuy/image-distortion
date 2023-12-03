#include "image.hpp"
#include "distortion_model.hpp"

bool DistortionModel::is_equal(
    float a, float b, float max_rel_diff = FLT_EPSILON
) {
    // Calculate the difference.
    float diff = fabs(a - b);
    a = fabs(a);
    b = fabs(b);
    // Find the largest
    float largest = (b > a) ? b : a;

    if (diff <= largest * max_rel_diff)
        return true;
    return false;
}

float DistortionModel::distortion_x(float x, float y) {
    float pow_x = std::pow(x, 2);
    float pow_y = std::pow(y, 2);
    float pow_r = pow_x + pow_y;

    float d_x = this->p_2 * (3.0 * pow_x + pow_y)
        + x * (this->k_2 * pow_r * pow_r
        + this->k_1 * pow_r + 1.0)
        + 2.0 * this->p_1 * x * y;
    return d_x;
}

float DistortionModel::distortion_y(float x, float y) {
    float pow_x = std::pow(x, 2);
    float pow_y = std::pow(y, 2);
    float pow_r = pow_x + pow_y;

    float d_y = this->p_1 * (pow_x + 3.0 * pow_y)
        + y * (this->k_2 * pow_r * pow_r
        + this->k_1 * pow_r + 1.0)
        + 2.0 * this->p_2 * x * y;
    return d_y;
}

cv::Mat DistortionModel::distortion(cv::Mat img) {
    // Image resized_img(img);
    Image dst(img);

    // double max_distortion_x = this->distortion_x(1.0, 1.0);
    // double max_distortion_y = this->distortion_y(1.0, 1.0);
    // resized_img.resize(max_distortion_x, max_distortion_y);

    float offset_x = static_cast<float>(img.cols) / 2.0;
    float offset_y = static_cast<float>(img.rows) / 2.0;

    for (int i = 0; i < img.rows; i++) {
        float y = (static_cast<float>(i) - offset_y) / offset_y;
        for (int j = 0; j < img.cols; j++) {
            float x = (static_cast<float>(j) - offset_x) / offset_x;
            float d_x = this->distortion_x(x, y);
            float d_y = this->distortion_y(x, y);

            int d_i = static_cast<int>(d_y * offset_y)
                    + static_cast<int>(offset_y);
            int d_j = static_cast<int>(d_x * offset_x)
                    + static_cast<int>(offset_x);

            if (d_i > -1 && d_j > -1 && d_j < img.cols && d_i < img.rows) {
                cv::Vec3b distortion_pixel = get_pixel(d_j, d_i, img);
                dst.set_pixel(j, i, distortion_pixel);
            } else {
                dst.set_pixel(j, i, cv::Vec3b(0, 0, 0));
            }
        }
    }
    // dst.resize(1.0 / max_distortion_x, 1.0 / max_distortion_y);
    return dst.get_img();
}


cv::Mat DistortionModel::distortion_sparse(cv::Mat img, int sparse) {
    // Image resized_img(img);
    Image dst(img);

    // double max_distortion_x = this->distortion_x(1.0, 1.0);
    // double max_distortion_y = this->distortion_y(1.0, 1.0);
    // resized_img.resize(max_distortion_x, max_distortion_y);

    float offset_x = static_cast<float>(img.cols) / 2.0;
    float offset_y = static_cast<float>(img.rows) / 2.0;

    float d_y;
    for (int i = 0; i < img.rows; i++) {
        int mod_i = i % sparse;

        float y = (static_cast<float>(i) - offset_y) / offset_y;
        float d_x;
        for (int j = 0; j < img.cols; j++) {
            int mod_j = j % sparse;

            float x = (static_cast<float>(j) - offset_x) / offset_x;
            float c_x, c_y;

            if (mod_j == sparse - 1) {
                c_x = this->distortion_x(x, y);
            } else if (mod_j == 0) {
                float s_x = static_cast<float>(sparse - 1) / offset_x;
                d_x = this->distortion_x(x + s_x, y);
                c_x = (d_x - x) * static_cast<float>(mod_j + 1) / static_cast<float>(sparse) + x;
            } else {
                c_x = (d_x - x) * static_cast<float>(mod_j + 1) / static_cast<float>(sparse) + x;
            }

            if (mod_i == sparse - 1) {
                c_y = this->distortion_y(x, y);
            } else if (mod_i == 0) {
                float s_y = static_cast<float>(sparse - 1) / offset_y;
                d_y = this->distortion_y(x, y + s_y);
                c_y = (d_y - y) * static_cast<float>(mod_i + 1) / static_cast<float>(sparse) + y;
            } else {
                c_y = (d_y - y) * static_cast<float>(mod_i + 1) / static_cast<float>(sparse) + y;
            }

            int d_i = static_cast<int>(c_y * offset_y)
                    + static_cast<int>(offset_y);
            int d_j = static_cast<int>(c_x * offset_x)
                    + static_cast<int>(offset_x);

            if (d_i > -1 && d_j > -1 && d_j < img.cols && d_i < img.rows) {
                cv::Vec3b distortion_pixel = get_pixel(d_j, d_i, img);
                dst.set_pixel(j, i, distortion_pixel);
            } else {
                dst.set_pixel(j, i, cv::Vec3b(0, 0, 0));
            }
        }
    }
    // dst.resize(1.0 / max_distortion_x, 1.0 / max_distortion_y);
    return dst.get_img();
}

void DistortionModel::get_distortion_filter(
    int width, int height, int *filter
) {
    float offset_x = static_cast<float>(width) / 2.0;
    float offset_y = static_cast<float>(height) / 2.0;

    for (int i = 0; i < height; i++) {
        float y = (static_cast<float>(i) - offset_y) / offset_y;
        for (int j = 0; j < width; j++) {
            float x = (static_cast<float>(j) - offset_x) / offset_x;
            float d_x = this->distortion_x(x, y);
            float d_y = this->distortion_y(x, y);

            int d_i = static_cast<int>(d_y * offset_y)
                    + static_cast<int>(offset_y);
            int d_j = static_cast<int>(d_x * offset_x)
                    + static_cast<int>(offset_x);

            if (d_i > -1 && d_j > -1 && d_j < width && d_i < height) {
                filter[i * width + j] = d_i * width + d_j;
            } else {
                filter[i * width + j] = height * width;
            }
        }
    }
}

void DistortionModel::plot(float spacing) {
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
