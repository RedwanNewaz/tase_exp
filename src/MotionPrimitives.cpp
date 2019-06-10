//
// Created by redwan on 5/1/19.
//

#include "../include/MotionPrimitives.h"
#define UNIT 0.90

MotionPrimitives::MotionPrimitives() {
    std::string topic;
    nh_.getParam("/tase/topic_pub/repeat", repeat_);
    nh_.getParam("/tase/topic_pub/goal", topic);

    nh_.getParam("/tase/move_base/xy_tolarance", xy_tolarance_);
    nh_.getParam("/tase/move_base/pitch", pitch_);
    nh_.getParam("/tase/move_base/nap_rate", nap_rate_);

    setX = setY = yaw = 0.0;
    pub_ = nh_.advertise<geometry_msgs::PoseStamped>(topic, 1000);

    look_[NORTH]= 0;
    look_[SOUTH]= 0;
    look_[WEST]= UNIT;
    look_[EAST]= -UNIT;

    std::string action_name = "head_controller/point_head";
    point_head_action_client_.reset(new point_head_action_client_t(action_name, true));
    if (!point_head_action_client_->waitForServer(ros::Duration(5.0)))
        ROS_ERROR("%s may not be connected.", action_name.c_str());



}

MotionPrimitives::~MotionPrimitives() {


}



void MotionPrimitives::goTo(double x, double y) {
    // get current base pose
    std::vector<double> base_pose = getCurrentBasePose();
    double x_pre = base_pose[0];
    double y_pre = base_pose[1];

    // first make a turn
    double dx = x - x_pre;
    double dy = y - y_pre;


    yaw = tf2Atan2(dy,  dx);
    setX = x ;
    setY = y ;

    double dist = sqrt(dx*dx+dy*dy);
    if(dist<0.2)
    {
        ROS_WARN("ignoring current request");
        return;
    }

    ROS_INFO("Robot base target yaw: %f", yaw);
    target_achieved = false;
    publishGoal();





}

void MotionPrimitives::lookAt(int z) {

    ros::Rate r(1/float(nap_rate_));
    r.sleep();


    
    control_msgs::PointHeadGoal point_head_goal;
    point_head_goal.target.header.stamp = ros::Time::now();
    point_head_goal.target.header.frame_id = "base_link";
    z -=5;
    std::cout << "u: " << z << std::endl;
    if(z==0)
    {
        ROS_INFO("looking at north");
        point_head_goal.target.point.x = 1.0;
        point_head_goal.target.point.y = 0.0;
        point_head_goal.target.point.z = 0.6;
    }
    else if(z==1){
        ROS_INFO("looking at south");
        point_head_goal.target.point.x = 0.0;
        point_head_goal.target.point.y = 0.0;
        point_head_goal.target.point.z = 0.0;
    }
    else if(z==2) // west
    {
        ROS_INFO("looking at west");
        point_head_goal.target.point.x = 0.0;
        point_head_goal.target.point.y = 1.0;
        point_head_goal.target.point.z = 0.6;
    }else if(z==3) //east
    {
        ROS_INFO("looking at east");
        point_head_goal.target.point.x = 0.0;
        point_head_goal.target.point.y = -1.0;
        point_head_goal.target.point.z = 0.6;
    }
    point_head_goal.min_duration = ros::Duration(1.0);

    point_head_action_client_->sendGoal(point_head_goal);
    bool result = point_head_action_client_->waitForResult(ros::Duration(5.0));
    if(!result) lookAt(z);
    if(z != 0) lookAt(5);




}

void MotionPrimitives::publishGoal() {
    /**@brief
     * we need to repeat the publish msg several times with nap_rate intervals to be ensured that other nodes received it. \par
     * While executing trajectory this thread take sleep using conditional variable -the performance is significantly better. \par
     * This thread wakes up from the localizationCallback if the target is achieved.
     */

    std::unique_lock<mutex>lk(mu_);
    // convert setx, sety, yaw to PoseStamped msg
    geometry_msgs::PoseStamped msg;
    msg.header.frame_id ="/map";
    msg.header.stamp = ros::Time::now();
    msg.pose.position.x = setX;
    msg.pose.position.y = setY;

    auto q = getOrientation(yaw);
    msg.pose.orientation.x = q.x();
    msg.pose.orientation.y = q.y();
    msg.pose.orientation.z = q.z();
    msg.pose.orientation.w = q.w();

    ros::Rate r(nap_rate_);
    for (int j = 0; j < repeat_; ++j) {
        pub_.publish(msg);
        r.sleep();
    }
    cond_.wait(lk,[=]{ return target_achieved;});
    lk.unlock();

//    ros::Rate r(nap_rate_);
//    for (int j = 0; j < repeat_; ++j) {
//        pub_.publish(msg);
//        r.sleep();
//    }
//
//
//    while(!target_achieved)
//        r.sleep();

}




void MotionPrimitives::localizationCallback(const nav_msgs::Odometry::ConstPtr &msg) {

    std::lock_guard<std::mutex>lk(mu);
    auto p = msg->pose.pose.position;

    robot_position[0] = p.x ;
    robot_position[1] = p.y ;
    robot_position[2] = p.z ;
    //robot_orientation = msg->transform.rotation;

    //std::call_once(fix_origin_, [&](){ oX = p.x; oY = p.y; });

    //=======================================================================
    // Transform from map to odom
    //=======================================================================
    double useless_pitch, useless_roll, yaw;
    ros::Time t;
    std::string err2 = "";
    tf::StampedTransform tf_odom_to_fixed;
    tf_listener_.getLatestCommonTime("odom", "map", t, &err2);
    tf_listener_.lookupTransform("odom", "map", t, tf_odom_to_fixed);
    tf_odom_to_fixed.getBasis().getEulerYPR(yaw, useless_pitch, useless_roll);

    tf::Point goal_point_map_frame(robot_position[0], robot_position[1], 0.0);
    goal_point_map_frame = tf_odom_to_fixed.inverse() * goal_point_map_frame;
    p.x = goal_point_map_frame.getX();
    p.y = goal_point_map_frame.getY();

    //double psi = tf2::getYaw(msg->transform.rotation);

    float err =sqrt(pow((p.x-setX),2)+pow((p.y-setY),2));
    if(err<xy_tolarance_)
    {
        target_achieved = true;
        cond_.notify_one();
    }
    else if(err<pitch_)
        ROS_INFO("err %f", err);




//ROS_INFO("robot at [%f, %f, %f]", abs(p.x-setX), abs(p.y-setY), abs(psi-yaw));
}




