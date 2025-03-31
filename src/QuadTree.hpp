#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include "RGB.hpp"
#include <cmath>
#include <vector>

class QuadTree
{
private:
    int startHeight, startWidth, currentHeight, currentWidth;
    bool isSmallest;
    QuadTree* topLeftChild;
    QuadTree* topRightChild;
    QuadTree* bottomLeftChild;
    QuadTree* bottomRightChild;

public:
    static int height, width, numNodes;
    static RGB* block;
    static int varianceChoice, minimumBlockHeightSize, minimumBlockWidthSize;
    static double threshold; 

    QuadTree();
    QuadTree(int currentH, int currentW, int startH, int startW);
    ~QuadTree();

    static void setValue(int h, int w, RGB value);
    static RGB getValue(int h, int w);

    RGB getMean();
    RGB getMin();
    RGB getMax();

    double variance();
    double meanAbsoluteDeviation();
    double maxPixelDifference();
    double entropy();
    double structuralSimilarityIndex(); // BONUS

    void checkDivideBlock();

    void colorNormalization();

    int getDepth();

    // Belum bikin function untuk ngeset value array block biar otomatis. Untuk sementara harus manual pake set value.
};

#endif