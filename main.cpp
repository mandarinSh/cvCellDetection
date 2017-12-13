#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <string>

void drawImage(std::string  name, cv::Mat  source)
{
  cv::namedWindow(name, cv::WINDOW_KEEPRATIO);
  cv::imshow(name, source);
  cv::waitKey();
}

int main(int argc, char * argv[])
{
  std::string imgPath = argv[1];

  std::cout << "Hello World!" << std::endl;

//  std::string imgPath = "/home/mandarin/Documents/Detection/53_x20_10 1 4 пассаж.jpeg";
//    std::string imgPath = "/home/mandarin/Documents/Detection/1.jpg";
  cv::Mat srcMat = cv::imread(imgPath);

  drawImage("image", srcMat);

  cv::Mat grayMat;
  cv::cvtColor(srcMat, grayMat, CV_BGR2GRAY);
//  cv::imshow("gray", grayMat);


  cv::Mat binMat;
  cv::adaptiveThreshold(grayMat, binMat, 200, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 301, -2);
  drawImage("bin", binMat);


  cv::dilate(binMat, binMat, cv::getStructuringElement(CV_SHAPE_ELLIPSE, cv::Size(15, 15)));
  cv::erode(binMat, binMat, cv::getStructuringElement(CV_SHAPE_ELLIPSE, cv::Size(15, 15)));

  drawImage("bin2", binMat);


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
//  drawImage("gauss1", gaussMat1);

  cv::Mat gaussMat2;
  cv::GaussianBlur(binMat, gaussMat2, cv::Size(99, 99), 0, 0);
//  drawImage("gauss2", gaussMat2);


  cv::Mat gaussBinMat1;
  cv::adaptiveThreshold(gaussMat1, gaussBinMat1, 200, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 15, -4);

  cv::Mat gaussBinMat2;
  cv::adaptiveThreshold(gaussMat2, gaussBinMat2, 200, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 15, -4);

  cv::Mat gaussBinMat = gaussBinMat1-gaussBinMat2;
//  drawImage("gaussBin", gaussBinMat);


  //-------------------------  MedianBlur Filter ----------------------

  cv::Mat medianMat;
  cv::medianBlur(gaussBinMat, medianMat, 9);
//  drawImage("MedianBlur", medianMat);

  //------------------------- EndMedianBlur ---------------------------


  std::vector<std::vector<cv::Point> > contours;
  cv::Mat hierarchy;
  cv::findContours(medianMat, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
  cv::Mat contourImage(medianMat.size(), CV_8UC3, cv::Scalar(0,0,0));
  for (size_t i = 0; i < contours.size(); i++) {
    cv::drawContours(contourImage, contours, i, cv::Scalar(255, 255, 255));
  }

//  drawImage("contours", contourImage);


//--------------------

//  cv::Mat srcMat2(medianMat.size(), CV_8UC3, cv::Scalar(0,0,0));

  for (size_t i = 0; i < contours.size(); i++) {

    cv::drawContours(srcMat, contours, i, cv::Scalar(255, 255, 255), 2, cv::LINE_8, hierarchy, 2);
  }

  drawImage("srcMat2", srcMat);

//-----------------------

//--------convexHull------------------

  std::vector < std::vector < cv::Point > > hull(contours.size());
  for(size_t i = 0; i < contours.size(); ++i){
      cv::convexHull(cv::Mat(contours[i]), hull[i], false);
    }

  cv::Mat hullImage(contourImage.size(), CV_8UC3);
  for (size_t i = 0; i < contours.size(); ++i){
      cv::drawContours(hullImage, hull, i, cv::Scalar(255, 255, 255));
    }
//  drawImage("hullImage", hullImage);

//-------END convexHull----------------

  std::ofstream outFile;
  outFile.open("out.csv");
  outFile << " , marker1(Blue), marker2(Red)\n";
  outFile << "count, \n";
  outFile << "marker_type, centerX, centerY\n";



  return 0;
}

