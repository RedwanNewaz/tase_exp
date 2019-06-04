//
// Created by redwan on 6/3/19.
//

#include "FetchAction.h"

FetchAction::FetchAction(string name) :
        as_(nh_, name, boost::bind(&FetchAction::executeCB, this, _1), false),
action_name_(name){
    std::string topic_name;
    nh_.getParam("/tase/topic_pub/vicon", topic_name);
    move = new MotionPrimitives();
    gazebo_model = nh_.subscribe(topic_name, 100, &MotionPrimitives::localizationCallback, move);
    obstacle_viz = nh_.subscribe("/tase/obstacles", 100, &FetchAction::ObstacleCallback, this);
    robot.set(0,0);
    as_.start();

}

void FetchAction::executeCB(const tase_exp::fetchGoalConstPtr &goal) {
    // helper variables
    bool success = true;
    // push_back the seeds for the fibonacci sequence

    feedback_.result = false;

    robot = robot + goal->act;
    // publish info to the console for the user
     ROS_INFO("%s recived %d", action_name_.c_str(), goal->act);
     ROS_INFO_STREAM(robot);


    if(success)
    {
        result_.coord.push_back(0);
        result_.coord.push_back(0);
        ROS_INFO("%s: Succeeded", action_name_.c_str());
        // set the action state to succeeded
        as_.setSucceeded(result_);
    }

    if(goal->act<5)
        move->goTo(robot.x,robot.y);
    else if(goal->act<9)
        move->lookAt(goal->act);

    if(robot==BeliefState(3,2) || robot==BeliefState(2,3))
    {
        ROS_INFO_STREAM("GOAL REACHED");
        ros::shutdown();
    }

}

FetchAction::~FetchAction() {
    delete move;
}

void FetchAction::ObstacleCallback(const geometry_msgs::PoseArray::ConstPtr &msg) {



    int id;
    bool isGt = (msg->header.frame_id != "/ref");
    ROS_INFO_STREAM("recived frame " <<msg->header.frame_id <<isGt);
    id = isGt?0:10;
    for(auto &pose:msg->poses)
    {
        QPoint p(pose.position.x,pose.position.y);
        auto prob = pose.position.z;
        if(isGt)
        {
            disp.setObstacles(++id,p,"red",prob);
            ROS_INFO_STREAM("publishing obstacles");
        }
        else
        {
            disp.setObstacles(++id,p,"yellow",prob);
            ROS_INFO_STREAM("publishing ground truth");
        }
    }




//    vector<pair<QPoint, float>> gt;
//    auto collision = pi->getObstacle();
//    for(auto &g:ground_truth)
//    {
//        bool isSame = false;
//        for(auto &b:collision)
//        {
//            if(b.first==g)
//            {
//                isSame = true;
//                break;
//            }
//        }
//        if(!isSame)
//            gt.push_back(make_pair(g,1.0));
//    }
//    // call visuzliation function
//    int id = 0;
//    for( auto &o:gt)
//    {
//        disp.setObstacles(++id,o.first,"yellow",o.second);
//    }
//
//    for( auto &o:collision)
//    {
//        disp.setObstacles(++id,o.first,"red",o.second);
//    }

}
