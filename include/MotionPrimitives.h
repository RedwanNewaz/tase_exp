//
// Created by redwan on 5/1/19.
//

#ifndef TASE_EXP_MOTIONPRIMITIVES_H
#define TASE_EXP_MOTIONPRIMITIVES_H


#include <iostream>
#include <condition_variable>
#include <mutex>
#include <map>
#include <string>
#include <ros/ros.h>
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/PoseStamped.h>
#include <actionlib/client/simple_action_client.h>
#include <control_msgs/PointHeadAction.h>
#include <control_msgs/PointHeadGoal.h>
//#include <move_base_msgs/MoveBaseAction.h>

#include "StateEstimator.h"

using namespace std;

class MotionPrimitives: public StateEstimator {
    friend class TrajPlanner;
    typedef actionlib::SimpleActionClient<control_msgs::PointHeadAction> point_head_action_client_t;
//    typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;


public:
    MotionPrimitives();

    virtual ~MotionPrimitives();

    void localizationCallback(const geometry_msgs::TransformStamped::ConstPtr &msg);

    // Go to target location
    void goTo(double x, double y);
    void lookAt(int z);
    void publishGoal();



private:
    double yaw, setX, setY, oX,oY;
    float pitch_, xy_tolarance_;
    int nap_rate_, repeat_;
    ros::Publisher pub_;
    ros::NodeHandle nh_;
    std::once_flag fix_origin_;
    condition_variable cond_;
    mutex mu_;
    enum OBS_TYPE{
        NORTH=5,
        SOUTH,
        WEST,
        EAST
    };
    map<OBS_TYPE,float>look_;
    boost::shared_ptr<point_head_action_client_t> point_head_action_client_;
//    boost::shared_ptr<MoveBaseClient>move_base_client_;



};


#endif //TASE_EXP_MOTIONPRIMITIVES_H
