//
// Created by redwan on 5/1/19.
//

#include "../include/StateEstimator.h"


StateEstimator::StateEstimator() {
    target_achieved = false;
    robot_position.resize(3);
    robot_position[0] = robot_position[1] = robot_position[2] =0.0;
}

std::vector<double> StateEstimator::getCurrentBasePose(){
    return robot_position;
}


tf2::Quaternion StateEstimator::getOrientation(double yaw){
    tf2::Quaternion q;
    q.setRPY(0.0, 0.0, yaw);
    q.normalize();
    ROS_INFO_STREAM(yaw<<"-->"<<q.getAxis());
    return q;
}

geometry_msgs::Quaternion StateEstimator::getGoalOrientation(){
    return robot_orientation;
}
