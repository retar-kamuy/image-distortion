#include <iostream>
#include "image.hpp"
#include "distortion_model.hpp"
#include "inverse_distortion.hpp"

float pitch(int pixel) {
    return 2.0 / static_cast<float>(pixel);
}

int main(void) {
    std::string filepath = "data/Lenna.png";
    float k_1 = 0.15;
    float k_2 = 0.0;
    float p_1 = 0.0;
    float p_2 = 0.0;

    // DistortionModel mdl(k_1, k_2, p_1, p_2);
    // mdl.plot(0.2);

    Image img(filepath);

    // cv::Mat dst = mdl.distortion(img.get_img());

    // imwrite("Lenna_output.png", dst);

    InverseDistortion inv(k_1);
    // inv.plot(0.2);

    cv::Mat dst = inv.inverse_distortion_image(img.get_img());

    imwrite("Lenna_output.png", dst);

    DistortionModel mdl(k_1, k_2, p_1, p_2);

    cv::Mat dst_2 = mdl.distortion(dst);

    imwrite("Lenna_output_2.png", dst_2);

    int filter[512 * 512];
    mdl.get_distortion_filter(dst.cols, dst.rows, filter);

    return 0;
}
