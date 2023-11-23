#include <iostream>
#include "image.hpp"
#include "distortion_model.hpp"

float pitch(int pixel) {
    return 2.0 / static_cast<float>(pixel);
}

int main(void) {
    std::string filepath = "data/Lenna.png";
    float k_1 = 0.15;
    float k_2 = 0.0;
    float p_1 = 0.0;
    float p_2 = 0.0;

    DistortionModel mdl(k_1, k_2, p_1, p_2);
    // mdl.plot_distortion(0.2);

    Image img(filepath);

    cv::Mat dst = mdl.distortion(img.get_img());

    imwrite("Lenna_output.png", dst);

    return 0;
}
