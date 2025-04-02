#ifndef INPUT_HPP
#define INPUT_HPP

#include "QuadTree.hpp"
#include <iostream>
using namespace std;

extern unsigned char *imageData;
extern int imageChannels;

/*
    .png = PNG
    .jpg = JPG
    .jpeg = JPEG
*/
extern string fileType; 

int inputImage();

int exportImage();

void imageToBlock();

void blockToImage();

void inputErrorMethod();

void inputTreshold();

void inputMinBlockSize();

int getFileSize(string filename);

string getFileType(string filename);

#endif