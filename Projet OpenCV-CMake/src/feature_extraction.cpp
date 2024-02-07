//
// Created by anouk on 24/01/2024.
//

#include "feature_extraction.hpp"
#include <fstream>
#include <iostream>

pair<int,int> extractSize(const string& fileIm){
    Mat imBase = imread(fileIm);
    Mat im;
    cvtColor(imBase, im, cv::COLOR_BGR2GRAY);
    threshold(im, im, 200, 255, cv::THRESH_BINARY);

    int premierPixelNoirX = im.cols;
    int dernierPixelNoirX = 0;
    int premierPixelNoirY = im.rows;
    int dernierPixelNoirY = 0;
    // parcourt les colonnes de l'image
    for (int x = 0; x < im.cols; ++x) {
        // parcourt les lignes de l'image
        for (int y = 0; y < im.rows; ++y) {
            if (im.at<uchar>(y, x) < 100) {  // pixel noir trouvé
                if (y<premierPixelNoirY) {
                    premierPixelNoirY = y;
                }
                if (y>dernierPixelNoirY) {
                    dernierPixelNoirY = y;
                }
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
    // Calcul la hauteur du motif
    int hauteur = dernierPixelNoirY - premierPixelNoirY ;
    //std::cout<<"x :"<<premierPixelNoirX<<" x der : "<< dernierPixelNoirX<<"   y:"<<premierPixelNoirY<<" y der :"<<dernierPixelNoirY<<endl;

    Rect roiRect(premierPixelNoirX, premierPixelNoirY, largeur, hauteur);  // (x, y, width, height)
    Mat roi = imBase(roiRect).clone();
    int newHauteur;
    int newLargeur;
    if(hauteur>largeur){
        newHauteur=250;
        double ratio=(static_cast<double>(largeur)/hauteur)*250;
        newLargeur = static_cast<int>(ratio);
    } else {
        newLargeur = 250;
        double ratio = (static_cast<double>(hauteur)/largeur)*250;
        newHauteur = static_cast<int>(ratio);
    }
    resize(roi, roi, cv::Size(newLargeur, newHauteur));

    // Image vide blanche pour les marges
    Mat imVide(250, 250, roi.type(), cv::Scalar(255, 255, 255));
    // Position de l'image redimensionnée sur la vide
    int posX = static_cast<int>((250 - newLargeur) / 2);
    int posY = static_cast<int>((250 - newHauteur) / 2);
    Rect rec = Rect(posX, posY, newLargeur, newHauteur);
    // Place image redimensionnée sur le fond blanc
    roi.copyTo(imVide(rec));

    imwrite("../saved_model.png",imVide);

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

Point reco_barycentre(const string imName){

    // Load image
    Mat im = imread(imName);
    if(im.data == nullptr){
        cerr << "Image not found: "<< imName << endl;
        waitKey(0);
        //system("pause");
        exit(EXIT_FAILURE);
    }

    //imshow("exemple_init", im);

    // Convertit l'image en niveaux de gris
    Mat imGray;
    cvtColor(im, imGray, cv::COLOR_BGR2GRAY);

    // Binarise l'image à l'aide de la méthode d'Otsu
    Mat imBinary;
    threshold(imGray, imBinary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    // Inverser l'image binaire
    bitwise_not(imBinary, imBinary);

    // Trouve les contours de l'objet dans l'image binaire
    vector<vector<Point>> contours;
    findContours(imBinary, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

    // Dessine les contours sur l'image
    drawContours(im, contours, -1, cv::Scalar(0, 255, 0), 2);

    Moments moments0 = moments(contours[0]);

    double centerX;
    double centerY;

    // Renvoie la valeur des moments calculés
    //std::cout << "moments0.m10 : "<< to_string(moments0.m10) <<"; moments.m01 : " << to_string(moments0.m01) <<"; moments.m00 : " << to_string(moments0.m00) << std::endl;

    if(moments0.m10!=0 && moments0.m01!=0 && moments0.m00 != 0){
        // Calcule les coordonnées du centre de masse
        centerX = moments0.m10 / moments0.m00;
        centerY = moments0.m01 / moments0.m00;
    }

    else if((cv::moments(contours[1])).m10!=0 && (moments(contours[1])).m01!=0 && (moments(contours[1])).m00 != 0){
        Moments moments1 = moments(contours[1]);
        //Sinon, calcule le barycentre avec le deuxième contour capté
        centerX = moments1.m10 / moments1.m00;
        centerY = moments1.m01 / moments1.m00;
    }
    else if((moments(contours[2])).m10!=0 && (moments(contours[2])).m01!=0 && (moments(contours[2])).m00 != 0){
        Moments moments2 = moments(contours[2]);
        //Sinon, calcule le barycentre avec le troisième contour capté
        centerX = moments2.m10 / moments2.m00;
        centerY = moments2.m01 / moments2.m00;
    }
    else{
        Moments moments3 = moments(contours[3]);

        //Sinon, calcule le barycentre avec le troisième contour capté
        if(!moments3.m00==0){
            centerX = moments3.m10 / moments3.m00;
            centerY = moments3.m01 / moments3.m00;
        }
            //Si le moment est toujours nul, on met le centre de masse au centre
        else{
            centerX = im.cols/2;
            centerY = im.rows/2;
        }
    }

    // Affiche les coordonnées du centre de masse
    //std::cout << "Le centre de masse de "<< imName <<" a pour coordonnees : (" << centerX << ", " << centerY << ")" << std::endl;

    // Enregistre le centre de masse dans res
    Point res = Point (centerX,centerY);

    // Dessine le centre de masse sur l'image
    circle(im, res, 5, cv::Scalar(0, 0, 255), -1);

    // Visualisation de l'image avec le centre de masse
    //imshow("Extraction du centre de masse", im);

    return res;

}
