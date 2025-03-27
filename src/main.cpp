#include <iostream>
#include <opencv2/opencv.hpp>
#include "quadTree.hpp"
#include <vector>
#include <string>

using namespace cv;
using namespace std;

int main()
{
    string inputPath, outputPath, gifPath;
    int errorMethod, threshold, blockSize;
    double compressionTarget;

    // 1. Input path gambar
    cout << "Masukkan alamat gambar yang akan dikompresi: ";
    cin >> inputPath;

    // Baca gambar
    Mat image = imread(inputPath, IMREAD_GRAYSCALE);
    if (image.empty()) {
        cerr << "Gagal membaca gambar!" << endl;
        return -1;
    }

    // 2. Pilih metode perhitungan error
    cout << "Pilih metode error (1: MAE, 2: RMSE): ";
    cin >> errorMethod;

    // 3. Input ambang batas
    cout << "Masukkan ambang batas error: ";
    cin >> threshold;

    // 4. Ukuran blok minimum
    cout << "Masukkan ukuran blok minimum: ";
    cin >> blockSize;

    // 5. Target persentase kompresi
    cout << "Masukkan target persentase kompresi (1.0 = 100%, 0 untuk nonaktif): ";
    cin >> compressionTarget;

    // 6. Output path gambar terkompresi
    cout << "Masukkan alamat gambar hasil kompresi: ";
    cin >> outputPath;

    // 7. Output path GIF (bonus)
    cout << "Masukkan alamat gif (opsional, kosongkan jika tidak ingin menyimpan): ";
    cin.ignore();
    getline(cin, gifPath);

    
}