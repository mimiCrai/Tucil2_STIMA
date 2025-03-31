
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <iostream>
#include "Input.hpp"
#include "include/stb_image.h"
#include "include/stb_image_write.h"

using namespace std;

unsigned char *imageData;
int imageChannels;
string fileType;

void inputImage(){
    string inputPath;
    cout << "Masukkan alamat gambar yang akan dikompresi: ";
    cin >> inputPath;
    if (inputPath.size() >= 4 && inputPath.substr(inputPath.size() - 4) == ".png"){
        imageData = stbi_load(inputPath.c_str(), &QuadTree::width, &QuadTree::height, &imageChannels, 4);
        fileType = "PNG";
    }
    else if (inputPath.size() >= 4 && inputPath.substr(inputPath.size() - 4) == ".jpg"){
        imageData = stbi_load(inputPath.c_str(), &QuadTree::width, &QuadTree::height, &imageChannels, 3);
        fileType = "JPG";
    }
    else if (inputPath.size() >= 5 && inputPath.substr(inputPath.size() - 5) == ".jpeg"){
        imageData = stbi_load(inputPath.c_str(), &QuadTree::width, &QuadTree::height, &imageChannels, 3);
        fileType = "JPEG";
    }
    else{
        fileType = "UNKNOWN";
    }
    if (imageData == nullptr){
        cout << "Gambar tidak ditemukan!" << endl;
        exit(0);
    }
    else if (fileType == "UNKNOWN"){
        cout << "Format gambar tidak didukung!" << endl;
    }
    cout << "Gambar berhasil dimuat!" << endl << endl;
    imageToBlock();
}

void exportImage(){
    if (fileType == "PNG"){
        stbi_write_png("test/output.png", QuadTree::width, QuadTree::height, 4, imageData, QuadTree::width * 4);
        cout << "Gambar berhasil diekspor ke test/output.png" << endl;
    }
    else if (fileType == "JPG"){
        stbi_write_jpg("test/output.jpg", QuadTree::width, QuadTree::height, 3, imageData, 0);
        cout << "Gambar berhasil diekspor ke test/output.jpg" << endl;
    }
    else if (fileType == "JPEG"){
        stbi_write_jpg("test/output.jpeg", QuadTree::width, QuadTree::height, 3, imageData, 0);
        cout << "Gambar berhasil diekspor ke test/output.jpeg" << endl;
    }
}

void imageToBlock(){
    QuadTree::block = new RGB[QuadTree::width * QuadTree::height];
    for (int i = 0; i < QuadTree::height; i++){
        for (int j = 0; j < QuadTree::width; j++){
            if (fileType == "PNG"){
                int index = (i * QuadTree::width + j) * 4;
                unsigned char r = imageData[index + 0]; 
                unsigned char g = imageData[index + 1];  
                unsigned char b = imageData[index + 2];  
                unsigned char a = imageData[index + 3]; 
                QuadTree::block[i * QuadTree::width + j] = RGB(r, g, b, a);
            }
            else{
                int index = (i * QuadTree::width + j) * 3;
                unsigned char r = imageData[index + 0];
                unsigned char g = imageData[index + 1];
                unsigned char b = imageData[index + 2];
                QuadTree::block[i * QuadTree::width + j] = RGB(r, g, b);
            }
        }
    }
}


void blockToImage(){
    for (int i = 0; i < QuadTree::height; i++){
        for (int j = 0; j < QuadTree::width; j++){
            RGB color = QuadTree::block[i * QuadTree::width + j];
            if (fileType == "PNG"){
                int index = (i * QuadTree::width + j) * 4;
                imageData[index + 0] = color.red;
                imageData[index + 1] = color.green;
                imageData[index + 2] = color.blue;
                imageData[index + 3] = color.alpha;
            }
            else{
                int index = (i * QuadTree::width + j) * 3;
                imageData[index + 0] = color.red;
                imageData[index + 1] = color.green;
                imageData[index + 2] = color.blue;
            }
        }
    }
}


void inputErrorMethod(){
    cout << "Pilih metode error: " << endl;
    cout << "1. Variance" << endl;
    cout << "2. Mean Absolute Deviation (MAD)" << endl;
    cout << "3. Max Pixel Difference" << endl;
    cout << "4. Entropy" << endl;
    // cout << "5. Structural Similarity Index (SSIM)" << endl;
    cout << "Masukkan pilihan (1-4): ";
    int choice;
    cin >> choice;
    cout << endl;
    if (choice == 1){
        QuadTree::varianceChoice = 1;
    }
    else if (choice == 2){
        QuadTree::varianceChoice = 2;
    }
    else if (choice == 3){
        QuadTree::varianceChoice = 3;
    }
    else if (choice == 4){
        QuadTree::varianceChoice = 4;
    }
    // else if (choice == 5){
    //     QuadTree::varianceChoice = 5;
    // }
    else{
        cout << "Pilihan tidak valid!" << endl ;
        exit(0);
    }
}


void inputTreshold(){
    cout << "Masukkan ambang batas error: ";
    cin >> QuadTree::threshold;
    if (QuadTree::threshold < 0){
        cout << "Ambang batas tidak valid!" << endl;
        exit(0);
    }
    cout << endl;
}

void inputMinBlockSize(){
    cout << "Masukkan ukuran blok minimum: ";
    cin >> QuadTree::minimumBlockSize;
    if (QuadTree::minimumBlockSize < 1){
        cout << "Ukuran blok minimum tidak valid!" << endl;
        exit(0);
    }
    cout << endl;
}

#endif
#endif