#include <iostream>
#include <QHHeadersGLUT.h>//Include all necessary headers

#include "ros/ros.h"
#include "std_msgs/Float32.h"

int main(int argc, char *argv[]){

    ros::init(argc, argv, "talker");
    ros::NodeHandle n;

    ros::Publisher pub = n.advertise<std_msgs::Float32>("mynum", 10);
    ros::Rate loop_rate(100);

    while (ros::ok()){
        std_msgs::Float32 f;
        f.data = 120.0;

        pub.publish(f);

        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}