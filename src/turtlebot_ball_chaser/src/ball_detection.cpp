#include "ros/ros.h"
#include "turtlebot_ball_chaser/DriveToBall.h"
//#include <sensor_msgs/Image.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

static const std::string OPENCV_WINDOW = "Image window";

using namespace std;
using namespace cv;

//vector<vector<int>> myColors = {{24,21,42,76,186,255}}; // green ball
// vector<vector<int>> myColors = {{21,58,16,66,255,255}}; // green ball
std::vector<std::vector<int>> myColors = {{25,117,68,44,255,167}}; // green ball

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

public:
  SubscribeAndPublish()
    :it_(nh_)
  {
    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    image_sub_ = it_.subscribe("/camera/color/image_raw", 1,&SubscribeAndPublish::process_image_callback, this);   
    // image_sub_ = it_.subscribe("/camera/color/image_raw", 1,&ImageConverter::imageCallback, this);
    image_pub_ = it_.advertise("/image_converter/output_video", 1);
    cv::namedWindow(OPENCV_WINDOW);
  }

  ~SubscribeAndPublish()
  {
    cv::destroyWindow(OPENCV_WINDOW);
  }

  // Methods
ros::NodeHandle n(){ return n_;}

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    ROS_INFO_STREAM("Moving the arm to the center");
    
    // Request a service and pass the velocities to it to drive the robot
    turtlebot_ball_chaser::DriveToBall srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;

    if (!client.call(srv))
        ROS_ERROR("Failed to call service safe_move");
} 

int getContours(Mat imgDil, Mat img) {
    
    std::vector<std::vector<Point>> contours;
    std::vector<Vec4i> hierarchy;
    
    findContours(imgDil, contours, hierarchy, RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
//    drawContours(img, contours,-1,Scalar(255,0,255),2);
    std::vector<std::vector<Point>> conPoly(contours.size()); // corner points
    std::vector<Rect> boundRect(contours.size());
    
    Point myPoint(0,0);
    
    for (unsigned i = 0; i < contours.size(); i++) {
        int area = contourArea(contours[i]);

        if (area > 100) {
            
            float peri = arcLength(contours[i],true);
            approxPolyDP(contours[i],conPoly[i],0.02*peri,true);
                     
            //creating bounding box around shapes
            boundRect[i] = boundingRect(conPoly[i]);
            int objCor = (int)conPoly[i].size(); 
            myPoint.x = boundRect[i].x;
            myPoint.y = boundRect[i].y;
            
            if (objCor > 5)  {
                rectangle(img,boundRect[i].tl(),boundRect[i].br(),Scalar(255,0,255),2);
            
                pix = boundRect[i].x+boundRect[i].width/2;
//            drawContours(img, conPoly,i,Scalar(0,0,0),2);
            }
            
        }
    }
        
    return pix;
}
                              
int findColor(Mat img) {
    
    Mat imgHSV;
    
    cvtColor(img,imgHSV, COLOR_BGR2HSV);
    
    for (size_t i = 0; i < myColors.size(); i++) {
    
        Scalar lower(myColors[i][0],myColors[i][1],myColors[i][2]);
        Scalar upper(myColors[i][3],myColors[i][4],myColors[i][5]);   
        
        Mat mask;
 
        inRange(imgHSV,lower,upper,mask);
        //imshow(to_string(i),mask);
        pix = getContours(mask, img);
    
    }
    
    return pix;
}
/*
int shapeContours(Mat imgDil,Mat img, vector<Rect> colorRect) {
    
    std::vector<std::vector<Point>> contours;
    std::vector<Vec4i> hierarchy;
    int ball_pixel;
    for (size_t i = 0; i<colorRect.size(); i++) {
        if (colorRect[i].x > 50 && colorRect[i].y > 50) {
            Rect roi(colorRect[i].x,colorRect[i].y,colorRect[i].width,colorRect[i].height);
            Mat imgCrop = imgDil(roi); 
            imshow("imageCrop", imgCrop);   
            findContours(imgCrop, contours, hierarchy, RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
//            drawContours(img, contours,-1,Scalar(255,0,255),2);
            std::vector<std::vector<Point>> conPoly(contours.size()); // corner points
            std::vector<Rect> boundRect(contours.size());
            std::string objectType;
    
            for (unsigned m = 0; m < contours.size(); m++) {
                int area = contourArea(contours[m]);
//              std::cout << area << std::endl; 
                if (area > 1000) {
            
                    float peri = arcLength(contours[m],true);
                    approxPolyDP(contours[m],conPoly[m],0.02*peri,true);
//                  drawContours(img, conPoly,m,Scalar(255,0,255),2);
//                  std::cout<<conPoly[m].size()<<std::endl;
            
                    //creating bounding box around shapes
                    boundRect[m] = boundingRect(conPoly[m]);
                    int objCor = (int)conPoly[m].size();           
            
                    if (objCor > 11)  {
                    objectType = "Circle";
            
                    rectangle(img,colorRect[i].tl()+boundRect[m].tl(),colorRect[i].br(),Scalar(0,0,255),2);
//                  putText(img,objectType,{boundRect[m].x+5,boundRect[m].y+20},FONT_ITALIC,0.5,Scalar(50,50,50),1);
            
                    ball_pixel = colorRect[i].x+colorRect[i].width/2; 
                    }
                }
            }
        }
    }
//    drawContours(img, contours,-1,Scalar(255,0,255),2);
    return ball_pixel;
}
*/
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

    bool left = false, middle = false, right = false; // image regions
    float linear_x =0.0, angular_z = 0.0; // speed command
    Mat img = cv_ptr->image;
    pix = 0;
    //color detection
    pix = findColor(img);
    int width = img.size().width;
    // Update GUI Window
    cv::imshow(OPENCV_WINDOW, img);
    cv::waitKey(3);
    ROS_INFO("Ball lenght:%d", width);
    ROS_INFO("Ball found at this pixel:%d", pix);
    if(pix > 10 && pix < (int)width/3) {
        left = true;
    }
    else if(pix >= (int)width/3 && pix < (int)width*2/3) {
        middle = true;
    }
    else if(pix >= (int)width*2/3 && pix < (int)width-10) {
        right = true;
    }
    else {
        left = false;
        middle = false;
        right = false;
    } 
    

    if (left) {linear_x = 0; angular_z = 0.1;}
    else if (right) {linear_x = 0; angular_z = -0.1;}
    else if (middle){linear_x = 0.07; angular_z = 0;}
    else {linear_x = 0.0;angular_z = 0.0;}

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
  ros::init(argc, argv, "ball_detection");

  //Create an object of class SubscribeAndPublish that will take care of everything
  SubscribeAndPublish SAPObject;

  // Define a client service capable of requesting services from command_robot
  client = SAPObject.n().serviceClient<turtlebot_ball_chaser::DriveToBall>("/turtlebot_ball_chaser/command_robot");

  ros::spin();

  return 0;
}