#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace cv;

Point center, circumference;
Mat source;

void drawCircle(int action, int x, int y, int flags, void *userData) 
{
  if (action == EVENT_LBUTTONDOWN)
  {
    center = Point(x, y);
    circle(source, center, 1, Scalar(255, 255, 0), 2, CV_AA);
  }
  else if (action == EVENT_LBUTTONUP)
  {
    circumference = Point(x, y);
    float radius = sqrt(pow(center.x - circumference.x, 2) + pow(center.y - circumference.y, 2));
    circle(source, center, radius, Scalar(0, 255, 0), 2, CV_AA);
    imshow("windows", source);
  }
}

int main(int argc, char* argv[])
{
  source = imread(argv[1], 1);
  if(source.empty()) {
    printf("failed to load image \n");
    return EXIT_FAILURE;
  }

  Mat dummy = source.clone();
  namedWindow("windows");
  setMouseCallback("windows", drawCircle);
  int k = 0;
  while(k!=27)
  {
    imshow("windows", source);
    putText(source, "choose a center and drag, Press ESC to exit and c to clear", Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255, 255, 255), 2);
    k = waitKey(20) & 0xFF;
    if (k == 99)
      dummy.copyTo(source);
  }
}