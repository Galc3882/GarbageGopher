#ifndef LOCALIZATION_HPP
#define LOCALIZATION_HPP

#include <gtsam/nonlinear/NonlinearFactorGraph.h>
#include <gtsam/inference/Symbol.h>
#include <gtsam/geometry/Pose2.h>

namespace Localization {

class RobotLocalization {
public:
    RobotLocalization();

    // Updates the robot's pose based on movement and measurements
    void updatePose(const gtsam::Pose2& odometry, const gtsam::Pose2& measurement);

    // Fetches the current estimate of the robot's pose
    gtsam::Pose2 getCurrentPose() const;

private:
    gtsam::NonlinearFactorGraph graph;  // Factor graph for SLAM
    gtsam::Values initialEstimate;  // Initial pose estimate
    gtsam::Values currentEstimate;  // Updated pose estimate
};

};

#endif // LOCALIZATION_HPP
