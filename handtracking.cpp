#include <stdio.h>
#include <opencv2/opencv.hpp>

float angle() {
  
}

int main(int argc, char* argv[]) 
{
  int h=100, s=100, v=100;
  //get capture from camera
  CvCapture* capture = cvCaptureFromCAM(0);
  cvNamedWindow("handtracking");
  cvCreateTrackbar("H", "handtracking", &h, 179, NULL);
  cvCreateTrackbar("S", "handtracking", &s, 255, NULL);
  cvCreateTrackbar("V", "handtracking", &v, 255, NULL);
}