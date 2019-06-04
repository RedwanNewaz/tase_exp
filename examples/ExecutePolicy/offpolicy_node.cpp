//
// Created by redwan on 6/2/19.
//
#include <ros/ros.h>
#include <cassert>
#include <string>
#include <vector>
#include <memory>
#include "include/ConditionalPlan.h"
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include "tase_exp/fetchAction.h"

void sendAction(int a)
{
// create the action client
    // true causes the client to spin its own thread
    actionlib::SimpleActionClient<tase_exp::fetchAction> ac("FetchMotionPremitives", true);

//    ROS_INFO("Waiting for action server to start.");
    // wait for the action server to start
    ac.waitForServer(); //will wait for infinite time

    ROS_INFO("Action server started, sending goal %d", a);
    // send a goal to the action
    tase_exp::fetchGoal goal;
    goal.act = a;
    ac.sendGoal(goal);

    //wait for the action to return
    bool finished_before_timeout = ac.waitForResult(ros::Duration(30.0));

    if (finished_before_timeout)
    {
        actionlib::SimpleClientGoalState state = ac.getState();
//        ROS_INFO("Action finished: %s",state.toString().c_str());
    }
    else
        ROS_INFO("Action did not finish before the time out.");

}


int main(int argc, char *argv[]){
    ros::init(argc,argv,"OffPolicyNode");
    ROS_INFO("OffPolicyNode STARTED");
    ros::NodeHandle nh;
    string policy_file("/home/redwan/ros/melodic/dev/src/tase_exp/result/policy.txt"),
    belief_file("/home/redwan/ros/melodic/dev/src/tase_exp/result/belief.txt"),
    test_file("/home/redwan/ros/melodic/dev/src/tase_exp/result/corridor_test_11");
//    nh.getParam("/tase/policy/policy_file", policy_file);
//    nh.getParam("/tase/policy/belief_file", belief_file);
//    nh.getParam("/tase/policy/test_file", test_file);
    auto policyData = ConditionalPlan::read_file(policy_file);
    auto beliefData = ConditionalPlan::read_file(belief_file);
    auto gtData = ConditionalPlan::read_file(test_file);

    vector<ConditionalPlan> Policy;
    int pre = -1;
    for (int j = 0; j < policyData.size(); ++j) {
        int index = Policy.size();
        Policy.emplace_back(ConditionalPlan(policyData[j],beliefData[j]));
        auto b = Policy[index].b_indx;
        sendAction(Policy[index].action);
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