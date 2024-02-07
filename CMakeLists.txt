cmake_minimum_required(VERSION 3.26)
project(RIV)

set(CMAKE_CXX_STANDARD 14)

include_directories("Projet OpenCV-CMake/include")

add_executable(RIV
        "Projet OpenCV-CMake/include/feature_extraction.hpp"
        "Projet OpenCV-CMake/include/histogram.hpp"
        "Projet OpenCV-CMake/include/icon_label.hpp"
        "Projet OpenCV-CMake/include/redresser_image.h"
        "Projet OpenCV-CMake/include/text_size.hpp"
        "Projet OpenCV-CMake/src/feature_extraction.cpp"
        "Projet OpenCV-CMake/src/histogram.cpp"
        "Projet OpenCV-CMake/src/icon_label.cpp"
        "Projet OpenCV-CMake/src/main_projet.cpp"
        "Projet OpenCV-CMake/src/redresser_image.cpp"
        "Projet OpenCV-CMake/src/text_size.cpp")
