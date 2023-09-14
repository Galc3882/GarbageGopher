#ifndef ML_MODEL_HPP
#define ML_MODEL_HPP

#include <string>
#include <vector>
#include <torch/torch.h>
#include <torch/script.h>
#include <opencv2/opencv.hpp>
#include <opencv2/cudaarithm.hpp>
#include <iostream>

class MLModel {
public:
    MLModel();
    ~MLModel();

    // Load UODDM model
    bool loadUODDM(const std::string& checkpoint_path, const std::string& cfg_file);

    // Load MiDaS model
    bool loadMiDaS(const std::string& weight_path);

    // Run object detection
    std::vector<std::pair<std::string, cv::Rect>> detectObjects(const std::string& image_path);

    // Get depth perception
    cv::Mat getDepthPerception(const cv::Mat* image_ptr);

private:
    torch::jit::script::Module uodd_model;
    torch::jit::script::Module midas_model;

};

#endif // ML_MODEL_HPP