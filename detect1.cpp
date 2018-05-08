#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char* argv[])
{
  int height, width;
  int t1min = 0,t1max = 0,t2min = 0,t2max = 0,t3min = 0,t3max = 0; // 过滤条件值，默认为０

  CvMat* threshold_matrix  = cvCreateMat(2 ,3 ,CV_32FC1);
  CvFileStorage* temp = cvOpenFileStorage("threshold_conf.xml",NULL,CV_STORAGE_READ);

  // 如果有就加载阈值
  if(temp)
  {
    threshold_matrix = (CvMat*)cvLoad("threshold_conf.xml");
    t1min = (int)CV_MAT_ELEM(*threshold_matrix, float, 0, 0);
    t2min = (int)CV_MAT_ELEM(*threshold_matrix, float, 0, 1);
    t3min = (int)CV_MAT_ELEM(*threshold_matrix, float, 0, 2);
    t1max = (int)CV_MAT_ELEM(*threshold_matrix, float, 1, 0);
    t2max = (int)CV_MAT_ELEM(*threshold_matrix, float, 1, 1);
    t3max = (int)CV_MAT_ELEM(*threshold_matrix, float, 1, 2);
  }

  CvCapture* capture = cvCaptureFromCAM(0);

  if( !capture )
  {
    std::cout<< "ERROR: capture is NULL" <<std::endl;
    return -1;
  }

  IplImage* frame = cvQueryFrame( capture );

  // 创建窗口显示变换结果
  cvNamedWindow( "Camera", CV_WINDOW_AUTOSIZE );
  cvNamedWindow( "HSV", CV_WINDOW_AUTOSIZE );
  cvNamedWindow( "F1", CV_WINDOW_AUTOSIZE );
  cvNamedWindow( "F2", CV_WINDOW_AUTOSIZE );
  cvNamedWindow( "F3", CV_WINDOW_AUTOSIZE );

  cvCreateTrackbar("t1min", "F1", &t1min, 260 , NULL );
  cvCreateTrackbar("t1max", "F1", &t1max, 260,  NULL  );
  
  cvCreateTrackbar("t2min", "F2", &t2min, 260 , NULL );
  cvCreateTrackbar("t2max", "F2", &t2max, 260,  NULL  );
  
  cvCreateTrackbar("t3min", "F3", &t3min, 260 , NULL );
  cvCreateTrackbar("t3max", "F3", &t3max, 260,  NULL  );

  CvScalar hsv_min = cvScalar(t1min, t2min, t3min, 0);
  CvScalar hsv_max = cvScalar(t1max, t2max ,t3max, 0);

  // 获取frame的size
  height    = frame->height;
  width     = frame->width;
  CvSize size = cvSize(width,height);

  // 初始化动态检测需要用到的图像
  IplImage*  hsv_frame    = cvCreateImage(size, IPL_DEPTH_8U, 3);   // HSV 图片
  IplImage*  thresholded   = cvCreateImage(size, IPL_DEPTH_8U, 1);  // 总阈值过滤后的图片
  IplImage*  thresholded1   = cvCreateImage(size, IPL_DEPTH_8U, 1); // t1色调过滤后的图片，
  IplImage*  thresholded2   = cvCreateImage(size, IPL_DEPTH_8U, 1); // t2饱和度过滤后的图片
  IplImage*  thresholded3   = cvCreateImage(size, IPL_DEPTH_8U, 1); // t3亮度过滤

  while( 1 )
  {   
    // 如果条件改变，重新获取阈值
    hsv_min = cvScalar(t1min, t2min, t3min, 0);
    hsv_max = cvScalar(t1max, t2max ,t3max, 0);

    // Get one frame
    frame = cvQueryFrame( capture );
    
    if( !frame )
    {
      std::cout<< "ERROR: frame is null" <<std::endl;
      break;
    }

    // 从RGB 转　HSV
    cvCvtColor(frame, hsv_frame, CV_BGR2HSV);
    
    // 过滤.
    cvInRangeS(hsv_frame, hsv_min, hsv_max, thresholded);

    cvSplit( hsv_frame,thresholded1, thresholded2, thresholded3, NULL );
    
    cvInRangeS(thresholded1,cvScalar(t1min,0,0,0) ,cvScalar(t1max,0,0,0) ,thresholded1);
    cvInRangeS(thresholded2,cvScalar(t2min,0,0,0) ,cvScalar(t2max,0,0,0) ,thresholded2);
    cvInRangeS(thresholded3,cvScalar(t3min,0,0,0) ,cvScalar(t3max,0,0,0) ,thresholded3);

    CvMemStorage* storage = cvCreateMemStorage(0); // 0 - 64k
    
    cvSmooth(thresholded, thresholded, CV_GAUSSIAN, 5, 5);
    
    CvSeq* circles = cvHoughCircles(thresholded, storage, CV_HOUGH_GRADIENT, 2,
                              thresholded->height/4, 100, 50, 30, 300);

    for (int i = 0; i < circles->total; i++)
    { 
      float* p = (float*)cvGetSeqElem( circles, i );
      printf("Ball! x=%f y=%f r=%f\n\r",p[0],p[1],p[2] );
      
      cvCircle( frame, cvPoint(cvRound(p[0]),cvRound(p[1])),  //plot centre
                              2, CV_RGB(255,255,255), -1, 8, 0);
      cvCircle( frame, cvPoint(cvRound(p[0]), cvRound(p[1])),  //plot circle
                              cvRound(p[2]), CV_RGB(0, 255, 0), 2, 8, 0);
    }
    
    // show images in windows
    cvShowImage( "Camera", frame );
    cvShowImage( "HSV", hsv_frame);
    cvShowImage( "After Color Filtering", thresholded );
    cvShowImage( "F1", thresholded1 );
    cvShowImage( "F2", thresholded2 );
    cvShowImage( "F3", thresholded3 );
      
    cvReleaseMemStorage(&storage);
  
    if( (cvWaitKey(10) & 255) == 27 ) // esc pressed
      break;
  }
    
  //Save the threshold values before exiting
  *((float*)CV_MAT_ELEM_PTR( *threshold_matrix, 0, 0 ) ) = t1min;
  *((float*)CV_MAT_ELEM_PTR( *threshold_matrix, 0, 1 ) ) = t2min;
  *((float*)CV_MAT_ELEM_PTR( *threshold_matrix, 0, 2 ) ) = t3min;
  *((float*)CV_MAT_ELEM_PTR( *threshold_matrix, 1, 0 ) ) = t1max;
  *((float*)CV_MAT_ELEM_PTR( *threshold_matrix, 1, 1 ) ) = t2max;
  *((float*)CV_MAT_ELEM_PTR( *threshold_matrix, 1, 2 ) ) = t3max;

  cvSave("threshold_matrix.xml",threshold_matrix);
  
  // Release the capture device housekeeping
  cvReleaseCapture( &capture );
  cvDestroyWindow( "mywindow" );
  return 0;
}