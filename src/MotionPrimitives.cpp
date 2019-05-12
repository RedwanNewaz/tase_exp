//
// Created by redwan on 5/1/19.
//

#include "../include/MotionPrimitives.h"


MotionPrimitives::MotionPrimitives() {
    std::string topic;
    nh_.getParam("/tase/topic_pub/repeat", repeat_);
    nh_.getParam("/tase/topic_pub/goal", topic);

    nh_.getParam("/tase/move_base/xy_tolarance", xy_tolarance_);
    nh_.getParam("/tase/move_base/err_viz", err_viz_);
    nh_.getParam("/tase/move_base/nap_rate", nap_rate_);

    setX = setY = yaw = 0.0;
    pub_ = nh_.advertise<geometry_msgs::PoseStamped>(topic, 1000);

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
    if(dist<1)
    {
        ROS_WARN("ignoring current request");
        return;
    }

    ROS_INFO("Robot base target yaw: %f", yaw);
    publishGoal();
    target_achieved = false;


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
    cond_.wait(lk,[&](){ return target_achieved;});
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




void MotionPrimitives::localizationCallback(const geometry_msgs::TransformStamped::ConstPtr &msg) {

    std::lock_guard<std::mutex>lk(mu);
    auto p = msg->transform.translation;

    robot_position[0] = p.x ;
    robot_position[1] = p.y ;
    robot_position[2] = p.z ;
    robot_orientation = msg->transform.rotation;

    std::call_once(fix_origin_, [&](){ oX = p.x; oY = p.y; });


    double psi = tf2::getYaw(msg->transform.rotation);

    float err =sqrt(pow((p.x-setX),2)+pow((p.y-setY),2));
    if(err<xy_tolarance_)
    {
        target_achieved = true;
        cond_.notify_one();
    }
    else if(err<err_viz_)
        ROS_INFO("err %f", err);




//ROS_INFO("robot at [%f, %f, %f]", abs(p.x-setX), abs(p.y-setY), abs(psi-yaw));
}




