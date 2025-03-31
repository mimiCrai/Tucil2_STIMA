#include <chrono>
#include "Input.hpp"

using namespace std;

int main()
{
    inputImage();
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
    cout << "Elapsed time: " << elapsed.count() << " seconds" << endl;
    cout << "Number of nodes: " << QuadTree::numNodes << endl;
    cout << "Depth: " << qt.getDepth() << endl;

    exportImage();

    return 0;
}