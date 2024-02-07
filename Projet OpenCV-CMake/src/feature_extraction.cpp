//
// Created by anouk on 24/01/2024.
//

#include "feature_extraction.hpp"
#include <fstream>
#include <iostream>

pair<int,int> extractSize(const string& fileIm){
    Mat im = imread(fileIm);
    threshold(im, im, 200, 255, cv::THRESH_BINARY);
    int premierPixelNoirX = 500;
    int dernierPixelNoirX = 0;
    // parcourt les lignes de l'image
    for (int i = 0; i < im.rows; ++i) {
        // parcourt les colonnes de l'image
        for (int x = 0; x < im.cols; ++x) {
            if (im.at<uchar>(i, x) < 50) {  // pixel noir trouvé
                if (x<premierPixelNoirX) {
                    premierPixelNoirX = x;
                }
                if (x>dernierPixelNoirX) {
                    dernierPixelNoirX = x;
                }
            }
        }
    }
    // Calcul la largeur du motif
    int largeur = dernierPixelNoirX - premierPixelNoirX ;

    int premierPixelNoirY = 500;
    int dernierPixelNoirY = 0;
    // parcourt les colonnes de l'image
    for (int i = 0; i < im.cols; ++i) {
        // parcourt les lignes de l'image
        for (int y = 0; y < im.rows; ++y) {
            if (im.at<uchar>(y, i) < 50) {  // pixel noir trouvé
                if (y<premierPixelNoirY) {
                    premierPixelNoirY = y;
                }
                if (y>dernierPixelNoirY) {
                    dernierPixelNoirY = y;
                }
            }
        }
    }
    // Calcul la hauteur du motif
    int hauteur = dernierPixelNoirY - premierPixelNoirY ;

    return make_pair(largeur, hauteur);
}

vector<int> countPixel( const string& image) {
    //Mat im = imread(".//..//Result/accident_001_01_1_3.jpg");
    vector<int> res;


    Mat im = imread(image);

    if (im.data == nullptr) {
        cerr << "Image not found: " << endl;
        waitKey(0);
        //system("pause");
        exit(EXIT_FAILURE);
    }
    //imshow("image de base", im);

    Mat gray ;
    cvtColor(im, gray, cv::COLOR_BGR2GRAY);
    //imshow("image bw",gray);

    // Transform it to binary and invert it. White on black is needed.
    Mat bw;
    threshold(gray, bw, 40, 255, THRESH_BINARY_INV | THRESH_OTSU);
    //imshow("binary", bw);

    //vector<Point> black_pixels;   // output, locations of non-zero pixels
    int nbBlack = cv::countNonZero(bw);

    int nbPixels = bw.rows * bw.cols;
    //cout << "Nb pixels: " << nbPixels << endl;
    //cout << "White pixels: " << nbPixels - nbBlack << endl;
    //cout << "black pixels: " << nbBlack << endl; // amount of black pixels is returned from the size
    res.push_back(nbPixels); res.push_back(nbBlack); res.push_back(nbPixels - nbBlack);

    return res;
}

double air(const string& image){
    Mat img = imread(image, IMREAD_GRAYSCALE);
    //imshow("image de base",img);

    // Binarise l'image à l'aide de la méthode d'Otsu
    Mat binary;
    cv::threshold(img, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    // Inverse l'image binaire
    cv::bitwise_not(binary, binary);

    //imshow("binary",binary);

    // Trouve les contours dans l'image binarisée
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(binary, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    // Calcule les moments du premier contour trouvé
    Moments M = moments(contours[0]);
    double area = 0;
    // Calculer l'aire du premier contour trouvé
    for(int i = 0; i < contours.size(); i++) {
        area += cv::contourArea(contours[i]);
    }
    /*Mat drawing = Mat::zeros(binary.size(), CV_8UC3);
    for(int i = 0; i < contours.size(); i++) {
        Scalar color_contours = Scalar(0, 255, 0); // green - color for contours
        // draw ith contour
        drawContours(drawing, contours, i, color_contours, 1, 8, vector<Vec4i>(), 0, Point());
        imshow("drawing contour", drawing);

    }*/
    return area;
}
