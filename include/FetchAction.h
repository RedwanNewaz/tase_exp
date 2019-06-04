//
// Created by redwan on 6/3/19.
//

#ifndef TASE_EXP_FETCHACTION_H
#define TASE_EXP_FETCHACTION_H

#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include "tase_exp/fetchAction.h"
#include "BeliefState.h"
#include "MotionPrimitives.h"
#include "geometry_msgs/PoseArray.h"
#include "VizTraj.h"
using namespace std;
class FetchAction {
public:
    FetchAction(string name);
    void executeCB(const tase_exp::fetchGoalConstPtr &goal);

    virtual ~FetchAction();

private:
    ros::NodeHandle nh_;
    actionlib::SimpleActionServer<tase_exp::fetchAction> as_;
    std::string action_name_;
    // create messages that are used to published feedback/result
    tase_exp::fetchFeedback feedback_;
    tase_exp::fetchResult result_;
    BeliefState robot;
    MotionPrimitives *move;
    ros::Subscriber gazebo_model, obstacle_viz;
    VizTraj disp;

protected:
    void ObstacleCallback(const geometry_msgs::PoseArray::ConstPtr &msg);
};


#endif //TASE_EXP_FETCHACTION_H
