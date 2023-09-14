#include "image_processing.hpp"

namespace ImageProcessing
{

    cv::Mat preprocessForModel(const cv::Mat &input, int targetWidth, int targetHeight)
    {
        cv::Mat resized, normalized;

        // Resize
        cv::resize(input, resized, cv::Size(targetWidth, targetHeight));

        // Normalize to 0-1 range
        resized.convertTo(normalized, CV_32F, 1.0 / 255.0);

        return normalized;
    }

    cv::Mat enhanceImageQuality(const cv::Mat &input)
    {
        cv::Mat enhanced, temp;

        // Adjust brightness and contrast
        double alpha = 1.0; // Contrast control
        int beta = 10;      // Brightness control
        input.convertTo(enhanced, -1, alpha, beta);

        // Denoise
        cv::fastNlMeansDenoisingColored(enhanced, temp, 10, 10, 7, 21);
        enhanced = temp.clone();

        return enhanced;
    }

    cv::Mat drawBoundingBoxes(const cv::Mat &input, const std::vector<cv::Rect> &boxes)
    {
        cv::Mat withBoxes = input.clone();

        for (const auto &box : boxes)
        {
            cv::rectangle(withBoxes, box.tl(), box.br(), cv::Scalar(0, 255, 0), 2);
        }

        return withBoxes;
    }

    cv::Mat highlightPath(const cv::Mat &input, const std::vector<cv::Point> &pathPoints)
    {
        cv::Mat withPath = input.clone();

        if (pathPoints.size() > 1)
        {
            for (size_t i = 1; i < pathPoints.size(); ++i)
            {
                cv::line(withPath, pathPoints[i - 1], pathPoints[i], cv::Scalar(0, 0, 255), 2);
            }
        }

        return withPath;
    }

} // namespace ImageProcessing
