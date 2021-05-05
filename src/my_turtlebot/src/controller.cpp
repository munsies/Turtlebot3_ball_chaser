#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/Point.h"
#include <tf/transform_datatypes.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/Float64.h>
#include <math.h>

using namespace std;

// double x_current, y_current, roll, pitch, yaw, inc_x, inc_y, angle_to_goal;
double x_current, y_current, roll, pitch, yaw;
double inc_x, inc_y, angle_to_goal;

ros::Publisher velocity_publisher;
ros::Subscriber pose_subscriber;

double mod(double first_term, double second_term)
{
    // Compute the modulus
    return first_term - (second_term)*floor(first_term / (second_term));
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
    yaw = mod(yaw, 2 * M_PI);
    // yaw = -yaw;

    ROS_INFO(" roll : %1.2f", roll);
    ROS_INFO(" pitch : %1.2f", pitch);
    ROS_INFO(" yaw : %1.2f", yaw);
}

double getdistance(double x, double y, double goal_x, double goal_y) {
    double distance = sqrt(pow(goal_x-x,2)+pow(goal_y-y,2));
    return distance;
}

void moveGoal(geometry_msgs::Point goal_pose, double distance_tolerace) {
    geometry_msgs::Twist vel_msg;

    ros::Rate loop_rate(100);

    double Kp = 1.0;
    double Kd = 0.3;
    double Ki = 0.0;

    double e_sum = 0;
    double e_a_sum = 0;

    double Kang_p = 1.0;
    double Kang_d = 0.0;
    double Kang_i = 0.0;

    double ct_error_prev = 0.0;
    double ct_a_prev = 0.0;
    // double e = 1.0;

    do {
        double e = getdistance(x_current,y_current,goal_pose.x, goal_pose.y);
        e_sum = e + e_sum;

        vel_msg.linear.x = Kp*e + Kd*(e-ct_error_prev)/0.1 + Ki * e_sum;
        // vel_msg.linear.x = 0.0;
        ct_error_prev = e;
        inc_x = goal_pose.x - x_current;
        inc_y = goal_pose.y - y_current;

  
        angle_to_goal = atan2f(inc_y,inc_x);
        angle_to_goal = mod(angle_to_goal, 2 * M_PI);
        double e_angle = angle_to_goal-yaw;
        e_a_sum = e_a_sum + e_angle; 
        // e_angle = mod(e_angle, 2 * M_PI);
        vel_msg.angular.z = Kang_p*(e_angle) + Kang_d*(e_angle-ct_a_prev)/0.1 + Kang_i * e_a_sum;
        // vel_msg.angular.z = 0.0;
        ct_a_prev = e_angle;

        ROS_INFO(" current_x : %1.2f", x_current);
        ROS_INFO(" current_y : %1.2f", y_current);
        // ROS_INFO(" linear speed : %1.2f", vel_msg.linear.x);
        // ROS_INFO(" angular speed : %1.2f", vel_msg.angular.z);
        ROS_INFO(" error : %1.2f", e);

        if (vel_msg.linear.x > 0.05)
            vel_msg.linear.x = 0.05;   
        else if (vel_msg.linear.x < -0.05)
            vel_msg.linear.x = -0.05; 

        if (vel_msg.angular.z > 0.2)
            vel_msg.angular.z = 0.2;   
        else if (vel_msg.angular.z < -0.2)
            vel_msg.angular.z = -0.2; 

        velocity_publisher.publish(vel_msg);

        ros::spinOnce();
		loop_rate.sleep();
    } while(getdistance(x_current,y_current,goal_pose.x, goal_pose.y)>distance_tolerace);
    
    vel_msg.linear.x = 0.0;
    vel_msg.angular.z = 0.0;
    ROS_INFO("End goal is reached");
    velocity_publisher.publish(vel_msg);
    
}

int main(int argc, char **argv) {

    ros::init(argc, argv, "speed_controller");
    ros::NodeHandle n;
    // double speed;
    // double distance;
    // bool isForward;

    pose_subscriber = n.subscribe("/odom",10,poseCallback);
    velocity_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel",10);
 

    ROS_INFO("****TEST***\n");
    ros::Rate loop_rate(0.5);
    

    geometry_msgs::Point goal_pose;
    goal_pose.x = 0.5;
    goal_pose.y = -0.25;
    // goal_pose.theta = 0;     

    // while (ros::ok()) {
        moveGoal(goal_pose,0.05);

    // move(0.1,1.0,1);
    // rotate(0.1,1.5707,1);
    loop_rate.sleep();
    ros::spinOnce();
    // }

    return 0; 
}