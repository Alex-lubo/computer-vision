#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// string lcs(string &str1, string &str2) {
//   string maxStr, minStr;
//   string res = "";
//   if(str1.length() > str2.length()){
//     maxStr = str1;
//     minStr = str2;
//   } else {
//     maxStr = str2;
//     minStr = str1;
//   }
//   for(size_t i = 0; i< minStr.length(); i++) {
//     for(size_t j = 0, z=minStr.length() -i;z!=minStr.length()+1;j++,z++){
//       res = minStr.substr(j,z);
//       if(maxStr.find(res)!=string::npos)
//         return res;
//     }
//   }
//   return "";
// }

int main(int argc, char* argv[]) {
  Mat img(300, 400, CV_8UC3, Scalar(0, 0,0));
  IplImage* img2 = cvCreateImage(cvSize(400, 300), IPL_DEPTH_8U, 3);
  cvZero(img2);
  // IplImage* img2 = (IplImage)img;
  circle(img, Point(100, 150), 50, 135, 2,8, 0);
  namedWindow("tet", CV_WINDOW_AUTOSIZE);
  imshow("tet", img);
  int top = 20,left=5,bottom=100, right=120;
  // draw plant
  // for(int row = top;row <bottom;row++){
  //   for(int col = left;col <right;col++){
  //     *(cvPtr2D(img2, row, col)+1)=255;
  //   }
  // }
  
  // draw rectangle
  for(int col = left;col <right;col++){
    *(cvPtr2D(img2, top, col)+1)=255;
    *(cvPtr2D(img2, bottom, col)+1)=255;
  }
  for(int row = top;row <bottom;row++){
    *(cvPtr2D(img2, row, left)+1)=255;
    *(cvPtr2D(img2, row, right)+1)=255;
  }
  namedWindow("tet1", CV_WINDOW_AUTOSIZE);
  cvShowImage("tet1", img2);

// 3.5题
  IplImage* img3 = cvCreateImage(cvSize(400, 400), IPL_DEPTH_8U, 1);
  cvSetZero(img3);
  int interset_x = 0;
  int interset_y = 0;
  int offset_x = img3->width;
  int offset_y = img3->height;
  int add = 0;
  bool cond = true;
  while(add <200) {
    cvSetImageROI(img3, cvRect(interset_x, interset_y, offset_x, offset_y));
    if(cond) {
      cvSet(img3, Scalar(0));
      cond = false;
    } else {
      cvSet(img3, Scalar(add));
      cond = true;
    }
    cvResetImageROI(img3);
    interset_x += 10;
    interset_y += 10;
    offset_x -= 20;
    offset_y -= 20;
    add += 10;
  }
  namedWindow("tet3", CV_WINDOW_AUTOSIZE);
  cvShowImage("tet3", img3);

  //3.7题
  IplImage *img4 = cvLoadImage("girl.jpg");
  IplImage *img4_r = cvCreateImage(cvSize(img4->width, img4->height), img4->depth,1);
  IplImage *img4_g = cvCreateImage(cvSize(img4->width, img4->height), img4->depth,1);
  IplImage *img4_b = cvCreateImage(cvSize(img4->width, img4->height), img4->depth,1);
  cvSplit(img4, img4_r, img4_g, img4_b, NULL);
  cvNamedWindow("green-c");
  cvShowImage("green-c", img4_g);
  IplImage *clone1 = cvCloneImage(img4_g);
  IplImage *clone2 = cvCloneImage(img4_g);
  double max_val, min_val;
  cvMinMaxLoc(img4_g, &min_val, &max_val);
  double  scalar = (max_val - min_val)/2.;
  cvSet(clone1, Scalar(scalar));
  cvSetZero(clone2);
  cvCmp(img4_g, clone1, clone2, CV_CMP_GE);
  cvNamedWindow("mask");
  cvShowImage("mask",clone2);
  cvSubS(img4_g, cvScalar(scalar/2), img4_g, clone2);
  cvNamedWindow("subs");
  cvShowImage("subs",img4_g);
  cvWaitKey(0);

  cvReleaseImage(&img2);
  cvReleaseImage(&img3);
  cvReleaseImage(&img4);
  cvReleaseImage(&img4_r);
  cvReleaseImage(&img4_g);
  cvReleaseImage(&img4_b);
  cvDestroyAllWindows();
  return EXIT_SUCCESS;
}