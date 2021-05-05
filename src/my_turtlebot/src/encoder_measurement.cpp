#include "ros/ros.h"
#include <turtlebot3_msgs/SensorState.h>
#include <sensor_msgs/Imu.h>
#include <nav_msgs/Odometry.h>
#include <tf/transform_datatypes.h>
#include <fstream>
#include <string>
// #include <rosbag/bag.h>

using namespace std;

std::ofstream out_file {"/home/manan/catkin_ws3/src/my_turtlebot/src/imu_data.txt"};
std::ofstream out_file2 {"/home/manan/catkin_ws3/src/my_turtlebot/src/encoder_data.txt"};
std::ofstream out_file3 {"/home/manan/catkin_ws3/src/my_turtlebot/src/odom_pose.txt"};

 
double t0; 

void poselisten(const nav_msgs::Odometry::ConstPtr & msg);
void encoder_values(const turtlebot3_msgs::SensorState::ConstPtr & msg);
void imu_values(const sensor_msgs::Imu::ConstPtr & msg);

int main(int argc, char **argv) {

    ros::init(argc, argv, "encoder_listner");
    ros::NodeHandle n;
    t0 = ros::Time::now().toSec();
    ros::Subscriber odom_listner = n.subscribe("/sensor_state",1,encoder_values);
    ros::Subscriber imu_listner = n.subscribe("/imu",10,imu_values);
    ros::Subscriber pose_listner = n.subscribe("/odom",1,poselisten);

    ROS_INFO("****Listen***\n");  
    ros::Rate loop_rate(10);
    if (!out_file) {
        std::cerr << "Error creating file" << std::endl;
    return 1;
    }
    if (!out_file2) {
        std::cerr << "Error creating file" << std::endl;
    return 1;
    }
    if (!out_file3) {
        std::cerr << "Error creating file" << std::endl;
    return 1;
    }
    // loop_rate.sleep();
    ros::spin();
//    out_file.close();
    return 0; 
}

void encoder_values(const turtlebot3_msgs::SensorState::ConstPtr & msg) {
    
    // ros::Time encoder_time = msg->header.stamp;
    double t_current = ros::Time::now().toSec();
    double dt = (t_current - t0);
    int enc_l = msg->left_encoder; 
    int enc_r = msg->right_encoder; 
    // ROS_INFO(" time stamp  : %f ", encoder_time.toSec());
    // ROS_INFO(" encoder left  : %d", enc_l);
    // ROS_INFO(" encoder right : %d", enc_r);
    out_file2 << dt << " " << enc_l << " " << enc_r << endl;
}

void imu_values(const sensor_msgs::Imu::ConstPtr & msg) {

    // ros::Time imu_time = msg->header.stamp;
    double t_current = ros::Time::now().toSec();
    double dt = (t_current - t0);
    float acc_x = msg->linear_acceleration.x; 
    float acc_y = msg->linear_acceleration.y;
    float acc_z = msg->linear_acceleration.z;
    float ang_x = msg->angular_velocity.x; 
    float ang_y = msg->angular_velocity.y;
    float ang_z = msg->angular_velocity.z;
    // std::string line;
    out_file << dt << " " << acc_x << " " << acc_y << " " << acc_z << \
    " " << ang_x << " " << ang_y << " " << ang_z << endl;
    // ROS_INFO(" accelaration x: %1.2f, y: %1.2f, z: %1.2f ", acc_x,acc_y,acc_z);
    // ROS_INFO(" angular velocity x: %1.2f, y: %1.2f, z: %1.2f ", ang_x,ang_y,ang_z);
}

void poselisten(const nav_msgs::Odometry::ConstPtr & msg) {
    // ros::Time pose_time = msg->header.stamp;
    double t_current = ros::Time::now().toSec();
    double dt = (t_current - t0);
    double x_current = msg->pose.pose.position.x;
    double y_current = msg->pose.pose.position.y;
    double roll, pitch, yaw;
    tf::Quaternion q (
        msg->pose.pose.orientation.x,
        msg->pose.pose.orientation.y,
        msg->pose.pose.orientation.z,
        msg->pose.pose.orientation.w);
    tf::Matrix3x3 m(q);
    m.getRPY(roll, pitch, yaw);
    out_file3 << dt << " " << x_current << " " << y_current << " " << yaw << endl;
    // ROS_INFO(" current_x : %1.2f", x_current);
    // ROS_INFO(" current_y : %1.2f", y_current);
    // ROS_INFO(" roll : %1.2f", roll);
    // ROS_INFO(" pitch : %1.2f", pitch);
    // ROS_INFO(" yaw : %1.2f", yaw);
}