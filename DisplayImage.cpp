#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char* argv[]) {
  if(argc != 2) {
    printf("usage: DisplayImage.out <Image_Path>\n");
    return -1;
  }

  Mat image = imread(argv[1], 1);
  if(image.empty()) {
    printf("No image data \n");
    return EXIT_FAILURE;
  }

  double scaleX = 0.6;
  double scaleY = 0.6;
  double angle = 30;
  double scale = 1;
  Point2f center(image.cols/2, image.rows/2);

 
  Mat grayImage, scaleDown, scaleUp, rotateM, M;
  cvtColor(image, grayImage, COLOR_BGR2GRAY);
  resize(image, scaleDown, Size(), scaleX, scaleY, INTER_LINEAR);
  resize(image, scaleUp, Size(), scaleX*4, scaleY*4, INTER_LINEAR);
  Mat crop = image(Range(30, 140), Range(50, 130));
  M = getRotationMatrix2D(center, angle, scale);
  warpAffine(image, rotateM, M, Size(image.cols, image.rows));

  namedWindow("Image", WINDOW_AUTOSIZE);
  namedWindow("Gray-Image", WINDOW_NORMAL);
  namedWindow("scaleDown", WINDOW_AUTOSIZE);
  namedWindow("scaleUp", WINDOW_AUTOSIZE);
  namedWindow("crop", WINDOW_AUTOSIZE);
  namedWindow("rotate", WINDOW_AUTOSIZE);

  imshow("Image", image);
  imshow("Gray-Image", grayImage);
  imshow("scaleDown", scaleDown);
  imshow("scaleUp", scaleUp);
  imshow("crop", crop);
  imshow("rotate", rotateM);
  waitKey(0);
  return EXIT_SUCCESS;
}