//
// Created by redwan on 6/2/19.
//
#include <ros/ros.h>
#include "include/HamiltonianCycle.h"

int main(int argc, char *argv[]){

    ros::init(argc,argv,"HamiltonianNode");
    ROS_INFO("Hamiltonian Node STARTED");
    ros::NodeHandle nh;
    int row = 4, col =4;
    float len;
    HamiltonianCycle ham;
    auto g = ham.gridAdjacency(row,col);
    if(ham.hamCycle(g))
    {
        auto path = ham.getTrajectory();
        for(auto &p:path)
            cout<<p<<endl;
    }

    return 0;
}