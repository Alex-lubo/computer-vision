#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace cv;
using namespace std;

const float GOOD_MATCH_PERCENT = 0.05f;

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
    warpPerspective(im_src, im_dst, H, Size(600, 400));
    imshow("destination", im_dst);
  }
}

void alignImages(Mat &im1, Mat &im2, Mat &im1Reg, Mat &h)
{
  Mat im1Gray, im2Gray;
  cvtColor(im1, im1Gray, CV_BGR2GRAY);
  cvtColor(im2, im2Gray, CV_BGR2GRAY);
  std::vector<KeyPoint> keypoints1, keypoints2;
  Mat descriptors1, descriptors2;

  ORB orb(600, 1.2f, 4, 31, 0, 2, ORB::FAST_SCORE, 31);
  orb.detect(im1Gray, keypoints1);
  orb.compute(im1Gray, keypoints1, descriptors1);
  orb.detect(im2Gray, keypoints2);
  orb.compute(im2Gray,keypoints2, descriptors2);

  BFMatcher matcher;
  std::vector<DMatch> matches;
  Mat img_matches;
  printf("0-------");
  if(!descriptors1.empty() && !descriptors2.empty()) {
    printf("1-------");
    matcher.match(descriptors1, descriptors2, matches, Mat());
    std::sort(matches.begin(), matches.end());
    const int numGoodMatches = matches.size()*GOOD_MATCH_PERCENT;
    matches.erase(matches.begin()+numGoodMatches, matches.end());
    drawMatches(im1, keypoints1, im2, keypoints2, matches, img_matches);
  }
  if(!img_matches.empty()){
    printf("2-------");
    std::vector<Point2f> points1, points2;
    for(size_t i=0;i<matches.size();i++){
      points1.push_back(keypoints1[matches[i].queryIdx].pt);
      points2.push_back(keypoints2[matches[i].trainIdx].pt);
    }
    h = findHomography(points1, points2, RANSAC);
    warpPerspective(im1, im1Reg, h, im2.size());
    imwrite("matches.jpg", img_matches);
  }
}

int main(int argc, char* argv[])
{
  if (argc < 2){
    string result_msg = format("usage: %s imgfile [-a].", argv[0]);
    printf("%s", result_msg.c_str());
    return EXIT_FAILURE;
  }
  bool autoAlign = 0;
  printf("argc: %d, argv[1]: %s",argc, argv[1]);
  if (argc == 4 && strcmp(argv[1],"-a") == 0) {
    autoAlign = 1;
  }
  if (autoAlign) 
  {
    Mat imReg, h;
    Mat im1 = imread(argv[2], IMREAD_COLOR);
    Mat im2 = imread(argv[3], IMREAD_COLOR);
    if (im1.empty() or im2.empty()){
      printf("failed to load image \n");
      return EXIT_FAILURE;
    }
    printf("begin---");
    alignImages(im1, im2, imReg, h);
    imwrite("alignedbook.jpg", imReg);
  } 
  else 
  {
    actionlock = 0;
    pts_dst.push_back(Point2f(0, 0));
    pts_dst.push_back(Point2f(599, 0));
    pts_dst.push_back(Point2f(599, 399));
    pts_dst.push_back(Point2f(0, 399));
    
    Mat dummy;
    // Mat fitMat;

    im_src = imread(argv[1], IMREAD_COLOR);
    if(im_src.empty())
    {
      printf("failed to load imagefile");
      return EXIT_FAILURE;
    }
    if(im_src.cols > 1024 || im_src.rows> 768){
      // Size dsize = Size(im_src.cols*0.4, im_src.rows*0.4);
      resize(im_src, im_src, Size(im_src.cols/2, im_src.rows/2), 0, 0, CV_INTER_LINEAR);
    }
    dummy = im_src.clone();
    namedWindow("window", 0);
    namedWindow("destination", 0);
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
  }
  return EXIT_SUCCESS;
}