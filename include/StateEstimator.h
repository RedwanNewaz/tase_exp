//
// Created by redwan on 5/1/19.
//

#ifndef TASE_EXP_STATEESTIMATOR_H
#define TASE_EXP_STATEESTIMATOR_H

#include <iostream>
#include <vector>
#include "ros/ros.h"
#include <tf2/utils.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Scalar.h>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/TransformStamped.h>
#include <memory>
#include <mutex>


class StateEstimator {

public:
    StateEstimator();
    std::vector<double> getCurrentBasePose();

    tf2::Quaternion getOrientation(double yaw);

    geometry_msgs::Quaternion getGoalOrientation();


protected:
    const double xy_goal_tolerence_ = 0.155;
    std::vector<double> robot_position;
    geometry_msgs::Quaternion robot_orientation;
    bool target_achieved;
    geometry_msgs::TransformStamped base_to_map_tf_;
    std::mutex mu;

};


#endif //TASE_EXP_STATEESTIMATOR_H
