//
// Created by redwan on 6/4/19.
//

#ifndef TASE_EXP_FETCHACTIONCLIENT_H
#define TASE_EXP_FETCHACTIONCLIENT_H
#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include "tase_exp/fetchAction.h"
#include "geometry_msgs/PoseArray.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/Point.h"

#define GUI true

#ifdef GUI
#include <QtCore>
#include <QMessageBox>
#endif

using namespace std;

class FetchActionClient{
public:
    FetchActionClient(int argc, char *argv[])
    {

        ros::init(argc, argv, "test_fetch_motions");
        ros::NodeHandle nh;
        nh.getParam("/tase/policy/test_folder", folderPath);
        pub = nh.advertise<geometry_msgs::PoseArray>("/tase/obstacles",100);
        seq = 0;
        ACTION_MEANING[1]="Move north";
        ACTION_MEANING[2]="Move south";
        ACTION_MEANING[3]="Move west ";
        ACTION_MEANING[4]="Move east ";
        ACTION_MEANING[5]="Look north";
        ACTION_MEANING[6]="Look south";
        ACTION_MEANING[7]="Look west ";
        ACTION_MEANING[8]="Look east ";
        ACTION_MEANING[9]="Pick up left hand ";
        ACTION_MEANING[10]="Pick up right hand";
    }
    template <class C, typename T>
    int operator()(vector<C> &conditionalPlans, vector<T>&Gt){
        int result =0;
        for (auto &plan:conditionalPlans)
        {
            sendVizData(plan->obstacles,Gt);
#ifdef GUI
            result = popUpMsg(plan->robot,ACTION_MEANING[plan->action]);
#endif
            if(result == 4194304)break;
            sendAction(plan->action);
        }
        return 1;
    }

    template <class T>
    void sendVizData(vector<T> &obstacles, vector<T> &groundTruth)
    {
        ros::Rate r(10);
        geometry_msgs::PoseArray obs_msg, gt_msg;
        pubVizMsg(groundTruth,gt_msg,"/ref");
        r.sleep();
        pubVizMsg(obstacles,obs_msg,"/obstacles");

    }
    string getFolderPath()
    {
        return folderPath;
    }


private:

    ros::Publisher pub;
    int seq;
    std::map<int,string>ACTION_MEANING;
    string folderPath;
protected:
    void sendAction(int a)
    {
// create the action client
        // true causes the client to spin its own thread
        actionlib::SimpleActionClient<tase_exp::fetchAction> ac("FetchMotionPremitives", true);

        ROS_INFO("Waiting for action server to start.");
        // wait for the action server to start
        ac.waitForServer(); //will wait for infinite time

        ROS_INFO("Action server started, sending goal.");
        // send a goal to the action
        tase_exp::fetchGoal goal;
        goal.act = a;
        ac.sendGoal(goal);

        //wait for the action to return
        bool finished_before_timeout = ac.waitForResult(ros::Duration(30.0));

        if (finished_before_timeout)
        {
            actionlib::SimpleClientGoalState state = ac.getState();
            ROS_INFO("Action finished: %s",state.toString().c_str());
        }
        else
            ROS_INFO("Action did not finish before the time out.");

    }


    template <typename T>
    void pubVizMsg(vector<T> &data, geometry_msgs::PoseArray &msg, string frame)
    {
        msg.header.frame_id = frame;
        msg.header.stamp= ros::Time::now();
        msg.header.seq = seq++;
        for(auto &o:data)
        {
            geometry_msgs::Pose pose;
            pose.position.x = o.x;
            pose.position.y = o.y;
            pose.position.z = o.prob;
            msg.poses.push_back(pose);

        }
        pub.publish(msg);
    }

#ifdef GUI
template <class T>
int popUpMsg(T robot, string act)
{

    QMessageBox msgBox;
    int window_w(1920),window_h(1080);
    msgBox.move((window_w-msgBox.width())/2,(window_h-msgBox.height())/2);
    QString x=QString::number(robot.x),y=QString::number(robot.y);
    QString msg = "Loc: ("+x+","+y+")";
    msgBox.setText("Continue Next Motion? [" +QString(act.c_str()) + "]");
    msgBox.setInformativeText(msg);
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    return msgBox.exec();
}
#endif

};


#endif //TASE_EXP_FETCHACTIONCLIENT_H
