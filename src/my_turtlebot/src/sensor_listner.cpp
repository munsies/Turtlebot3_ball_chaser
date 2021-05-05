#include "ros/ros.h"
//#include "geometry_msgs/Twist.h"
//#include "geometry_msgs/Point.h"
#include <tf/transform_datatypes.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/Float64.h>
#include <math.h>

using namespace std;

ros::Subscriber pose_listner;

double x_current, y_current, roll, pitch, yaw;

void poselisten(const nav_msgs::Odometry::ConstPtr & msg);

double mod(double first_term, double second_term)
{
    // Compute the modulus
    return first_term - (second_term)*floor(first_term / (second_term));
}

int main(int argc, char **argv) {

    ros::init(argc, argv, "sensor_listner");
    ros::NodeHandle n;
  
    pose_listner = n.subscribe("/odom",1000,poselisten);

    ROS_INFO("****Listen***\n");

    // ros::Rate loop_rate(0.5);
 

    ros::spin();

    return 0; 
}

void poselisten(const nav_msgs::Odometry::ConstPtr & msg) {
    
    x_current = msg->pose.pose.position.x;
    y_current = msg->pose.pose.position.y;
    tf::Quaternion q (
        msg->pose.pose.orientation.x,
        msg->pose.pose.orientation.y,
        msg->pose.pose.orientation.z,
        msg->pose.pose.orientation.w);
    tf::Matrix3x3 m(q);
    m.getRPY(roll, pitch, yaw);
    ROS_INFO(" current_x : %1.2f", x_current);
    ROS_INFO(" current_y : %1.2f", y_current);
    yaw = mod(yaw, 2 * M_PI);
    ROS_INFO(" roll : %1.2f", roll);
    ROS_INFO(" pitch : %1.2f", pitch);
    ROS_INFO(" yaw : %1.2f", yaw);
}