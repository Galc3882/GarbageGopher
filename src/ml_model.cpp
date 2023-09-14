#include "ml_model.hpp"

MLModel::MLModel() {}

MLModel::~MLModel() {}

bool MLModel::loadUODDM(const std::string &checkpoint_path, const std::string &cfg_file)
{
    try
    {
        uodd_model = torch::jit::load(checkpoint_path);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error loading the UODDM model: " << e.what() << std::endl;
        return false;
    }
    return true;
}

bool MLModel::loadMiDaS(const std::string &weight_path)
{
    try
    {
        midas_model = torch::jit::load(weight_path);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error loading the MiDaS model: " << e.what() << std::endl;
        return false;
    }
    return true;
}

std::vector<std::pair<std::string, cv::Rect>> MLModel::detectObjects(const std::string &image_path)
{
    std::vector<std::pair<std::string, cv::Rect>> detected_objects;

    cv::Mat image = cv::imread(image_path);
    torch::Tensor tensor_image = torch::from_blob(image.data, {image.rows, image.cols, 3}, torch::kFloat32);
    tensor_image = tensor_image.permute({2, 0, 1});
    tensor_image = tensor_image.unsqueeze(0); // Add batch dimension

    auto output = uodd_model.forward({tensor_image}).toTuple();
    // output structure: [[class_indices], [bounding_boxes]]
    auto class_indices = output->elements()[0].toIntList();
    auto boxes = output->elements()[1].toTensor(); // tensor of shape [N, 4] for N bounding boxes

    for (size_t i = 0; i < class_indices.size(); ++i)
    {
        int x = boxes[i][0].item<int>();
        int y = boxes[i][1].item<int>();
        int w = boxes[i][2].item<int>() - x;
        int h = boxes[i][3].item<int>() - y;

        detected_objects.push_back({std::to_string(class_indices[i]), cv::Rect(x, y, w, h)});
    }

    return detected_objects;
}

cv::Mat MLModel::getDepthPerception(const cv::Mat *image_ptr)
{
    torch::Tensor tensor_image = torch::from_blob(image_ptr->data, {image_ptr->rows, image_ptr->cols, 3}, torch::kFloat32);
    tensor_image = tensor_image.permute({2, 0, 1});
    tensor_image = tensor_image.unsqueeze(0); // Add batch dimension

    auto depth_tensor = midas_model.forward({tensor_image}).toTensor();

    // Convert the tensor to a cv::Mat
    cv::Mat depth_map(image_ptr->rows, image_ptr->cols, CV_32FC1, depth_tensor.data_ptr<float>());

    return depth_map;
}
