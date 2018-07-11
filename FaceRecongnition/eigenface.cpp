#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace cv;

static Mat norm_0_255(InputArray _src) {
  Mat src = _src.getMat();
  Mat dst;
  switch(src.channels()){
    case 1:
      normalize(src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
      break;
    case 3:
      normalize(src, dst, 0, 255, NORM_MINMAX, CV_8UC3);
      break;
    default:
      src.copyTo(dst);
      break;
  }
  return dst;
}

static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator= ';') {
  std::cout<<filename<<std::endl;
  std::ifstream file(filename.c_str(), std::ifstream::in);
  if(!file) {
    string error_msg = "No valid input file,please check the given filename";
    CV_Error(CV_StsBadArg, error_msg);
  }
  string line, path, classlabel;
  while(std::getline(file, line)) {
    std::stringstream liness(line);
    getline(liness, path, separator);
    getline(liness, classlabel);
    if(!path.empty() && !classlabel.empty()) {
      images.push_back(imread(path, 0));
      labels.push_back(atoi(classlabel.c_str()));
    }
  }
}

int main(int argc, char* argv[]) {
  if(argc < 2) {
    std::cout <<"usage: "<<argv[0]<<" <csv.ext> <output_folder> "<<std::endl;
    exit(EXIT_FAILURE);
  }

  string output_folder = ".";
  if (argc == 3) {
    output_folder = argv[2];
  }

  vector<Mat> images;
  vector<int> labels;
  string fn_csv = string(argv[1]);
  try {
    read_csv(fn_csv, images, labels);
  } catch (Exception& e) {
    std::cerr <<"Error opening file \""<<fn_csv<<"\". Reason: "<<e.msg<<std::endl;
    exit(EXIT_FAILURE);
  }
  if(images.size() <= 1) {
      string error_msg = "This demo needs at least 2 images to work. Please add more images to your data set!";
      CV_Error(CV_StsError, error_msg);
  }

  // trainning data and test data
  int height = images[0].rows;
  Mat testSample = images[images.size() - 1];
  int testLabel = labels[labels.size() - 1];
  images.pop_back();
  labels.pop_back();

  Ptr<FaceRecognizer> model = createEigenFaceRecognizer();
  model->train(images, labels);

  int predictedLabel = model->predict(testSample);
  string result_msg = format("Predicted class = %d /actual class =%d.", predictedLabel, testLabel);
  std::cout<<result_msg<<std::endl;

  // analysis
  Mat eigenvalues = model->getMat("eigenvalues");
  Mat W = model->getMat("eigenvectors");
  Mat mean = model->getMat("mean");
  if(argc == 2) {
    imshow("mean", norm_0_255(mean.reshape(1, images[0].rows)));
  } else {
    imwrite(format("%s/mean.png", output_folder.c_str()), norm_0_255(mean.reshape(1, images[0].rows)));
  }

  std::cout<<
  for (int i = 0; i < min(10, W.cols); i++) {
    string msg = format("Eigenvalue #%d = %.5f", i, eigenvalues.at<double>(i));
    std::cout << msg << std::endl;
    Mat ev = W.col(i).clone();
    Mat grayscale = norm_0_255(ev.reshape(1, height));
    Mat cgrayscale;
    applyColorMap(grayscale, cgrayscale, COLORMAP_JET);
    if(argc == 2) {
        imshow(format("eigenface_%d", i), cgrayscale);
    } else {
        imwrite(format("%s/eigenface_%d.png", output_folder.c_str(), i), norm_0_255(cgrayscale));
    }
  }
  for(int num_components = min(W.cols, 10); num_components < min(W.cols, 300); num_components+=15) {
    Mat evs = Mat(W, Range::all(), Range(0, num_components));
    Mat projection = subspaceProject(evs, mean, images[0].reshape(1,1));
    Mat reconstruction = subspaceReconstruct(evs, mean, projection);
    reconstruction = norm_0_255(reconstruction.reshape(1, images[0].rows));
    if(argc == 2) {
        imshow(format("eigenface_reconstruction_%d", num_components), reconstruction);
    } else {
        imwrite(format("%s/eigenface_reconstruction_%d.png", output_folder.c_str(), num_components), reconstruction);
    }
  }
  if(argc == 2) {
    waitKey(0);
  }
  return EXIT_SUCCESS;
}
