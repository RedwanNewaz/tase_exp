#include <iostream>
#include <unordered_map>
#include <vector>
#include <iterator>

#include <utility>
#include <thread>
#include <chrono>
#include "include/PolicyReader.h"
#include "include/ValidatePolicy.h"
#include "include/PomdpPolicyChecker.h"

#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include "tase_exp/fetchAction.h"
#include "geometry_msgs/PoseArray.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/Point.h"

//ros::Publisher pub;
//#define GUI true
#define SORT_POLICY true

#ifdef GUI
#include <QApplication>
#include <QtCore>
#include <QMessageBox>

int popUpMsg(beliefstruct robot, string act)
{
    QMessageBox msgBox;
    int window_w(1920),window_h(1080);
    msgBox.move((window_w-msgBox.width())/2,(window_h-msgBox.height())/2);
    QString x=QString::number(robot.x),y=QString::number(robot.y);
    QString msg = "Loc: ("+x+","+y+")";
    msgBox.setText("Continue Next Motion? [ "+QString(act.c_str())+"]");
    msgBox.setInformativeText(msg);
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    return msgBox.exec();
}
#endif
int seq =0;

void vizObstacle(vector<beliefstruct>&obstacles, ros::Publisher &pub )
{

    geometry_msgs::PoseArray msg;
    msg.header.frame_id = "/obstacles";
    msg.header.stamp= ros::Time::now();
    msg.header.seq = seq++;
    for(auto &o:obstacles)
    {
        geometry_msgs::Pose pose;
        pose.position.x = o.x;
        pose.position.y = o.y;
        pose.position.z = o.prob;
        msg.poses.push_back(pose);

    }
    pub.publish(msg);
}

void vizGroundTruth(vector<beliefstruct>&obstacles, ros::Publisher &pub )
{

    geometry_msgs::PoseArray msg;
    msg.header.frame_id = "/ref";
    msg.header.stamp= ros::Time::now();
    msg.header.seq = seq++;
    for(auto &o:obstacles)
    {
        geometry_msgs::Pose pose;
        pose.position.x = o.x;
        pose.position.y = o.y;
        pose.position.z = o.prob;
        msg.poses.push_back(pose);

    }
    pub.publish(msg);
}
void publishObstacles(vector<beliefstruct>&obstacles, vector<beliefstruct>&gt, ros::Publisher &pub )
{

    ros::Rate r(10);
    int count = 0;
    vizGroundTruth(gt,pub);
    r.sleep();
    vizObstacle(obstacles,pub);


}


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


using namespace std;

int main(int argc, char *argv[]) {

#ifdef GUI
    QApplication app(argc, argv);
#endif

    ros::init(argc, argv, "test_fetch_motions");
    std::cout << "Policy, Checker!" << std::endl;
//    assert(argc>=2 &&"folder path cannot found");
    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<geometry_msgs::PoseArray>("/tase/obstacles",100);
    string folderPath;
    nh.getParam("/tase/policy/test_folder", folderPath);


    PolicyReader reader(folderPath);
    reader.readFiles();
////    std::cout<<reader<<endl;
    auto b_0 = beliefstruct{0,0,1.0};
    auto Goal = beliefstruct{2,2,1.0};
    auto Gt = reader.GroundTruth;

    ValidatePolicy check(Goal,Gt);
    check.GetValidPolicy(reader.Policy);
    for (int j = 0; j < reader.Policy.size(); ++j) {
        auto pi = reader.Policy[j][0];
//        publishObstacles(pi->obstacles);
        std::thread thread1(publishObstacles, std::ref(pi->obstacles), std::ref(Gt), std::ref(pub));
        sendAction(pi->action);
        thread1.join();

    }

//    PomdpPolicyChecker *PI;
//
//#ifdef SORT_POLICY
//    BinaryTreeSort BTS(b_0);
//    BTS.createTree(reader.Policy);
////    BTS.printPolicy();
//    PI= BTS.getLinkedList();
//#else
//    assert(argc>=3 &&"option cannot found");
//    int option = atoi(argv[2]);
//    for(auto &pi:reader.Policy)
//    {
////        cout << *pi.second[0] ;
//        /* 1 & 2. allocate new node & put in the data  */
//        auto node = new PomdpPolicy(pi.second[option]->action,pi.second[option]->robot);
//        node->setObstacleBelief(pi.second[option]->obstacles);
//        /* 3. Make next of new node as head */
//        node->next = PI;
//        /* 4. move the head to point to the new node */
//        PI    = node;
//    }
//#endif
//    cout <<"POMPD POLICY \n"<<endl;
//    int result=0;
//    while(PI)
//    {
//        cout<<*PI<< "\t\t<<<<!!!!>>>\t OBST: ";
//        printVec(PI->getObstaclesBelief());
//#ifdef GUI
//        result = popUpMsg(PI->getState(), PI->nextActionName());
//#endif
//
//        sendAction(PI->getAction());
//        PI = PI->next;
//        if(result == 4194304)break;
//    }

    return 0;
}