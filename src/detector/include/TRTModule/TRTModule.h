//
// Created by zps on 23-11-21.
//

#ifndef MY_TEST_TRTMODULE_H
#define MY_TEST_TRTMODULE_H

#include <opencv2/core.hpp>
#include <NvInfer.h>

struct alignas(4) bbox_t {
    cv::Point2f pts[4];
    float confidence;
    int color_id;
    int tag_id;

    bool operator==(const bbox_t&) const = default;
    bool operator!=(const bbox_t&) const = default;
};

/*/
 * 四点模型
 */
class TRTModule {
    static constexpr int TOPK_NUM = 128;
    static constexpr float KEEP_THRES = 0.1f;

public:
    explicit TRTModule(const std::string &onnx_file); //禁止隐式类型转换

    ~TRTModule();

    TRTModule(const TRTModule &) = delete;

    TRTModule operator=(const TRTModule &) = delete;

    std::vector<bbox_t>operator()(const cv::Mat &src) const;

private:
    void build_engine_from_onnx(const std::string &onnx_file);

    void build_engine_from_cache(const std::string &onnx_file);

    void cache_engine(const std::string &cache_file);

    nvinfer1::ICudaEngine* engine;
    nvinfer1::IExecutionContext* context;
    mutable void* device_buffer[2];
    float* output_buffer;
    cudaStream_t stream;
    int input_idx, output_idx;
    size_t input_sz, output_sz;
};
#endif //MY_TEST_TRTMODULE_H
