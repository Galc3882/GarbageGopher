#include <onnxruntime_cxx_api.h>
#include <opencv2/opencv.hpp>

// function to load onnx model
Ort::Session LoadModel(const char *model_path)
{
    Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "log");
    Ort::SessionOptions session_options;
    session_options.SetIntraOpNumThreads(1);
    session_options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_EXTENDED);
    Ort::Session session(env, model_path, session_options);
    return session;
}

// forward run onnx model on input image and return output image in gpu memory
cv::cuda::GpuMat RunModel(Ort::Session &session, cv::cuda::GpuMat &input)
{
    // get input image size
    int width = input.cols;
    int height = input.rows;
    int channels = input.channels();

    // create input tensor object from input image
    Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeCPU);
    std::vector<int64_t> input_node_dims = {1, channels, height, width}; // batch size 1, 3 channels, width, height
    Ort::Value input_tensor = Ort::Value::CreateTensor<float>(memory_info, input.ptr<float>(), input.total(), input_node_dims.data(), input_node_dims.size());

    // create output tensor object
    std::vector<const char *> output_node_names = {"output"};
    std::vector<int64_t> output_node_dims = {1, 1, height, width}; // batch size 1, 1 channel, width, height
    Ort::Value output_tensor = Ort::Value::CreateTensor<float>(memory_info, output_node_dims.data(), output_node_dims.size());

    // forward run onnx model
    session.Run(Ort::RunOptions{nullptr}, input_node_names.data(), &input_tensor, 1, output_node_names.data(), &output_tensor, 1);

    // get output image from output tensor
    cv::cuda::GpuMat output(height, width, CV_32FC1, output_tensor.GetTensorMutableData<float>(), cv::cuda::StreamAccessor::getStream(cv::cuda::Stream::Null()));
    return output;
}

// camera streaming pipeline https://github.com/JetsonHacksNano/CSI-Camera/blob/master/simple_camera.cpp
std::string gstreamer_pipeline(int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method)
{
    return "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)" + std::to_string(capture_width) + ", height=(int)" +
           std::to_string(capture_height) + ", framerate=(fraction)" + std::to_string(framerate) +
           "/1 ! nvvidconv flip-method=" + std::to_string(flip_method) + " ! video/x-raw, width=(int)" + std::to_string(display_width) + ", height=(int)" +
           std::to_string(display_height) + ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}

// initialize camera
cv::VideoCapture initializeCamera()
{
    int capture_width = 1280;
    int capture_height = 720;
    int framerate = 30;
    int flip_method = 2;

    std::string pipeline = gstreamer_pipeline(capture_width,
                                              capture_height,
                                              display_width,
                                              display_height,
                                              framerate,
                                              flip_method);

    cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER);
    return cap;
}

// get input image from camera
cv::Mat getCameraImage(cv::VideoCapture &cap)
{
    cv::Mat img;
    cap.read(img);
    return img;
}

// destroy camera
void destroyCamera(cv::VideoCapture &cap)
{
    cap.release();
}
