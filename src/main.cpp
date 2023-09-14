// main.cpp
#include <iostream>
#include "actuators.hpp"
#include "camera.hpp"
#include "image_processing.hpp"
#include "localization.hpp"
#include "ml_model.hpp"
#include "navigation.hpp"
#include "path_planning.hpp"
#include "sensors.hpp"
#include "pyheader.hpp"

int main()
{
    std::cout << "GarbageGopher Autonomous Garbage Bin Initialized" << std::endl;

    // Initializing components
    Camera camera;
    cv::VideoCapture cap = camera.initializeCamera();

    MLModel model;
    model.loadUODDM("./models/object_detection.model", "./models/cfg_file.cfg");
    model.loadMiDaS("./models/depth_perception.model");

    ServoActuator actuator;
    PyObject *servoKit = actuator.initServoKit();

    Navigation::RobotNavigator navigator(100, 100);
    Navigation::RobotState state = navigator.getRobotState();

    // Main loop
    while (true)
    {
        cv::Mat image = camera.getCameraImage(cap);
        cv::Mat enhancedImage = ImageProcessing::enhanceImageQuality(image);

        std::vector<std::pair<std::string, cv::Rect>> detections = model.detectObjects("./path_to_image");

        // Visualizing detected objects
        std::vector<cv::Rect> boxes;
        for (const auto &detection : detections)
        {
            boxes.push_back(detection.second);
        }
        cv::Mat imageWithBoxes = ImageProcessing::drawBoundingBoxes(enhancedImage, boxes);

        // Update navigation based on state
        switch (state)
        {
        case Navigation::RobotState::IDLE:
            std::cout << "Robot is idle." << std::endl;
            break;

        case Navigation::RobotState::MOVING_TO_USER:
            navigator.moveToUser();
            break;

        case Navigation::RobotState::WAITING_FOR_TRASH:
            // waiting for 10 seconds
            std::this_thread::sleep_for(std::chrono::seconds(10));
            navigator.setRobotState(Navigation::RobotState::RETURNING_TO_BASE);
            break;

        case Navigation::RobotState::RETURNING_TO_BASE:
            navigator.moveToBase();
            break;

        case Navigation::RobotState::AVOIDING_OBSTACLE:
            navigator.handleObstacle();
            break;
        }

        state = navigator.getRobotState();
    }

    camera.destroyCamera(cap);

    return 0;
}
