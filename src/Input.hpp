#ifndef INPUT_HPP
#define INPUT_HPP

#include "QuadTree.hpp"
#include <sstream>
#include <string>
using namespace std;

extern unsigned char *imageData;
extern int imageChannels;

/*
    .png = PNG
    .jpg = JPG
    .jpeg = JPEG
    .gif = GIF
*/
extern string fileType; 

int inputImage();

void imageToBlock();

void blockToImage();

void inputErrorMethod();

void inputTreshold();

void inputMinBlockSize();

string inputExportPath();

string inputGifPath();

int exportImage(string outputPath);

int getFileSize(string filename);

string getFileType(string filename);

RGB* copyQtBlock();

#endif