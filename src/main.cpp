#include <chrono>
#include "Input.hpp"

using namespace std;

int main()
{
    int originalFileSize = inputImage();
    inputErrorMethod();
    inputTreshold();
    inputMinBlockSize();
    
    QuadTree qt;
    cout << "Compresing..." << endl << endl;

    // Divide and conquer algorithm
    auto start = chrono::high_resolution_clock::now();
    qt.checkDivideBlock();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    blockToImage();

    cout << "Compression complete!" << endl << endl;
    int compressedFileSize = exportImage();
    cout << endl;
    cout << "Compression statistics:" << endl;
    cout << "Elapsed time: " << elapsed.count() << " seconds" << endl;
    cout << "Original image size: " << (double) originalFileSize / 1000 << " kilobytes" << endl;
    cout << "Compressed image size: " << (double) compressedFileSize / 1000 << " kilobytes" << endl;
    cout << "Compression percentage: " << (double) (originalFileSize - compressedFileSize) / originalFileSize * 100 << "%" << endl;
    cout << "Depth of quadtree: " << qt.getDepth() << endl;
    cout << "Number of nodes: " << QuadTree::numNodes << endl;
    return 0;
}