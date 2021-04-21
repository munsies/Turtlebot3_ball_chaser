#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;

static const std::string OPENCV_WINDOW = "Image window";
int frame_width = 1280;
int frame_height = 720;
VideoWriter video("BallTrack.avi", cv::VideoWriter::fourcc('M','J','P','G'), 30, Size(frame_width,frame_height));

int pix = 0;
//vector<vector<int>> myColors = {{24,21,42,76,186,255}}; // green ball
// std::vector<std::vector<int>> myColors = {{21,58,16,66,255,255}}; // green ball
std::vector<std::vector<int>> myColors = {{25,117,68,44,255,167}}; // green ball

class ImageConverter
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;

public:
  ImageConverter()
    : it_(nh_)
  {
    // Subscribe to input video feed and publish output video feed
    image_sub_ = it_.subscribe("/camera/color/image_raw", 1,&ImageConverter::imageCallback, this);
    image_pub_ = it_.advertise("/image_converter/output_video", 1);

    cv::namedWindow(OPENCV_WINDOW);
  }

  ~ImageConverter()
  {
    cv::destroyWindow(OPENCV_WINDOW);
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
  
  void imageCallback(const sensor_msgs::ImageConstPtr& msg)
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

    // Draw an example circle on the video stream
    if (cv_ptr->image.rows > 60 && cv_ptr->image.cols > 60)
      cv::circle(cv_ptr->image, cv::Point(50, 50), 10, CV_RGB(255,0,0));

    cv::Mat img = cv_ptr->image;
    pix = 0;
    //color detection
    pix = findColor(img);
    video.write(img);
    // Update GUI Window
    cv::imshow(OPENCV_WINDOW, img);
    cv::waitKey(3);
    ROS_INFO("image height:%d", img.size().height);
    ROS_INFO("image width:%d", img.size().width);
    ROS_INFO("Ball found at this pixel:%d", pix);

    // Output modified video stream
    //Convert this message to a ROS sensor_msgs::Image message. 
    image_pub_.publish(cv_ptr->toImageMsg());
  }

};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_pub_sub");
  ImageConverter ic;
  ros::spin();
  return 0;
}