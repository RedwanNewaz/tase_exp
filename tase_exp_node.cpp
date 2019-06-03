//
// Created by redwan on 5/1/19.
//
#include <ros/ros.h>
#include "../include/PomdpPolicy.h"
#include "../include/MotionPrimitives.h"
#include <thread>
#include <cassert>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <iterator>
#include <PolicyReader.h>
#include <PomdpPolicyChecker.h>
#include <ValidatePolicy.h>
#define SORT_POLICY true
void executePolicy(int argc, char *argv[])
{

    ROS_INFO("Policy, Checker!");
    assert(argc>=2 &&"folder path cannot found");

    string folderPath = argv[1];
    PolicyReader reader(folderPath);
    reader.readFiles();
////    std::cout<<reader<<endl;
    auto b_0 = beliefstruct{0,0,1.0};
    auto Goal = beliefstruct{2,2,1.0};
    auto Gt = reader.GroundTruth;

    ValidatePolicy check(Goal,Gt);
    check.GetValidPolicy(reader.Policy);

    PomdpPolicyChecker *PI;

    BinaryTreeSort BTS(b_0);
    BTS.createTree(reader.Policy);
    BTS.printPolicy();
//    PI= BTS.getLinkedList();
    ROS_INFO_STREAM("POMPD POLICY \n");


}



int main(int argc, char *argv[]){

    ros::init(argc,argv,"TASE_EXT");
    ROS_INFO("TASE EXPERIMENT STARTED");
    ros::NodeHandle nh;
    std::string topic_name;
//    nh.getParam("/tase/topic_pub/vicon", topic_name);
//    MotionPrimitives *move = new MotionPrimitives();


    std::thread runner(&executePolicy,argc,argv);


//    PolicyExecution planner(move);
    ros::AsyncSpinner spinner(4); // Use 4 threads
//    ros::Subscriber gazebo_model = nh.subscribe(topic_name, 100, &MotionPrimitives::localizationCallback, move);
    spinner.start();
    runner.join();

//    planner.start();
    ros::waitForShutdown();








    return 0;
}
