// navigation.hpp

#ifndef NAVIGATION_HPP
#define NAVIGATION_HPP

#include "actuators.hpp"
#include "sensors.hpp"
#include "localization.hpp"
#include "path_planning.hpp"
#include <vector>
#include <mutex>

namespace Navigation
{

    enum class RobotState
    {
        IDLE,
        MOVING_TO_USER,
        WAITING_FOR_TRASH,
        RETURNING_TO_BASE,
        AVOIDING_OBSTACLE
    };

    class RobotNavigator
    {
    public:
        RobotNavigator(int gridWidth, int gridHeight);

        // Main functionalities
        void moveToUser(); // Navigates the robot towards the user
        void moveToBase(); // Navigates the robot back to the base/station

        // Update the robot's position and path based on sensor readings and localization
        void updateNavigation();

        // Set robot state
        void setRobotState(RobotState state);

        // Get robot state
        RobotState getRobotState() const;

    private:
        PathPlanner planner;
        ServoActuator actuator;
        UltSensor::UltrasonicSensor ultrasonicSensor;
        Localization::RobotLocalization localization;

        RobotState robotState;
        std::vector<Point> currentPath; // Path the robot is currently following
        Point currentDestination;       // The robot's current destination point
        std::mutex navMutex;            // For thread-safe access to navigation data

        // Private helper functions
        void followPath(const std::vector<Point> &path); // Makes the robot follow a given path
        bool detectObstacle();                           // Checks if there's an obstacle in the robot's way
        void handleObstacle();                           // Defines the behavior when an obstacle is detected
    };

};

#endif // NAVIGATION_HPP
