//
// Created by zps on 23-11-20.
//

#include <iostream>
#include <memory>

#include <opencv2/opencv.hpp>
#include <glog/logging.h>
#include <NvInfer.h>
#include <NvOnnxParser.h>

#include <google_logger/google_logger.h>

class Logger : public nvinfer1::ILogger
{
    void log(Severity severity, const char* msg) noexcept override
    {
        // suppress info-level messages
        if (severity <= Severity::kWARNING)
            std::cout << msg << std::endl;
    }
} logger;

int main(int argc, char* argv[]) {
    armor_auto_aim::google_log::initGoogleLogger(argc, argv);
    LOG(INFO) << "Hello TensorRT and start!\n";
    std::string model_filename("../model/model-opt-4.onnx");
    // 构建器的实例
    nvinfer1::IBuilder *builder = nvinfer1::createInferBuilder(logger);
    LOG(INFO) << "builder success!\n";
//    std::unique_ptr<nvinfer1::IBuilder> builder(nvinfer1::createInferBuilder(logger));
    // 网络实例
    uint32_t flag = 1U << static_cast<uint32_t>(nvinfer1::NetworkDefinitionCreationFlag::kEXPLICIT_BATCH);
    nvinfer1::INetworkDefinition* network = builder->createNetworkV2(flag);
    LOG(INFO) << "Network success!\n";
    // 解析器
    nvonnxparser::IParser* parser = nvonnxparser::createParser(*network, logger);
    LOG(INFO) << "Parse success!\n";
    // 读取模型文件, 处理任何错误
    parser->parseFromFile(model_filename.c_str(),
                          static_cast<int32_t>(nvinfer1::ILogger::Severity::kWARNING));
    for (int32_t i = 0; i < parser->getNbErrors(); ++i) {
        std::cout << parser->getError(i)->desc() << std::endl;
    }
    LOG(INFO) << "Parse model success!\n";
    // 创建构建配置，指定 TensorRT 如何优化模型
    nvinfer1::IBuilderConfig* config = builder->createBuilderConfig();
    config->setMemoryPoolLimit(nvinfer1::MemoryPoolType::kWORKSPACE, 1U << 20);
    LOG(INFO) << "Config TensorRT success!\n";
    // 构建引擎
    nvinfer1::IHostMemory* serializedModel = builder->buildSerializedNetwork(*network, *config);
    LOG(INFO) << "<?> success!\n";
    // 反序列化
    nvinfer1::IRuntime* runtime = nvinfer1::createInferRuntime(logger);
    LOG(INFO) << "Infer Runtime success!\n";
//    nvinfer1::ICudaEngine* engine = runtime->deserializeCudaEngine(modelData, modelSize);
//    // 执行推理
//    nvinfer1::IExecutionContext *context = engine->createExecutionContext();
//    //
//    context->setTensorAddress(INPUT_NAME, inputBuffer);
//    context->setTensorAddress(OUTPUT_NAME, outputBuffer);
//    // 使用CUDA流推理
//    context->enqueueV3(stream);


    return 0;
}
