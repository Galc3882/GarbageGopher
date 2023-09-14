#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <opencv2/opencv.hpp>

class Camera {
public:
    // Default constructor
    Camera(int capture_width = 1280, int capture_height = 720, 
           int display_width = 1280, int display_height = 720,
           int framerate = 30, int flip_method = 2);

    // Initialize the camera
    cv::VideoCapture initializeCamera();

    // Get an image from the camera
    cv::Mat getCameraImage(cv::VideoCapture &cap);

    // Release the camera resource
    void destroyCamera(cv::VideoCapture &cap);

private:
    // Private variables to hold camera configuration
    int capture_width_;
    int capture_height_;
    int display_width_;
    int display_height_;
    int framerate_;
    int flip_method_;

    // Helper function to build the GStreamer pipeline string
    std::string gstreamer_pipeline();
};

#endif // CAMERA_HPP
