#include "QuadTree.hpp"
#include <algorithm>

int QuadTree::varianceChoice = 1;
int QuadTree::minimumBlockSize = 10;
int QuadTree::height = 0;
int QuadTree::width = 0;
int QuadTree::numNodes = 1;
double QuadTree::threshold = 100;
RGB* QuadTree::block = nullptr;


QuadTree::QuadTree(int currentH, int currentW, int startH, int startW) : currentHeight(currentH), currentWidth(currentW), startHeight(startH), startWidth(startW)
{
    isSmallest = true;
    numNodes++;
    topLeftChild = nullptr;
    topRightChild = nullptr;
    bottomLeftChild = nullptr;
    bottomRightChild = nullptr;
}

QuadTree::QuadTree()
{
    isSmallest = true;
    startHeight = 0;
    startWidth = 0;
    currentHeight = height;
    currentWidth = width;
    topLeftChild = nullptr;
    topRightChild = nullptr;
    bottomLeftChild = nullptr;
    bottomRightChild = nullptr;
}

QuadTree::~QuadTree(){}

void QuadTree::setValue(int h, int w, RGB value)
{
    block[h * width + w].red = value.red;
    block[h * width + w].green = value.green;
    block[h * width + w].blue = value.blue;
    block[h * width + w].alpha = value.alpha;
}

RGB QuadTree::getValue(int h, int w)
{
    return block[h * width + w];
}

//ambil value mean dari red, green, blue.
RGB QuadTree::getMean()
{
    RGB mean;
    for (int i = startHeight; i < startHeight + currentHeight; i++)
    {
        for (int j = startWidth; j < startWidth + currentWidth; j++)
        {
            mean += getValue(i, j);
        }
    }
    int total_pixel = currentHeight * currentWidth;
    mean /= total_pixel;
    return mean;
}

//ambil value red min, green min, blue min.
RGB QuadTree::getMin()
{
    RGB mini(255, 255, 255, 255);
    for (int i = startHeight; i < startHeight + currentHeight; i++)
    {
        for (int j = startWidth; j < startWidth + currentWidth; j++)
        {
            mini = min(mini, getValue(i, j));
        }
    }
    return mini;
}

//ambil value red max, green max, blue max.
RGB QuadTree::getMax()
{
    RGB maks;
    for (int i = startHeight; i < startHeight + currentHeight; i++)
    {
        for (int j = startWidth; j < startWidth + currentWidth; j++)
        {
            maks = max(maks, getValue(i, j));
        }
    }
    return maks;
}

//masuk rumus 1
double QuadTree::variance()
{
    double variance = 0;
    RGB mean = getMean();
    for (int i = startHeight; i < startHeight + currentHeight; i++)
    {
        for (int j = startWidth; j < startWidth + currentWidth; j++)
        {
            variance += pow(getValue(i, j).red - mean.red, 2);
            variance += pow(getValue(i, j).green - mean.green, 2);
            variance += pow(getValue(i, j).blue - mean.blue, 2);
        }
    }
    int total_pixel = currentHeight * currentWidth;
    variance /= total_pixel;
    variance /= 3;
    // variance = sqrt(variance);
    return variance;
}

//masuk rumus 2
double QuadTree::meanAbsoluteDeviation()
{
    double mean_absolute_deviation = 0;
    RGB mean = getMean();
    for (int i = startHeight; i < startHeight + currentHeight; i++)
    {
        for (int j = startWidth; j < startWidth + currentWidth; j++)
        {
            mean_absolute_deviation += abs(getValue(i, j).red - mean.red);
            mean_absolute_deviation += abs(getValue(i, j).green - mean.green);
            mean_absolute_deviation += abs(getValue(i, j).blue - mean.blue);
        }
    }
    int total_pixel = currentHeight * currentWidth;
    mean_absolute_deviation /= total_pixel;
    mean_absolute_deviation /= 3;
    return mean_absolute_deviation;
}

//masuk rumus 3
double QuadTree::maxPixelDifference()
{
    double max_pixel_difference = 0;
    RGB min = getMin();
    RGB max = getMax();
    max_pixel_difference += abs(max.red - min.red); //abs disini ga wajib si
    max_pixel_difference += abs(max.green - min.green);
    max_pixel_difference += abs(max.blue - min.blue);
    max_pixel_difference /= 3;
    return max_pixel_difference;
}

//masuk rumus 4
double QuadTree::entropy()
{
    double entropy = 0;
    for (int i = startHeight; i < startHeight + currentHeight; i++)
    {
        for (int j = startWidth; j < startWidth + currentWidth; j++)
        {
            entropy -= getValue(i, j).red * log2(getValue(i, j).red);
            entropy -= getValue(i, j).green * log2(getValue(i, j).green);
            entropy -= getValue(i, j).blue * log2(getValue(i, j).blue);
        }
    }
    entropy /= 3;
    return entropy;
}


//algoritma divide&conquer disini
void QuadTree::checkDivideBlock()
{
    if(currentHeight/2 >= minimumBlockSize && currentWidth/2 >= minimumBlockSize)
    {
        double Variance;
        if(varianceChoice == 1) Variance = variance();
        else if(varianceChoice == 2) Variance = meanAbsoluteDeviation();
        else if(varianceChoice == 3) Variance = maxPixelDifference();
        else if(varianceChoice == 4) Variance = entropy();
        // =========================BONUS==========================
        // else if(varianceChoice == 5) Variance = structuralSimilarityIndex();
        // ========================================================
        // Error handling kalo user memasukkan input tidak valid
        else Variance = 0;
        
        if(Variance > threshold)
        {
            isSmallest = false;
            //bagi block jadi 4
            int halfHeight = currentHeight / 2;
            int halfWidth = currentWidth / 2;
            int remainingHeight = currentHeight - halfHeight;
            int remainingWidth = currentWidth - halfWidth;

            topLeftChild = new QuadTree(halfHeight, halfWidth, startHeight, startWidth);
            topRightChild = new QuadTree(halfHeight, remainingWidth, startHeight, startWidth + halfWidth);
            bottomLeftChild = new QuadTree(remainingHeight, halfWidth, startHeight + halfHeight, startWidth);
            bottomRightChild = new QuadTree(remainingHeight, remainingWidth, startHeight + halfHeight, startWidth + halfWidth);

            topLeftChild->checkDivideBlock();
            topRightChild->checkDivideBlock();
            bottomLeftChild->checkDivideBlock();
            bottomRightChild->checkDivideBlock();
        }
        else colorNormalization();
        
    }
    else colorNormalization();
    
}


// Fungsi ini tidak ada panduan, jadi diasumsikan sementara kyk di bawah ini. dia menormalisasikan warna di satu blok
void QuadTree::colorNormalization()
{
    RGB normalized = getMean();
    for (int i = startHeight; i < startHeight + currentHeight; i++)
    {
        for (int j = startWidth; j < startWidth + currentWidth; j++)
        {
            setValue(i, j, normalized);
        }
    }
}

//max depth dfs
int QuadTree::getDepth()
{
    if(isSmallest) return 0;
    else
    {
        int depth1 = topLeftChild->getDepth();
        int depth2 = topRightChild->getDepth();
        int depth3 = bottomLeftChild->getDepth();
        int depth4 = bottomRightChild->getDepth();
        int max_depth = depth1;
        if(depth2 > max_depth) max_depth = depth2;
        if(depth3 > max_depth) max_depth = depth3;
        if(depth4 > max_depth) max_depth = depth4;
        return max_depth + 1;
    }
}
