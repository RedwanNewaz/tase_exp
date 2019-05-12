//
// Created by redwan on 5/3/19.
//

#ifndef TASE_EXP_VIZTRAJ_H
#define TASE_EXP_VIZTRAJ_H
#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <geometry_msgs/Point.h>
#include <QtCore>
#include <queue>
#include <iostream>
using namespace std;
class VizTraj{
public:
    VizTraj(){
        vis_pub = nh.advertise<visualization_msgs::Marker>("/trajectory", 100);

        marker.header.frame_id = "/map";
        marker.header.stamp = ros::Time::now();

        marker.action =visualization_msgs::Marker::ADD;

    }
    void setTrajectory(queue<QPointF>traj)
    {
        marker.ns = "global_traj";
        marker.type =visualization_msgs::Marker::LINE_STRIP;
        marker.id = 1;

        marker.scale.x = 0.1;
        marker.scale.y = 0.1;
        marker.scale.z = 0.1;
        marker.color.r =0.5;
        marker.color.g =0.5;
        marker.color.a = 1.0;

        while(!traj.empty())
        {
            auto point = traj.front();
            traj.pop();
            geometry_msgs::Point p;
            p.x = point.x();
            p.y = point.y();
            p.z = 0.10;
            marker.points.push_back(p);

        }
        publish();
    }

    void setObstacles(int id, QPoint pos, string type, float intensity)
    {
        marker.ns = "obstacles";
        marker.type =visualization_msgs::Marker::CYLINDER;
        marker.id = id;
        marker.scale.x = 0.5;
        marker.scale.y = 0.5;
        marker.scale.z = 1.0;
        auto color = color_type(type);
        marker.color.r =color[0];
        marker.color.g =color[1];
        marker.color.b =color[2];
        marker.color.a =intensity;
        marker.pose.position.x = pos.x();
        marker.pose.position.y = pos.y();
        marker.pose.position.z = 0.5;
        marker.pose.orientation.x = 0.0;
        marker.pose.orientation.y = 0.0;
        marker.pose.orientation.z = 0.0;
        marker.pose.orientation.w = 1.0;
        publish();

    }

    vector<float> color_type(string name)
    {
        vector<float>yellow{1.0,1.0,0.0}, red{1.0,0.0,0.0};
        return (name=="yellow")?yellow:red;
    }

    void publish()
    {
        vis_pub.publish( marker );
//        qDebug()<<"published traj";
    }

private:
    ros::NodeHandle nh;
    ros::Publisher vis_pub;
    visualization_msgs::Marker marker;


};


#endif //TASE_EXP_VIZTRAJ_H
