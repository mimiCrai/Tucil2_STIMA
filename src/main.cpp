#include <chrono>
#include "Input.hpp"

using namespace std;

int main()
{
    int originalFileSize = inputImage();
    inputErrorMethod();
    inputTreshold();
    inputMinBlockSize();
    string exportPath = inputExportPath();
    string gifPath = inputGifPath();

    RGB* imageBlock = copyQtBlock();

    // Divide and conquer algorithm
    cout << endl << "Compresing..." << endl << endl;
    auto start = chrono::high_resolution_clock::now();

    QuadTree qt;
    qt.checkDivideBlock();
    
    int compressedFileSize = exportImage(exportPath);
    double originalFileSizeInKB = (double) originalFileSize / 1000;
    double compressedFileSizeInKB = (double) compressedFileSize / 1000;
    double compressionPercentage = (double) (originalFileSize - compressedFileSize) / originalFileSize * 100;
    int depth = qt.getDepth();
    QuadTree::nodesAtDepth = vector<vector<QuadTree*>>(depth + 1);
    qt.buildNodesAtDepth();
    qt.generateGIF(imageBlock, gifPath);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    cout << endl << "Compression statistics:" << endl;

    cout << "Elapsed time: " << elapsed.count() << " seconds" << endl;
    cout << "Original image size: " << originalFileSizeInKB << " kilobytes" << endl;
    cout << "Compressed image size: " << compressedFileSizeInKB << " kilobytes" << endl;
    cout << "Compression percentage: " << compressionPercentage << "%" << endl;
    cout << "Depth of quadtree: " << depth << endl;
    cout << "Number of nodes: " << QuadTree::numNodes << endl;
    return 0;
}