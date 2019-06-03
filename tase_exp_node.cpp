//
// Created by redwan on 5/1/19.
//
#include <ros/ros.h>
#include "../include/PomdpPolicy.h"
#include "../include/MotionPrimitives.h"
//#include "../include/HamiltonianCycle.h"
#include <thread>

#include <cassert>



int main(int argc, char *argv[]){

    ros::init(argc,argv,"TASE_EXT");
    ROS_INFO("TASE EXPERIMENT STARTED");
    ros::NodeHandle nh;

    std::string topic_name;
    nh.getParam("/tase/topic_pub/vicon", topic_name);

//
    MotionPrimitives *move = new MotionPrimitives();
    PolicyExecution planner(move);
//    HamiltonianCycle planner(row,col,len,move);
    ros::AsyncSpinner spinner(4); // Use 4 threads
    ros::Subscriber gazebo_model = nh.subscribe(topic_name, 100, &MotionPrimitives::localizationCallback, move);
    spinner.start();
    planner.start();
    ros::waitForShutdown();








    return 0;
}
