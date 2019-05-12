//
// Created by redwan on 5/1/19.
//

#ifndef TASE_EXP_MOTIONPRIMITIVES_H
#define TASE_EXP_MOTIONPRIMITIVES_H




#include <ros/ros.h>
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/PoseStamped.h>
#include "StateEstimator.h"


class MotionPrimitives: public StateEstimator {
    friend class TrajPlanner;
public:
    MotionPrimitives();

    virtual ~MotionPrimitives();

    void localizationCallback(const geometry_msgs::TransformStamped::ConstPtr &msg);

    // Go to target location
    void goTo(double x, double y);
    void publishGoal();



private:
    double yaw, setX, setY, oX,oY;
    float err_viz_, xy_tolarance_;
    int nap_rate_, repeat_;
    ros::Publisher pub_;
    ros::NodeHandle nh_;
    std::once_flag fix_origin_;


};


#endif //TASE_EXP_MOTIONPRIMITIVES_H
