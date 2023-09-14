#include "localization.hpp"
#include <gtsam/slam/BetweenFactor.h>
#include <gtsam/slam/PriorFactor.h>
#include <gtsam/nonlinear/LevenbergMarquardtOptimizer.h>

namespace Localization
{

    RobotLocalization::RobotLocalization()
    {
        // Set an initial guess for the robot's pose. Assuming the robot starts at (0,0,0)
        initialEstimate.insert(gtsam::Symbol('x', 0), gtsam::Pose2(0.0, 0.0, 0.0));
    }

    void RobotLocalization::updatePose(const gtsam::Pose2 &odometry, const gtsam::Pose2 &measurement)
    {
        static int poseKey = 0;
        poseKey++;

        // Add odometry factor
        graph.emplace_shared<gtsam::BetweenFactor<gtsam::Pose2>>(gtsam::Symbol('x', poseKey - 1), gtsam::Symbol('x', poseKey), odometry);

        // Assume there's some noise in the odometry and measurement
        gtsam::noiseModel::Diagonal::shared_ptr noise = gtsam::noiseModel::Diagonal::Sigmas(gtsam::Vector3(0.2, 0.2, 0.1));

        // Add measurement factor
        graph.emplace_shared<gtsam::PriorFactor<gtsam::Pose2>>(gtsam::Symbol('x', poseKey), measurement, noise);

        // Update the current pose estimate using the optimizer
        gtsam::LevenbergMarquardtOptimizer optimizer(graph, initialEstimate);
        currentEstimate = optimizer.optimize();

        // Update initial estimate for the next iteration
        initialEstimate = currentEstimate;
    }

    gtsam::Pose2 RobotLocalization::getCurrentPose() const
    {
        return currentEstimate.at<gtsam::Pose2>(gtsam::Symbol('x', 0));
    }

} // namespace Localization
