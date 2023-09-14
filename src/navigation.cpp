#include "navigation.hpp"
#include "ml_model.hpp"
#include "camera.hpp"
#include "path_planning.hpp"
#include "actuators.hpp"
#include "sensors.hpp"
#include "localization.hpp"

namespace Navigation
{

    RobotNavigator::RobotNavigator(int gridWidth, int gridHeight)
        : planner(gridWidth, gridHeight), robotState(RobotState::IDLE) {}

    bool RobotNavigator::detectObstacle()
    {
        // Here, use the ultrasonic sensor to check for nearby obstacles.
        Sensors::UltrasonicSensor sensor;
        return sensor.getDistance() < 20;
    }

    void RobotNavigator::moveToUser()
    {
        // Initialize camera and machine learning models
        Camera cam;
        MLModel model;
        cv::VideoCapture cap = cam.initializeCamera();

        model.loadUODDM("models/uoddm/v1_checkpoint.pth", "models/uoddm/v1_config.yaml");

        while (robotState == RobotState::MOVING_TO_USER)
        {
            // Get image from camera
            cv::Mat image = cam.getCameraImage(cap);

            // Run object detection
            auto detectedObjects = model.detectObjects(image);

            // Check if user is detected
            bool userDetected = false;
            cv::Rect userBoundingBox;
            for (auto &obj : detectedObjects)
            {
                if (obj.first == "person")
                {
                    userDetected = true;
                    userBoundingBox = obj.second;
                    break;
                }
            }

            if (userDetected)
            {
                if (userBoundingBox.x + userBoundingBox.width / 2 < image.cols / 2 - 10)
                {
                    actuator.turnLeft(50);
                }
                else if (userBoundingBox.x + userBoundingBox.width / 2 > image.cols / 2 + 10)
                {
                    actuator.turnRight(50);
                }
                else
                {
                    if (detectObstacle())
                    {
                        handleObstacle();
                    }
                    else
                    {
                        actuator.moveForward(50);
                    }
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        cam.destroyCamera(cap);
    }

    void RobotNavigator::moveToBase()
    {
        Localization::RobotLocalization localizer;
        gtsam::Pose2 currentPose = localizer.getCurrentPose();

        Point start(currentPose.x(), currentPose.y());
        Point goal(0, 0);

        std::vector<Point> path = planner.AStarSearch(start, goal);
        followPath(path);
    }

    void RobotNavigator::updateNavigation()
    {
        Localization::RobotLocalization localizer;
        gtsam::Pose2 updatedPose = localizer.getCurrentPose();
    }

    void RobotNavigator::followPath(const std::vector<Point> &path)
    {
        for (const Point &point : path)
        {
            if (detectObstacle())
            {
                handleObstacle();
            }
            else
            {
                actuator.moveForward(180);
            }
        }
    }

    void RobotNavigator::handleObstacle()
    {
        actuator.stop();

        // Try to find a new path around the obstacle
        Localization::RobotLocalization localizer;
        gtsam::Pose2 currentPose = localizer.getCurrentPose();

        Point start(currentPose.x(), currentPose.y());
        Point goal(0, 0);

        Point obstaclePoint(currentPose.x() + OBSTACLE_DISTANCE, currentPose.y());
        planner.setObstacle(obstaclePoint);

        std::vector<Point> newPath = planner.AStarSearch(start, goal);
        followPath(newPath);
    }

} // namespace Navigation
