#ifndef TOUCHSTATE_H
#define TOUCHSTATE_H

#include <QHHeadersGLUT.h>//Include all necessary headers
#include <iostream>

#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include "touch_msgs/TouchState.h"
#include "geometry_msgs/Vector3.h"
#include "geometry_msgs/Wrench.h"
#include "geometry_msgs/Pose.h"


class Listener{
    HDfloat force_[3];

    public:
    Listener();
    void setForce(HDfloat *force_des);
    void getForce(HDfloat *force);
    void forceCallback(const geometry_msgs::Wrench::ConstPtr& msg);
};

Listener::Listener(){
    for(int i=0;i<3;i++)
        force_[i]=0.0;
}

// Set desired force into variable force_
void Listener::setForce(HDfloat *force_des){
    for(int i=0;i<3;i++)
        force_[i] = force_des[i];
}

// Return the force values into the provided force array
void Listener::getForce(HDfloat *force){
    for(int i=0;i<3;i++)
        force[i] = force_[i];
}

// Update force_ variable from wrench message
void Listener::forceCallback(const geometry_msgs::Wrench::ConstPtr& msg){
    force_[0]=msg->force.x;
    force_[1]=msg->force.y;
    force_[2]=msg->force.z;
}

#endif