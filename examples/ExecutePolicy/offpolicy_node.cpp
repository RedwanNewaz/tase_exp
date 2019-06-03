//
// Created by redwan on 6/2/19.
//
#include <ros/ros.h>
#include <cassert>
#include <string>
#include <vector>
#include <memory>
#include "include/ConditionalPlan.h"


int main(int argc, char *argv[]){
    ros::init(argc,argv,"OffPolicyNode");
    ROS_INFO("OffPolicyNode STARTED");
    ros::NodeHandle nh;
    string policy_file,belief_file,test_file;
    nh.getParam("/tase/policy/policy_file", policy_file);
    nh.getParam("/tase/policy/belief_file", belief_file);
    nh.getParam("/tase/policy/test_file", test_file);
    auto policyData = ConditionalPlan::read_file(policy_file);
    auto beliefData = ConditionalPlan::read_file(belief_file);
    auto gtData = ConditionalPlan::read_file(test_file);

    vector<ConditionalPlan> Policy;
    int pre = -1;
    for (int j = 0; j < policyData.size(); ++j) {
        int index = Policy.size();
        Policy.emplace_back(ConditionalPlan(policyData[j],beliefData[j]));
        auto b = Policy[index].b_indx;
        if(b<pre)
        {
            Policy.pop_back();
            break;
        }
        pre = b;
    }

    printVec(Policy);




    return 0;
}