//
// Created by zps on 23-11-24.
//

#include "autoaim.h"

#include <thread> // 包含与线程相关的类和函数，允许在程序中创建，控制和管理线程
#include <chrono> // 包含一系列与时间相关的函数，允许在程序中进行时间测量，延时等操作
#include <fmt/format.h>
#include <fmt/color.h>
#include <opencv2/opencv.hpp>
#include <pybind11/numpy.h> //用于支持numpy数组的交互
#include <string>
#include <cmath>

using namespace std::chrono;
namespace py = pybind11;

static bool debug = true;

void detection_run(const std::string &onnx_file) {
    /*/
     * 识別器
     */

    if(debug) std::cout << "========== detection_run =============" << std::endl;
    TRTModule model(onnx_file);

    int fps = 0, fps_count = 0;
    auto t1 = system_clock::now();
    int cnt_useless = -1;

    const cv::Scalar colors[3] = {{255, 0, 0 }, {0, 0, 255}, {0, 255, 0}};

    while(true) {
        try {
//            const auto& [img, q, timestamp] = sensor_sub.pop();
            auto detections = model(img);
        }
    }

}