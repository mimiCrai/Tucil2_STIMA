
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "Input.hpp"
#include "include/stb_image.h"
#include "include/stb_image_write.h"

using namespace std;

unsigned char *imageData = nullptr;
int imageChannels;
string fileType;

int inputImage(){
    string inputPath;
    cout << "Masukkan alamat gambar yang akan dikompresi: ";
    getline(cin, inputPath);
    fileType = getFileType(inputPath);
    if (fileType == "PNG"){
        imageData = stbi_load(inputPath.c_str(), &QuadTree::width, &QuadTree::height, &imageChannels, 4);
    }
    else if (fileType == "JPG"){
        imageData = stbi_load(inputPath.c_str(), &QuadTree::width, &QuadTree::height, &imageChannels, 3);
    }
    else if (fileType == "JPEG"){
        imageData = stbi_load(inputPath.c_str(), &QuadTree::width, &QuadTree::height, &imageChannels, 3);
    }
    if (imageData == nullptr){
        cout << "Gambar tidak ditemukan!" << endl;
        exit(0);
    }
    else if (fileType == "UNKNOWN"){
        cout << "Format gambar tidak didukung!" << endl;
        exit(0);
    }

    cout << "✅ Gambar berhasil dimuat!" << endl << endl;
    imageToBlock();

    return getFileSize(inputPath);
}

int exportImage(string outputPath){
    blockToImage();
    if (fileType == "PNG"){
        stbi_write_png(outputPath.c_str(), QuadTree::width, QuadTree::height, 4, imageData, QuadTree::width * 4);
    }
    else if (fileType == "JPG"){
        stbi_write_jpg(outputPath.c_str(), QuadTree::width, QuadTree::height, 3, imageData, 50);
    }
    else if (fileType == "JPEG"){
        stbi_write_jpg(outputPath.c_str(), QuadTree::width, QuadTree::height, 3, imageData, 50);
    }
    cout << "✅ Gambar berhasil diekspor ke: "<< outputPath << endl;
    return getFileSize(outputPath);
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
    cout << "  1. Variance" << endl;
    cout << "  2. Mean Absolute Deviation (MAD)" << endl;
    cout << "  3. Max Pixel Difference" << endl;
    cout << "  4. Entropy" << endl;
    // cout << "  5. Structural Similarity Index (SSIM)" << endl;
    cout << "Masukkan pilihan (1-4): ";
    string input;
    getline(cin, input);
    stringstream ss(input);
    int choice;
    ss >> choice;
    if (ss.fail() || !ss.eof()){
        cout << "Pilihan tidak valid!" << endl;
        exit(0);
    }
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
    cout << endl;
}


void inputTreshold(){
    cout << "Masukkan ambang batas error: ";
    string input;
    getline(cin, input);
    stringstream ss(input);
    ss >> QuadTree::threshold;
    if (ss.fail() || !ss.eof()){
        cout << "Ambang batas tidak valid!" << endl;
        exit(0);
    }
    if (QuadTree::threshold < 0){
        cout << "Ambang batas tidak valid!" << endl;
        exit(0);
    }
}

void inputMinBlockSize(){
    cout << "Masukkan ukuran blok minimum: ";
    string input;
    getline(cin, input);
    stringstream ss(input);
    ss >> QuadTree::minimumBlockSize;
    if (ss.fail() || !ss.eof()){
        cout << "Ukuran blok minimum tidak valid!" << endl;
        exit(0);
    }
    if (QuadTree::minimumBlockSize < 1){
        cout << "Ukuran blok minimum tidak valid!" << endl;
        exit(0);
    }
    cout << endl;
}

string inputExportPath(){
    string outputPath;
    cout << "Masukkan alamat gambar yang akan diekspor: ";
    getline(cin, outputPath);
    if (getFileType(outputPath) != fileType){
        cout << "Format gambar tidak sesuai dengan input!" << endl;
        exit(0);
    }
    return outputPath;
}

string inputGifPath(){
    string gifPath;
    cout << "Masukkan alamat GIF yang akan diekspor: ";
    getline(cin, gifPath);
    if (getFileType(gifPath) != "GIF"){
        cout << "Format tidak sesuai!" << endl;
        exit(0);
    }
    return gifPath;
}

int getFileSize(string filename){
    FILE *file = fopen(filename.c_str(), "rb");
    if (file == nullptr){
        cout << "File tidak ditemukan!" << endl;
        return -1;
    }
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fclose(file);
    return size;
}

string getFileType(string filename){
    string type;
    if (filename.size() >= 4 && filename.substr(filename.size() - 4) == ".png"){
        type = "PNG";
    }
    else if (filename.size() >= 4 && filename.substr(filename.size() - 4) == ".jpg"){
        type = "JPG";
    }
    else if (filename.size() >= 5 && filename.substr(filename.size() - 5) == ".jpeg"){
        type = "JPEG";
    }
    else if (filename.size() >= 4 && filename.substr(filename.size() - 4) == ".gif"){
        type = "GIF";
    }
    else{
        type = "UNKNOWN";
    }
    return type;
}

RGB* copyQtBlock(){
    RGB* output = new RGB[QuadTree::width * QuadTree::height];
    for (int i = 0; i < QuadTree::width * QuadTree::height; i++)
        output[i] = QuadTree::block[i];
    return output;
}


#endif
#endif