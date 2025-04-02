#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include "RGB.hpp"
#include <cmath>
#include <vector>
#include <iostream>
#include <algorithm>

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
    static int varianceChoice, minimumBlockSize;
    static double threshold; 
    static std::vector<std::vector<QuadTree*>> nodesAtDepth;

    QuadTree();
    QuadTree(int currentH, int currentW, int startH, int startW);
    ~QuadTree();

    /*
        Setting value of the static variable block at height h, width w
    */
    static void setValue(int h, int w, RGB value);

    /*
        Setting value of Block at height h, width w
    */
    void setValue(int h, int w, RGB value, RGB* Block);

    /*
        Getting value of the static variable block at height h, width w
    */
    static RGB getValue(int h, int w);

    /*
        Getting value of Block at height h, width w
    */
    RGB getValue(int h, int w, RGB* Block);

    RGB getMean();
    RGB getMean(RGB* Block);
    RGB getMin();
    RGB getMin(RGB* Block);
    RGB getMax();
    RGB getMax(RGB* Block);


    double variance();
    double meanAbsoluteDeviation();
    double maxPixelDifference();
    double entropy();
    double structuralSimilarityIndex(); // BONUS

    void checkDivideBlock();

    void colorNormalization();
    void colorNormalization(RGB* referenceBlock, RGB* Block);


    int getDepth();

    void buildNodesAtDepth(int depth = 0);

    void generateGIF(RGB* image, std::string outputPath);

    // Belum bikin function untuk ngeset value array block biar otomatis. Untuk sementara harus manual pake set value.
};

#endif