// #include <iostream.h>
#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace cv;

String faceCascadePath;
CascadeClassifier faceCascade;

int main(int argc, char* argv[]) 
{
  int faceNeighborsMax = 10;
  int neighborStep = 1;
  faceCascadePath = "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt2.xml";
  if (!faceCascade.load(faceCascadePath))
  {
    printf("failed to load face cascade");
    return EXIT_FAILURE;
  }

  std::vector<Rect> faces;
  Mat frame = imread("first-image.jpg");
  Mat frameGray, frameClone;
  cvtColor(frame, frameGray, COLOR_BGR2GRAY);
  for(int neigh = 0; neigh < faceNeighborsMax;neigh += neighborStep)
  {
    frameClone = frame.clone();
    faceCascade.detectMultiScale(frameGray, faces, 1.2, neigh);
    for(size_t i = 0; i< faces.size();i++)
    {
      rectangle(frameClone, faces[i], Scalar(255, 0, 0), 2, 4);
    }
    putText(frameClone, format("# Neighbors = %d", neigh), Point(10, 40), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 4);
    imshow("face detection", frameClone);
    int k = waitKey(0);
    if (k == 27) {
      destroyAllWindows();
      break;
    }
  }
  return EXIT_SUCCESS;
}