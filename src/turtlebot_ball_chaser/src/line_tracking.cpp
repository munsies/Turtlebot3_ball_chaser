#include "ros/ros.h"
#include "turtlebot_ball_chaser/DriveToBall.h"
//#include <sensor_msgs/Image.h>
#include <nav_msgs/Odometry.h>
#include <tf/transform_datatypes.h>
#include <math.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

static const std::string OPENCV_WINDOW = "Image window";

using namespace std;
using namespace cv;

// vector<vector<int>> myColors = {{32,0,0,179,126,116}}; // black tape lane
// vector<vector<int>> myColors = {{0,0,0,177,90,99}}; // black tape lane
// vector<vector<int>> myColors = {{0,0,33,177,126,67}}; // black tape lane day
// vector<vector<int>> myColors = {{75,36,230,123,50,255}}; // measuring tape lane
vector<vector<int>> myColors = {{27,84,142,42,228,214}}; // measuring tape lane
double x_current, y_current, roll, pitch, yaw;


int pix = 0;

// Define global client that can request services
ros::ServiceClient client;

class SubscribeAndPublish
{
  ros::NodeHandle n_; 
  ros::NodeHandle nh_;
  image_transport::Subscriber image_sub_;  
  image_transport::ImageTransport it_;
  image_transport::Publisher image_pub_;
  ros::Subscriber pose_subscriber;
  double heading_angle_prev=0.0, heading_angle_sum=0.0;
  double t0 = ros::Time::now().toSec();

public:
  SubscribeAndPublish()
    :it_(nh_)
  {

    image_sub_ = it_.subscribe("/camera/color/image_raw", 1,&SubscribeAndPublish::process_image_callback, this);   
    image_pub_ = it_.advertise("/image_converter/output_video", 1);
    cv::namedWindow(OPENCV_WINDOW);
    // pose_subscriber = n_.subscribe("/odom", 10,&SubscribeAndPublish::poseCallback, this); 

  }

  ~SubscribeAndPublish()
  {
    cv::destroyWindow(OPENCV_WINDOW);
  }

  // Methods
ros::NodeHandle n(){ return n_;}

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

    // ROS_INFO(" roll : %1.2f", roll);
    // ROS_INFO(" pitch : %1.2f", pitch);
    // ROS_INFO(" yaw : %1.2f", yaw);
}

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    ROS_INFO_STREAM("Moving the robot to the center");
    
    // Request a service and pass the velocities to it to drive the robot
    turtlebot_ball_chaser::DriveToBall srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;

    if (!client.call(srv))
        ROS_ERROR("Failed to call service safe_move");
} 

Mat findColor(Mat img) {
    
    Mat imgHSV, mask;
    
    cvtColor(img,imgHSV, COLOR_BGR2HSV);
    
    for (size_t i = 0; i < myColors.size(); i++) {
    
        Scalar lower(myColors[i][0],myColors[i][1],myColors[i][2]);
        Scalar upper(myColors[i][3],myColors[i][4],myColors[i][5]);   

 
        inRange(imgHSV,lower,upper,mask);
    
    }
    
    return mask;
}

Mat select_region(Mat img,  vector <Point> vertices) {
    Mat final = Mat::zeros(img.size(), CV_8UC3);
    Mat imgCrop = Mat::zeros(img.size(),CV_8UC1);
//    imgCrop(vertices).setTo(0);
    fillConvexPoly(imgCrop,vertices,Scalar(255,255,255),8,0);
//    img.setTo(Vec3b(0,0,0),imgCrop);
    bitwise_and(img, img, final, imgCrop);
    
    
    return final;
}

double lane_detection (Mat img, Mat color_det) {
    
    Mat imgGrey, imgBlur, imgCanny;
    
    int height = img.size().height;
    int width = img.size().width;
    
    // std::vector <Point> region_of_int = {{0,height},{width/2,height/2},{width,height}};
    std::vector <Point> region_of_int = {{0,height},{100,100},{width-100,100},{width,height}};
    
//    cvtColor(color_det,imgGrey,COLOR_BGR2GRAY);
//        cout << "came here" << endl;
    GaussianBlur(color_det,imgBlur, Size(3,3),3,0);
    // Canny(imgBlur,imgCanny, 250, 400);
//    Canny(imgBlur,imgCanny, 300, 400);
    Canny(imgBlur,imgCanny, 25, 75);
    
    Mat mask = select_region(imgCanny,region_of_int);
    
    vector<Vec4i> linesP; // will hold the results of the detection
    HoughLinesP(mask, linesP, 6, CV_PI/60, 160, 25, 15); // runs the actual detection    
    
    Mat cdstP; 
    cdstP = img.clone();
    double error = 0;
    
    // Draw the lines
    if (linesP.size() != 0) {
    for( size_t i = 0; i < linesP.size(); i++ )
    {
        Vec4i l = linesP[i];
        line( cdstP, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA);
        int del_x = width*2/3 - (l[2]+l[0])/2; // error function
        error += del_x;
        circle(cdstP,Point((l[2]+l[0])/2, (l[3]+l[1])/2),10,Scalar(255,0,0),2);
        circle(cdstP,Point(width*2/3, (l[3]+l[1])/2),10,Scalar(0,0,0),2);
    }

        error = error/(linesP.size()*width/2); // normalize error in image
    }
    // imshow("Image lines", cdstP);
    // Update GUI Window
    cv::imshow(OPENCV_WINDOW, cdstP);
    cv::waitKey(3);
    return error;
    
}

// Process image
void process_image_callback(const sensor_msgs::ImageConstPtr& msg)
{

    cv_bridge::CvImagePtr cv_ptr;
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }
    
    Mat img = cv_ptr->image;
    double heading_angle = 0;

    Mat mask = findColor(img);
    heading_angle = lane_detection(img,mask);
    heading_angle_sum += heading_angle_sum; 

    double Kp = 5.0, Kd = 0.5, Ki = 0.0;
    float linear_x =0.05, angular_z = 0.0; // speed command
    double t1 = ros::Time::now().toSec();
    double del_t = t1 - t0;
    angular_z = Kp*(heading_angle) + Kd*(heading_angle-heading_angle_prev)/del_t \
    + Ki * heading_angle_sum;
    t0 = t1;
    cout << heading_angle << endl;

    if (angular_z > 0.2)
        angular_z = 0.2;   
    else if (angular_z < -0.2)
        angular_z = -0.2; 

    if (heading_angle == 0.0) {    
        linear_x =0.0, angular_z = 0.0;
    }
    //  linear_x =0.0, angular_z = 0.0;
    drive_robot(linear_x,angular_z);
    ROS_INFO("Moving the robot x:%1.2f, z:%1.2f", (float)linear_x,(float)angular_z);

    // Output modified video stream
    //Convert this message to a ROS sensor_msgs::Image message. 
    image_pub_.publish(cv_ptr->toImageMsg());
}



};//End of class SubscribeAndPublish

int main(int argc, char **argv)
{
  //Initiate ROS
  ros::init(argc, argv, "line_tracking");

  //Create an object of class SubscribeAndPublish that will take care of everything
  SubscribeAndPublish SAPObject;

  // Define a client service capable of requesting services from command_robot
  client = SAPObject.n().serviceClient<turtlebot_ball_chaser::DriveToBall>("/turtlebot_ball_chaser/command_robot");

  ros::spin();

  return 0;
}