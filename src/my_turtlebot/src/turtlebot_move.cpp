#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/Point.h"
#include <tf/transform_datatypes.h>
#include <nav_msgs/Odometry.h>
//#include <std_msgs/Float64.h>
#include <math.h>

using namespace std;
ros::Publisher velocity_publisher;
ros::Subscriber pose_subscriber;
double x_current, y_current, roll, pitch, yaw;


void move(double speed, double distance, bool isForward);
void rotate(double angular_speed, double angle, bool isClockwise);
void poseCallback(const nav_msgs::Odometry::ConstPtr & msg);

int main(int argc, char **argv) {

    ros::init(argc, argv, "turtlebot_move");
    ros::NodeHandle n;
    double speed;
    double distance;
    bool isForward;

    velocity_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel",10);
    pose_subscriber = n.subscribe("/odom",10,poseCallback);

    ROS_INFO("****TEST***\n");
    ros::Rate loop_rate(0.5);

    move(0.1,0.5,0);
    // rotate(0.1,1.5707,1);

    ros::spinOnce();

    return 0; 
}

void move(double speed, double distance, bool isForward) {
    geometry_msgs::Twist vel_msg;
    nav_msgs::Odometry msg;
    
    double x0 = msg.pose.pose.position.x;
    double y0 = msg.pose.pose.position.y;

    if (isForward)
        vel_msg.linear.x = abs(speed);
    else 
        vel_msg.linear.x = -abs(speed);
    vel_msg.linear.y = 0;
    vel_msg.linear.z = 0;

    vel_msg.angular.x = 0;
    vel_msg.angular.y = 0;
    vel_msg.angular.z = 0;

    double current_distance = 0.0;
    ros::Rate loop_rate(100);

    do {
        ROS_INFO(" inital distance : %1.2f", x0);
        ROS_INFO(" current distance : %1.2f", x_current);
        velocity_publisher.publish(vel_msg);
        current_distance = sqrt(pow(x_current-x0,2)+pow(y_current-y0,2));
        // current_distance = abs(x_current-x0);
        ros::spinOnce();
        loop_rate.sleep();
    }while(current_distance < distance);
    vel_msg.linear.x = 0;
    velocity_publisher.publish(vel_msg);
    // x0 = x_current;

}

void rotate(double angular_speed, double abs_angle, bool isClockwise) {
    geometry_msgs::Twist vel_msg;
    // nav_msgs::Odometry msg;
    
    double w0 = 0;

    if (isClockwise)
        vel_msg.angular.z = -abs(angular_speed);
    else 
        vel_msg.angular.z = abs(angular_speed);
    vel_msg.linear.x = 0;
    vel_msg.linear.y = 0;
    vel_msg.linear.z = 0;

    vel_msg.angular.x = 0;
    vel_msg.angular.y = 0;
    
    double current_angle = 0.0;
    ros::Rate loop_rate(100);

    do {
        ROS_INFO(" inital angle : %1.2f", w0);
        ROS_INFO(" current angle : %1.2f", yaw);
        velocity_publisher.publish(vel_msg);
        current_angle = yaw-w0;
        // current_distance = abs(x_current-x0);
        ros::spinOnce();
        loop_rate.sleep();
    }while(current_angle < abs_angle);
     vel_msg.angular.z = 0;
    velocity_publisher.publish(vel_msg);
}

void poseCallback(const nav_msgs::Odometry::ConstPtr & msg) {
    x_current = msg->pose.pose.position.x;
    y_current = msg->pose.pose.position.y;
    tf::Quaternion q (
        msg->pose.pose.orientation.x,
        msg->pose.pose.orientation.y,
        msg->pose.pose.orientation.z,
        msg->pose.pose.orientation.w);
    tf::Matrix3x3 m(q);
    m.getRPY(roll, pitch, yaw);
    yaw = -yaw;
    // ROS_INFO(" current_x : %1.2f", x_current);
    // ROS_INFO(" current_y : %1.2f", y_current);
    // ROS_INFO(" roll : %1.2f", roll);
    // ROS_INFO(" pitch : %1.2f", pitch);
    // ROS_INFO(" yaw : %1.2f", yaw);
}