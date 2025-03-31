#include <iostream>
#include "QuadTree.hpp"
#include <vector>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/stb_image_write.h"

using namespace std;

int main()
{
    int width, height, channels;
    unsigned char* data = stbi_load("test.png", &width, &height, &channels, 3);
    if (data == nullptr) {
        cerr << "Error loading image" << endl;
        return -1;
    }
    RGB* block = new RGB[width * height];
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = (y * width + x) * 3;
            unsigned char r = data[index + 0];
            unsigned char g = data[index + 1];
            unsigned char b = data[index + 2];
    
            block[y * width + x] = RGB(r, g, b);
        }
    }
    QuadTree::block = block;
    QuadTree::height = height;
    QuadTree::width = width;

    QuadTree qt;
    qt.checkDivideBlock();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            RGB color = qt.getValue(y, x);
            int index = (y * width + x) * 3;
            data[index + 0] = color.red;
            data[index + 1] = color.green;
            data[index + 2] = color.blue;
        }
    }
    cout << "Compression complete!" << endl;
    cout << "Number of nodes: " << QuadTree::numNodes << endl;
    cout << "Depth: " << qt.getDepth() << endl;
    stbi_write_jpg("compressed.jpg", width, height, 3, data, 100);
    // stbi_image_free(data);
    // string inputPath, outputPath, gifPath;
    // int errorMethod, threshold, blockSize;
    // double compressionTarget;

    // // 1. Input path gambar
    // cout << "Masukkan alamat gambar yang akan dikompresi: ";
    // cin >> inputPath;


    // Baca gambar
    // Mat image = imread(inputPath, IMREAD_GRAYSCALE);
    // if (image.empty()) {
    //     cerr << "Gagal membaca gambar!" << endl;
    //     return -1;
    // }

    // 2. Pilih metode perhitungan error
    // cout << "Pilih metode error (1: MAE, 2: RMSE): ";
    // cin >> errorMethod;

    // // 3. Input ambang batas
    // cout << "Masukkan ambang batas error: ";
    // cin >> threshold;

    // // 4. Ukuran blok minimum
    // cout << "Masukkan ukuran blok minimum: ";
    // cin >> blockSize;

    // // 5. Target persentase kompresi
    // cout << "Masukkan target persentase kompresi (1.0 = 100%, 0 untuk nonaktif): ";
    // cin >> compressionTarget;

    // // 6. Output path gambar terkompresi
    // cout << "Masukkan alamat gambar hasil kompresi: ";
    // cin >> outputPath;

    // // 7. Output path GIF (bonus)
    // cout << "Masukkan alamat gif (opsional, kosongkan jika tidak ingin menyimpan): ";
    // cin.ignore();
    // getline(cin, gifPath);

    return 0;
}