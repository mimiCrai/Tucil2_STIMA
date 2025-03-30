#include "QuadTree.hpp"

QuadTree::QuadTree(int h, int w, int var, int minBlockHeightSize, int minBlockWidthSize, double threshold, int startH, int startW, QuadTree* Par) : height(h), width(w), varianceChoice(var), minimumBlockHeightSize(minBlockHeightSize), minimumBlockWidthSize(minBlockWidthSize), threshold(threshold), startHeight(startH), startWidth(startW), parent(Par)
{
    block = new RGB[height * width];
    isSmallest = true;
    topLeftChild = NULL;
    topRightChild = NULL;
    bottomLeftChild = NULL;
    bottomRightChild = NULL;
}


QuadTree::~QuadTree()
{
    delete[] block;
}

void QuadTree::setValue(int h, int w, RGB value)
{
    block[h * width + w].red = value.red;
    block[h * width + w].green = value.green;
    block[h * width + w].blue = value.blue;
}

RGB QuadTree::getValue(int h, int w)
{
    return block[h * width + w];
}

//ambil value mean dari red, green, blue.
RGB QuadTree::getMean()
{
    RGB mean;
    mean.red = 0;
    mean.green = 0;
    mean.blue = 0;
    int total_pixel = height * width;
    for (int i = 0; i < total_pixel; i++)
    {
        mean.red += block[i].red;
        mean.green += block[i].green;
        mean.blue += block[i].blue;
    }
    mean.red /= total_pixel;
    mean.green /= total_pixel;
    mean.blue /= total_pixel;
    return mean;
}

//ambil value red min, green min, blue min.
RGB QuadTree::getMin()
{
    RGB min;
    min.red = 255;
    min.green = 255;
    min.blue = 255;
    int total_pixel = height * width;
    for (int i = 0; i < total_pixel; i++)
    {
        if (block[i].red < min.red)
            min.red = block[i].red;
        if (block[i].green < min.green)
            min.green = block[i].green;
        if (block[i].blue < min.blue)
            min.blue = block[i].blue;
    }
    return min;
}

//ambil value red max, green max, blue max.
RGB QuadTree::getMax()
{
    RGB max;
    max.red = 0;
    max.green = 0;
    max.blue = 0;
    int total_pixel = height * width;
    for (int i = 0; i < total_pixel; i++)
    {
        if (block[i].red > max.red)
            max.red = block[i].red;
        if (block[i].green > max.green)
            max.green = block[i].green;
        if (block[i].blue > max.blue)
            max.blue = block[i].blue;
    }
    return max;
}

//masuk rumus 1
double QuadTree::variance()
{
    double variance = 0;
    RGB mean = getMean();
    int total_pixel = height * width;
    for (int i = 0; i < total_pixel; i++)
    {
        variance += pow(block[i].red - mean.red, 2);
        variance += pow(block[i].green - mean.green, 2);
        variance += pow(block[i].blue - mean.blue, 2);
    }
    variance /= total_pixel;
    variance /= 3;
    variance = sqrt(variance);
    return variance;
}

//masuk rumus 2
double QuadTree::meanAbsoluteDeviation()
{
    double mean_absolute_deviation = 0;
    RGB mean = getMean();
    int total_pixel = height * width;
    for (int i = 0; i < total_pixel; i++)
    {
        mean_absolute_deviation += abs(block[i].red - mean.red);
        mean_absolute_deviation += abs(block[i].green - mean.green);
        mean_absolute_deviation += abs(block[i].blue - mean.blue);
    }
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
    int total_pixel = height * width;
    for(int i = 0; i < total_pixel; i++)
    {
        if(block[i].red > 0)
        {
            entropy -= block[i].red * log2(block[i].red);
        }
        if(block[i].green > 0)
        {   
            entropy -= block[i].green * log2(block[i].green);
        }
        if(block[i].blue > 0)
        {
            entropy -= block[i].blue * log2(block[i].blue);
        }
    }
    entropy /= 3;
}


//algoritma divide&conquer disini
void QuadTree::checkDivideBlock()
{
    if(height/2 >= minimumBlockHeightSize && width/2 >= minimumBlockWidthSize)
    {
        double Variance = 0.0;
        if(varianceChoice == 1)
        {
            Variance = variance();
        }
        else if(varianceChoice == 2)
        {
            Variance = meanAbsoluteDeviation();
        }
        else if(varianceChoice == 3)
        {
            Variance = maxPixelDifference();
        }
        else if(varianceChoice == 4)
        {
            Variance = entropy();
        }
        // =========================BONUS==========================
        // else if(varianceChoice == 5)
        // {
        //     Variance = structuralSimilarityIndex();
        // }
        // ========================================================
        else // Error handling kalo user memasukkan input tidak valid
        {
            Variance = 0;
        }
        if(Variance > threshold)
        {
            isSmallest = false;
            //bagi block jadi 4
            int halfHeight = height / 2;
            int halfWidth = width / 2;
            int remainingHeight = height - halfHeight;
            int remainingWidth = width - halfWidth;

            QuadTree block1(halfHeight, halfWidth, varianceChoice, minimumBlockHeightSize, minimumBlockWidthSize, threshold, startHeight, startWidth, this);
            topLeftChild = &block1;

            QuadTree block2(halfHeight, remainingWidth, varianceChoice, minimumBlockHeightSize, minimumBlockWidthSize, threshold, startHeight, startWidth + halfWidth, this);
            topRightChild = &block2;

            QuadTree block3(remainingHeight, halfWidth, varianceChoice, minimumBlockHeightSize, minimumBlockWidthSize, threshold, startHeight + halfHeight, startWidth, this);
            bottomLeftChild = &block3;

            QuadTree block4(remainingHeight, remainingWidth, varianceChoice, minimumBlockHeightSize, minimumBlockWidthSize, threshold, startHeight + halfHeight, startWidth + halfWidth, this);
            bottomRightChild = &block4;

            for (int i = 0; i < halfHeight; i++)
            {
                for (int j = 0; j < halfWidth; j++)
                {
                    block1.setValue(i, j, getValue(i, j));
                }
                for (int j = halfWidth; j < width; j++)
                {
                    block2.setValue(i, j - halfWidth, getValue(i, j));
                }
            }
            for (int i = halfHeight; i < height; i++)
            {
                for (int j = 0; j < halfWidth; j++)
                {
                    block3.setValue(i - halfHeight, j, getValue(i, j));
                }
                for (int j = halfWidth; j < width; j++)
                {
                    block4.setValue(i - halfHeight, j - halfWidth, getValue(i, j));
                }
            }
            block1.checkDivideBlock();
            block2.checkDivideBlock();
            block3.checkDivideBlock();
            block4.checkDivideBlock();
        }
        else
        {
            colorNormalization();
        }
    }
    else
    {
        colorNormalization();
    }
}


// Fungsi ini tidak ada panduan, jadi diasumsikan sementara kyk di bawah ini. dia menormalisasikan warna di satu blok
void QuadTree::colorNormalization()
{
    int total_pixel = height * width;
    RGB min = getMin();
    RGB max = getMax();
    RGB normalized;
    // ===================Jika dirasa terlalu lama, bisa dihapus=======================
    for(int i = 0; i < total_pixel; i++)
    {
        normalized.red = (block[i].red - min.red) / (max.red - min.red);
        normalized.green = (block[i].green - min.green) / (max.green - min.green);
        normalized.blue = (block[i].blue - min.blue) / (max.blue - min.blue);
        block[i] = normalized;
    }
    // ================================================================================
    normalized = getMean();
    normalizedColor = normalized;
}


RGB QuadTree::returnBlockColor()
{
    return normalizedColor;
}


//gatau kepake ga, tapi basically ngambil warna setelah dicompress, di koordinat <h,w>
RGB QuadTree::getCompressedColor(int h, int w)
{
    if(isSmallest)
    {
       return returnBlockColor(); 
    }
    else
    {
        if(h < height / 2 && w < width / 2)
        {
            return topLeftChild->getCompressedColor(h, w);
        }
        else if(h < height / 2 && w >= width / 2)
        {
            return topRightChild->getCompressedColor(h, w - width / 2);
        }
        else if(h >= height / 2 && w < width / 2)
        {
            return bottomLeftChild->getCompressedColor(h - height / 2, w);
        }
        else
        {
            return bottomRightChild->getCompressedColor(h - height / 2, w - width / 2);
        }
    }
}


//max depth dfs
int QuadTree::getDepth()
{
    if(isSmallest)
    {
        return 0;
    }
    else
    {
        int depth1 = topLeftChild->getDepth();
        int depth2 = topRightChild->getDepth();
        int depth3 = bottomLeftChild->getDepth();
        int depth4 = bottomRightChild->getDepth();
        int max_depth = depth1;
        if(depth2 > max_depth)
        {
            max_depth = depth2;
        }
        if(depth3 > max_depth)
        {
            max_depth = depth3;
        }
        if(depth4 > max_depth)
        {
            max_depth = depth4;
        }
        return max_depth + 1;
    }
}

//total dfs
int QuadTree::getNodeAmount()
{
    if(isSmallest)
    {
        return 1;
    }
    else
    {
        return topLeftChild->getNodeAmount() + topRightChild->getNodeAmount() + bottomLeftChild->getNodeAmount() + bottomRightChild->getNodeAmount() + 1;
    }
}