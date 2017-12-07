#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <string>

int main(int argc, char * argv[])
{
  std::string imgPath = argv[1];

  std::cout << "Hello World!" << std::endl;

//  std::string imgPath = "/home/mandarin/Documents/Detection/53_x20_10 1 4 пассаж.jpeg";
  cv::Mat srcMat = cv::imread(imgPath);

  cv::imshow("image", srcMat);
  cv::waitKey();

  cv::Mat grayMat;
  cv::cvtColor(srcMat, grayMat, CV_BGR2GRAY);
//  cv::imshow("gray", grayMat);


  cv::Mat binMat;
  cv::adaptiveThreshold(grayMat, binMat, 200, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 301, -2);
//  cv::imshow("bin", binMat);
//  cv::waitKey();

//  cv::Mat gaussMat1;
//  cv::GaussianBlur(binMat, gaussMat1, cv::Size(21, 21), 0, 0);
////  cv::imshow("gauss1", gaussMat1);
////  cv::waitKey();

//  cv::Mat gaussMat2;
//  cv::GaussianBlur(binMat, gaussMat2, cv::Size(35, 35), 0, 0);
////  cv::imshow("gauss2", gaussMat2);
////  cv::waitKey();

  cv::Mat gaussMat1;
  cv::GaussianBlur(binMat, gaussMat1, cv::Size(1, 1), 0, 0);
//  cv::imshow("gauss1", gaussMat1);
//  cv::waitKey();

  cv::Mat gaussMat2;
  cv::GaussianBlur(binMat, gaussMat2, cv::Size(99, 99), 0, 0);
//  cv::imshow("gauss2", gaussMat2);
//  cv::waitKey();


  cv::Mat gaussBinMat1;
  cv::adaptiveThreshold(gaussMat1, gaussBinMat1, 200, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 15, -4);

  cv::Mat gaussBinMat2;
  cv::adaptiveThreshold(gaussMat2, gaussBinMat2, 200, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 15, -4);

  cv::Mat gaussBinMat = gaussBinMat1-gaussBinMat2;
  cv::imshow("gaussBin", gaussBinMat);
  cv::waitKey();


  //-------------------------  MedianBlur Filter ----------------------

  cv::Mat medianMat;
  cv::medianBlur(gaussBinMat, medianMat, 9);
  cv::imshow("MedianBlur", medianMat);
  cv::waitKey();

  //------------------------- EndMedianBlur ---------------------------


  std::vector<std::vector<cv::Point> > contours;
  cv::findContours(medianMat, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE );
  cv::Mat contourImage(medianMat.size(), CV_8UC3, cv::Scalar(0,0,0));
  for (size_t i = 0; i < contours.size(); i++) {
    cv::drawContours(contourImage, contours, i, cv::Scalar(255, 255, 255));
  }

  cv::imshow("contours", contourImage);
  cv::waitKey();

//--------convexHull------------------

  std::vector < std::vector < cv::Point > > hull(contours.size());
  for(size_t i = 0; i < contours.size(); ++i){
      cv::convexHull(cv::Mat(contours[i]), hull[i], false);
    }

  cv::Mat hullImage(contourImage.size(), CV_8UC3);
  for (size_t i = 0; i < contours.size(); ++i){
      cv::drawContours(hullImage, hull, i, cv::Scalar(255, 255, 255));
    }
  cv::imshow("hullImage", hullImage);
  cv::waitKey();

//-------END convexHull----------------

  std::ofstream outFile;
  outFile.open("out.csv");
  outFile << " , marker1(Blue), marker2(Red)\n";
  outFile << "count, \n";
  outFile << "marker_type, centerX, centerY\n";



  return 0;
}

