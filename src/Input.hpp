#ifndef INPUT_HPP
#define INPUT_HPP

#include "QuadTree.hpp"
#include <iostream>


extern unsigned char *imageData;
extern int imageChannels;

/*
    .png = PNG
    .jpg = JPG
    .jpeg = JPEG
*/
extern std::string fileType; 

void inputImage();

void exportImage();

void imageToBlock();

void blockToImage();

void inputErrorMethod();

void inputTreshold();

void inputMinBlockSize();



#endif