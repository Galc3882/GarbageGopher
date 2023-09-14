#include "camera.hpp"

// Constructor to set default values for camera parameters
Camera::Camera(int capture_width, int capture_height,
               int display_width, int display_height,
               int framerate, int flip_method)
    : capture_width_(capture_width), capture_height_(capture_height),
      display_width_(display_width), display_height_(display_height),
      framerate_(framerate), flip_method_(flip_method)
{
    
}

// Helper function to build the GStreamer pipeline string
std::string Camera::gstreamer_pipeline()
{
    return "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)" + std::to_string(capture_width_) + ", height=(int)" +
           std::to_string(capture_height_) + ", framerate=(fraction)" + std::to_string(framerate_) +
           "/1 ! nvvidconv flip-method=" + std::to_string(flip_method_) + " ! video/x-raw, width=(int)" + std::to_string(display_width_) + ", height=(int)" +
           std::to_string(display_height_) + ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}

// Initialize the camera using the GStreamer pipeline and return the capture object
cv::VideoCapture Camera::initializeCamera()
{
    cv::VideoCapture cap(gstreamer_pipeline(), cv::CAP_GSTREAMER);
    return cap;
}

// Capture an image from the camera and return it
cv::Mat Camera::getCameraImage(cv::VideoCapture &cap)
{
    cv::Mat img;
    cap.read(img);
    return img;
}

// Release the camera and associated resources
void Camera::destroyCamera(cv::VideoCapture &cap)
{
    cap.release();
}
