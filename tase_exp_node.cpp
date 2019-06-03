//
// Created by redwan on 5/1/19.
//
#include <ros/ros.h>
#include "../include/PomdpPolicy.h"
#include "../include/MotionPrimitives.h"
#include <thread>
#include <cassert>

//#include "examples/PolicyChecker/include/PolicyReader.h"
//#include "examples/PolicyChecker/include/ValidatePolicy.h"
//#include "examples/PolicyChecker/include/PomdpPolicyChecker.h"
//


void executePolicy(int argc, char *argv[], MotionPrimitives *move)
{

}



int main(int argc, char *argv[]){

    ros::init(argc,argv,"TASE_EXT");
    ROS_INFO("TASE EXPERIMENT STARTED");
    ros::NodeHandle nh;
    std::string topic_name;
    nh.getParam("/tase/topic_pub/vicon", topic_name);
    MotionPrimitives *move = new MotionPrimitives();
    PolicyExecution planner(move);
    ros::AsyncSpinner spinner(4); // Use 4 threads
    ros::Subscriber gazebo_model = nh.subscribe(topic_name, 100, &MotionPrimitives::localizationCallback, move);
    spinner.start();
    planner.start();
    ros::waitForShutdown();








    return 0;
}
