#include <string>
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

using namespace cv;

int main(int argc, char* argv[])
{
  string outText;

  tesseract::TessBaseAPI *ocr = new tesseract::TessBaseAPI();
  if(ocr->Init(NULL, "eng", tesseract::OEM_DEFAULT)) {
    printf("Could not initialize tesseract.\n");
    return EXIT_FAILURE;
  };
  ocr->SetPageSegMode(tesseract::PSM_AUTO);
  
  Mat m = imread(argv[1], IMREAD_COLOR);
  if(m.empty()) {
    printf("No image data \n");
    return EXIT_FAILURE;
  }
  ocr->SetImage(m.data, m.cols, m.rows, 3, m.step);
  outText = string(ocr->GetUTF8Text());
  std::cout<<outText<<std::endl;
  ocr->End();
  return EXIT_SUCCESS;
}