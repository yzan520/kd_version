cmake_minimum_required(VERSION 3.15)
project(my_test)
set(CMAKE_CXX_STANDARD 20)

# find required opencv,glog
find_package(glog REQUIRED)
find_package(CUDA REQUIRED)
find_package(OpenCV REQUIRED) # 使用find_package获取opencv的相关信息，如lib,include,verison等
# tensorrt
set(TENSORRT_ROOT_PATH /opt/TensorRT-8.6.1.6) # 根目录
set(TENSORRT_INCLUDE_DIRS ${TENSORRT_ROOT_PATH}/include/) # 头文件夹
set(TENSORRT_LIB_PATH ${TENSORRT_ROOT_PATH}/lib/)
set(TENSORRT_LIB ${TENSORRT_ROOT_PATH}/lib/libnvinfer.so ${TENSORRT_ROOT_PATH}/lib/libnvonnxparser.so) # 依赖动态库
# cuda
set(CUDA_ROOT_PATH /usr/local/cuda-11.8/) #根目录
set(CUDA_INCLUDE_DIRS ${CUDA_ROOT_PATH}/include/) # 头文件夹
set(CUDA_LIB_PATH ${CUDA_ROOT_PATH}/lib/)
set(CUDA_LIBS ${CUDA_ROOT_PATH}lib64/libcudart.so) # 依赖动态库

add_subdirectory(src/google_logger)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.cpp src/*.h)

#add_executable(my_test ${sources} src/detector/include/TRTModule/TRTModule.h)
add_executable(my_test ${sources})
target_include_directories(my_test PUBLIC
        src/detector/include
        )

target_include_directories(${PROJECT_NAME} PUBLIC
        ${TENSORRT_INCLUDE_DIRS}
        ${CUDA_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
        )
# link
target_link_directories(${PROJECT_NAME} PUBLIC
        ${TENSORRT_LIB_PATH}
        ${CUDA_LIB_PATH}
        )
target_link_libraries(${PROJECT_NAME} PUBLIC
        google_logger
        glog::glog
        ${TENSORRT_LIB}
        ${CUDA_LIBS}
        ${OpenCV_LIBS}
        )

#set(SAMPLES_COMMON_SOURCES "/home/zps/TensorRT_learning/TensorRT/samples/common/logger.cpp")
#add_executable(TensorRT_test sampleMNIST.cpp ${SAMPLES_COMMON_SOURCES})

## add TensorRT8
#include_directories(/home/zps/TensorRT_learning/TensorRT/include)
#include_directories(/home/zps/TensorRT_learning/TensorRT/samples/common)
#set(TENSORRT_LIB_PATH "/opt/TensorRT-8.6.1.6/lib")
#file(GLOB LIBS "${TENSORRT_LIB_PATH}/*.so")

## add CUDA
#message("CUDA_LIBRARIES:${CUDA_LIBRARIES}")
#message("CUDA_INCLUDE_DIRS:${CUDA_INCLUDE_DIRS}")
#include_directories(${CUDA_INCLUDE_DIRS})
