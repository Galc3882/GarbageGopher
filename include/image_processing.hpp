#ifndef IMAGE_PROCESSING_HPP
#define IMAGE_PROCESSING_HPP

#include <opencv2/opencv.hpp>

namespace ImageProcessing {

    // Preprocesses the image for ML models: resizes, normalizes, and other enhancements.
    cv::Mat preprocessForModel(const cv::Mat& input, int targetWidth, int targetHeight);

    // Enhances image quality: adjusts brightness, contrast, and applies denoising.
    cv::Mat enhanceImageQuality(const cv::Mat& input);

    // Draws bounding boxes on the image based on detection results.
    cv::Mat drawBoundingBoxes(const cv::Mat& input, const std::vector<cv::Rect>& boxes);

    // Highlight the path that the robot plans to take (for instance, using a line).
    cv::Mat highlightPath(const cv::Mat& input, const std::vector<cv::Point>& pathPoints);

};

#endif // IMAGE_PROCESSING_HPP
