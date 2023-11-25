//
// Created by zps on 23-11-24.
//

#ifndef MY_TEST_AUTOAIM_H
#define MY_TEST_AUTOAIM_H

#include <array>
#include <opencv2/opencv.hpp>
#include <TRTModule/TRTModule.h>

struct Detection_pack {
    /*/
     * 打包数据结构，将识别结果，对应的图像，陀螺仪，和时间戳对应
     */
    std::vector<bbox_t> dection;
    cv::Mat img;
    std::array<double, 4> q;
    double timestamp;
};

#endif //MY_TEST_AUTOAIM_H
