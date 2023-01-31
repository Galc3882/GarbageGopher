#pragma once
#ifndef CAMERA_HPP
#define CAMERA_HPP

// function to load onnx model
Ort::Session LoadModel(const char *model_path);

// forward run onnx model on input image and return output image in gpu memory
cv::cuda::GpuMat RunModel(Ort::Session &session, cv::cuda::GpuMat &input);

// camera streaming pipeline
std::string gstreamer_pipeline(int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method);

// initialize camera
cv::VideoCapture initializeCamera();

// get input image from camera
cv::Mat getCameraImage(cv::VideoCapture &cap);

// destroy camera
void destroyCamera(cv::VideoCapture &cap);

#endif // CAMERA_HPP