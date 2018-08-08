#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace cv;

#define PERSPECTIVESIZE 0.2 //透视矩阵每次的变化率
#define RESIZESTEPSIZE 0.1  //缩小放大的变化率
#define ROTATESETPSIZE 10   //旋转图像时旋转的角度（逆时针）

float g_H_value = 0.;
float g_Resize_value = 0.;
int g_Rotate_degree = 0.;

void zoom_OUTorIN(IplImage *dst_imresize, float g_resize_value)
{
  CvSize dstSize = cvSize(dst_imresize->width * (1 + g_resize_value), dst_imresize->height * (1 + g_resize_value));
  IplImage *imageresize = cvCreateImage(dstSize, dst_imresize->depth, dst_imresize->nChannels);
  cvResize(dst_imresize, imageresize, CV_INTER_LINEAR);
  cvReleaseImage(&dst_imresize);
  dst_imresize = cvCloneImage(imageresize);
  cvReleaseImage(&imageresize);
}

void rotateImage_CWorCCW(IplImage *dst_rotate, float g_RotateDegree)
{
  IplImage *imagerotate = cvCreateImage(cvGetSize(dst_rotate), dst_rotate->depth, dst_rotate->nChannels);

  // 旋转中心
  CvPoint2D32f center;
  center.x = float(dst_rotate->width / 2.0 + 0.5);
  center.y = float(dst_rotate->height / 2.0 + 0.5);
  // 计算二维旋转的仿射变换矩阵
  float m[6];
  CvMat M = cvMat(2, 3, CV_32F, m);
  cv2DRotationMatrix(center, g_RotateDegree, 1, &M);
  // 变换图像，并用黑色填充其余值
  cvWarpAffine(dst_rotate, imagerotate, &M, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS, cvScalarAll(0));
  cvReleaseImage(&dst_rotate);
  dst_rotate = cvCloneImage(imagerotate);
  cvReleaseImage(&imagerotate);
}

//图像透视投影
void changeg_H_Plus(CvMat *H, char d, float g_H_value)
{
  if (d == 49)
    cvSetReal2D(H, 0, 0, cvGetReal2D(H, 0, 0) + g_H_value);
  if (d == 50)
    cvSetReal2D(H, 0, 1, cvGetReal2D(H, 0, 1) + g_H_value);
  if (d == 51)
    cvSetReal2D(H, 0, 2, cvGetReal2D(H, 0, 2) + g_H_value);
  if (d == 52)
    cvSetReal2D(H, 1, 0, cvGetReal2D(H, 1, 0) + g_H_value);
  if (d == 53)
    cvSetReal2D(H, 1, 1, cvGetReal2D(H, 1, 1) + g_H_value);
  if (d == 54)
    cvSetReal2D(H, 1, 2, cvGetReal2D(H, 1, 2) + g_H_value);
  if (d == 55)
    cvSetReal2D(H, 2, 0, cvGetReal2D(H, 2, 0) + g_H_value);
  if (d == 56)
    cvSetReal2D(H, 2, 1, cvGetReal2D(H, 2, 1) + g_H_value);
  if (d == 57)
    cvSetReal2D(H, 2, 2, cvGetReal2D(H, 2, 2) + g_H_value);
}

void WarpPerspective_PLUSorSUB(IplImage *src, IplImage *dst, CvMat *H, char d, float g_H_value)
{
  changeg_H_Plus(H, d, g_H_value);
  IplImage *dst_Perspective = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
  cvWarpPerspective(src, dst_Perspective, H, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS, cvScalarAll(0));
  cvReleaseImage(&dst);
  dst = cvCloneImage(dst_Perspective);
}

void WarpPerspective(IplImage *src, IplImage *dst, const CvMat warp_matrix, char d)
{
  CvMat *H = cvCloneMat(&warp_matrix);
  cvNamedWindow("Perspective_H_Chagne", 1);

  while (1)
  {
    cvShowImage("Perspective_H_Chagne", dst);
    char e = cvWaitKey(10);

    switch (e)
    {
    case 61:
      g_H_value = PERSPECTIVESIZE;
      WarpPerspective_PLUSorSUB(src, dst, H, d, g_H_value);
      break;
    case 45:
      g_H_value = -PERSPECTIVESIZE;
      WarpPerspective_PLUSorSUB(src, dst, H, d, g_H_value);
      break;
    case 27:
      break;
    }
    if (e == 27)
      break;
  }
  cvDestroyWindow("Perspective_H_Chagne");
  g_H_value = 0;
  cvReleaseImage(&dst);
  dst = cvCloneImage(src);
}

int main(int argc, char **argv)
{
  IplImage *src = cvLoadImage("girl.jpg", CV_LOAD_IMAGE_UNCHANGED);

  if (!src)
  {
    printf("load image error.\texit\n");
    return -1;
  }
  //初始化H矩阵
  float a[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
  const CvMat warp_matrix = cvMat(3, 3, CV_32FC1, a);
  //初始化缩放、旋转、透视投影原始矩阵
  IplImage *dst_rotate, *dst_imresize, *dst_Perspective;
  dst_rotate = cvCloneImage(src);
  dst_imresize = cvCloneImage(src);
  dst_Perspective = cvCloneImage(src);

  printf("input z means zoom in or zoom out\n");
  printf("input r means rotate\n");
  printf("input p means perspective\n");

  cvNamedWindow("Source_Image", 1);

  while (1)
  {
    cvShowImage("Source_Image", src);
    char c = cvWaitKey(10);

    switch (c)
    {
      // ESC
    case 27:
      break;
      // 122 -- 放大缩小图像
    case 90:
    {
      cvNamedWindow("Resize_Image", 1);
      while (1)
      {
        cvShowImage("Resize_Image", dst_imresize);
        char d = cvWaitKey(10);

        switch (d)
        {
        case 61:
          g_Resize_value = RESIZESTEPSIZE;
          zoom_OUTorIN(dst_imresize, g_Resize_value);
          break;
        case 45:
          g_Resize_value = -RESIZESTEPSIZE;
          zoom_OUTorIN(dst_imresize, g_Resize_value);
          break;
        case 27:
          break;
        }

        if (d == 27)
          break;
      }
      cvDestroyWindow("Resize_Image");
      g_Resize_value = 0;
      cvReleaseImage(&dst_imresize);
      dst_imresize = cvCloneImage(src);
      break;
    }
      //114 -- 旋转图像
    case 114:
    {
      cvNamedWindow("Rotate_Image", 1);
      while (1)
      {
        cvShowImage("Rotate_Image", dst_rotate);
        char d = cvWaitKey(10);

        switch (d)
        {
        case 107:
          g_Rotate_degree += ROTATESETPSIZE;
          rotateImage_CWorCCW(dst_rotate, g_Rotate_degree);
          break;
        case 109:
          g_Rotate_degree -= ROTATESETPSIZE;
          rotateImage_CWorCCW(dst_rotate, g_Rotate_degree);
          break;
        case 27:
          break;
        }

        if (d == 27)
          break;
      }
      cvDestroyWindow("Rotate_Image");
      g_Rotate_degree = 0;
      cvReleaseImage(&dst_rotate);
      dst_rotate = cvCloneImage(src);
      break;
    }

      //112 -- 透射变换
    case 80:
    {
      cvNamedWindow("Perspective_Image", 1);
      cvShowImage("Perspective_Image", dst_Perspective);
      while (1)
      {
        char d = cvWaitKey(10);

        switch (d)
        {
        case 49:
          WarpPerspective(src, dst_Perspective, warp_matrix, d);
          break;
        case 50:
          WarpPerspective(src, dst_Perspective, warp_matrix, d);
          break;
        case 51:
          WarpPerspective(src, dst_Perspective, warp_matrix, d);
          break;
        case 52:
          WarpPerspective(src, dst_Perspective, warp_matrix, d);
          break;
        case 53:
          WarpPerspective(src, dst_Perspective, warp_matrix, d);
          break;
        case 54:
          WarpPerspective(src, dst_Perspective, warp_matrix, d);
          break;
        case 55:
          WarpPerspective(src, dst_Perspective, warp_matrix, d);
          break;
        case 56:
          WarpPerspective(src, dst_Perspective, warp_matrix, d);
          break;
        case 57:
          WarpPerspective(src, dst_Perspective, warp_matrix, d);
          break;
        case 27:
          break;
        }
        if (d == 27)
          break;
      }
      cvDestroyWindow("Perspective_Image");
      break;
    }
    }

    if (c == 27)
      break;
  }

  cvReleaseImage(&src);
  cvReleaseImage(&dst_rotate);
  cvReleaseImage(&dst_imresize);
  cvReleaseImage(&dst_Perspective);
  cvDestroyAllWindows();

  return 0;
}