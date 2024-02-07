//
// Created by anouk on 24/01/2024.
//

#ifndef PROJET_OPENCV_CMAKE_FEATURE_EXTRACTION_HPP
#define PROJET_OPENCV_CMAKE_FEATURE_EXTRACTION_HPP

#include <utility> // pour std::pair
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
using namespace std;
using namespace cv;

pair<int,int> extractSize(const string& fileIm);
vector<int> countPixel( const string& image);
double air(const string& image);




#endif //PROJET_OPENCV_CMAKE_FEATURE_EXTRACTION_HPP
