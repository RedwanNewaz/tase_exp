//
// Created by redwan on 5/3/19.
//

#ifndef TASE_EXP_TRAJPLANNER_H
#define TASE_EXP_TRAJPLANNER_H

#include <iostream>
#include <QtCore>
#include <QPointF>
#include <QThread>
#include <queue>
#include "MotionPrimitives.h"
#include "VizTraj.h"
class TrajPlanner:public QThread{
public:
    TrajPlanner(MotionPrimitives *move):move_(move){

    };

    virtual ~TrajPlanner() {
        delete move_;
    }

    virtual bool generate()=0;
    bool hasNext() {
        return !trajectory_.empty();
    };
    QPointF next() {
        QPointF point = trajectory_.front();
        trajectory_.pop();
        return point;
    }
    void write_traj(QString &log){
        QString filename ="/home/redwan/catkin_ws/src/tase_exp/result/log.csv";
        QFile file(filename);
        if (file.open(QIODevice::ReadWrite)) {
            QTextStream stream(&file);
            stream <<log;
        }
    }
    void run(){
        bool status = generate();
        if(!status)
        {
            std::cerr<<"no trajectory found"<<std::endl;
            exit(-1);
        }
        //TODO - create visulization class for showing gloabl trajectory

        // TODO - show robot tracking trajectory
        disp.setTrajectory(trajectory_);
        while (hasNext()&& ros::ok()){
            QPointF point = next();
            disp.publish();
            //FIXME - get tracking error and write log with tracking error
            //FIXME - get execution time
            auto start = ros::Time::now();
            move_->goTo(point.x(),point.y());
            auto end = ros::Time::now();
            auto duration = end - start;
            qDebug()<<point <<" | "<<trajectory_.size()<<" | "<<duration.sec<<" s";
        }

        qDebug()<<"Planner thread terminated";

    }

protected:
    std::queue<QPointF> trajectory_;
    MotionPrimitives *move_;
    VizTraj disp;
};
#endif //TASE_EXP_TRAJPLANNER_H
