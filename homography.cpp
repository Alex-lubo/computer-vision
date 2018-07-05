#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace cv;
using namespace std;

vector<Point2f> pts_src;
vector<Point2f> pts_dst;
int actionlock;
Mat im_src, im_dst;

void getHomoPoint(int action, int x, int y, int flag, void* userdata) {
  if (actionlock < 4 && action == EVENT_LBUTTONUP) {
    pts_src.push_back(Point2f(x, y));
    actionlock += 1;
    circle(im_src, Point(x, y), 1, Scalar(255, 255, 0), 1, CV_AA);
    imshow("window", im_src);
  } 
  if (actionlock == 4)
  {
    Mat H = findHomography(pts_src, pts_dst);
    warpPerspective(im_src, im_dst, H, Size(300, 400));
    imshow("destination", im_dst);
  }
}

int main(int argc, char* argv[])
{
  actionlock = 0;
  pts_dst.push_back(Point2f(0, 0));
  pts_dst.push_back(Point2f(299, 0));
  pts_dst.push_back(Point2f(299, 399));
  pts_dst.push_back(Point2f(0, 399));
  
  Mat dummy;

  im_src = imread("book2.jpg");
  if(im_src.empty())
  {
    printf("failed to load jpg 2");
    return EXIT_FAILURE;
  }
  dummy = im_src.clone();
  namedWindow("window", WINDOW_AUTOSIZE);
  namedWindow("destination", WINDOW_AUTOSIZE);
  setMouseCallback("window", getHomoPoint);
  int k = 0;
  while(k != 27)
  {
    imshow("window", im_src);
    putText(im_src, "choose Four Point of the rectangle, Press ESC to exit and c to reset", Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255, 255, 255), 2);
    k = waitKey(20);
    if (k == 99)
    {
      pts_src.clear();
      actionlock = 0;
      dummy.copyTo(im_src);
    }
  }
  return EXIT_SUCCESS;
}