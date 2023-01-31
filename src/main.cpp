#include <iostream>
#include <chrono>
#include <thread>
#include <stdio.h>
#include <JetsonGPIO.h>
#include <Python.h>
#include <opencv2/opencv.hpp>
#include <sensors.hpp>
#include <actuators.hpp>
#include <camera.hpp>

void testLid()
{
    // Set the PYTHONPATH environment variable to the current directory
    setenv("PYTHONPATH", ".", 1);
    // Initialize the Python Interpreter
    Py_Initialize();

    // Initialize the ServoKit
    PyObject *kit = initServoKit();

    // Open the lid
    openLid(kit);

    // Wait 2 seconds
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Close the lid
    closeLid(kit);

    // Wait 2 seconds
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Finalize the Python Interpreter
    Py_Finalize();
}

// ultrasonic sensor test
void testUltrasonic()
{
    // create ultrasonic sensor object
    UltSensor::UltrasonicSensor ultrasonicSensor = UltSensor::UltrasonicSensor();

    // open new thread for reading
    std::thread ultrasonicThread(&UltSensor::UltrasonicSensor::ultrasonicDistanceThread, &ultrasonicSensor);

    for (int i = 0; i < 60; i++)
    {
        // print distance
        // std::cout << ultrasonicSensor.getDistance() << std::endl;
        ultrasonicSensor.printDebug();
        // sleep for 1/4 second
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    // stop reading
    ultrasonicSensor.stopReading();
    // join thread
    ultrasonicThread.join();
}

int main(void)
{
    // testLid();
    // testUltrasonic();

    // initialize camera
    cv::VideoCapture cap = initializeCamera();

    // load onnx model
    Ort::Session session = LoadModel("model/model-f6b98070.onnx");

    // get input image from camera
    cv::Mat input = getCameraImage(cap);

    // convert input image to gpu memory
    cv::cuda::GpuMat inputGpu = cv::cuda::GpuMat(input);

    // run model on input image
    cv::cuda::GpuMat output = RunModel(session, input);

    // convert output image from gpu memory to cpu memory
    cv::Mat outputCpu = cv::Mat(output);

    // show output image
    cv::imshow("output", outputCpu);
    cv::waitKey(0);

    // destroy camera
    destroyCamera(cap);

    return 0;
}