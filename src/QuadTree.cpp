#ifndef GIF_H_IMPLEMENTATION
#define GIF_H_IMPLEMENTATION

#include "include/gif.h"
#include "QuadTree.hpp"

int QuadTree::varianceChoice = 1;
int QuadTree::minimumBlockSize = 10;
int QuadTree::height = 0;
int QuadTree::width = 0;
int QuadTree::numNodes = 1;
double QuadTree::threshold = 100;
RGB* QuadTree::block = nullptr;
std::vector<std::vector<QuadTree*>> QuadTree::nodesAtDepth;


QuadTree::QuadTree(int currentH, int currentW, int startH, int startW) : currentHeight(currentH), currentWidth(currentW), startHeight(startH), startWidth(startW)
{
    isSmallest = true;
    numNodes++;
    topLeftChild = topRightChild = bottomLeftChild = bottomRightChild = nullptr;
}

QuadTree::QuadTree()
{
    isSmallest = true;
    startHeight = 0, startWidth = 0;
    currentHeight = height, currentWidth = width;
    topLeftChild = topRightChild = bottomLeftChild = bottomRightChild = nullptr;
}

QuadTree::~QuadTree(){}

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

void QuadTree::setValue(int h, int w, RGB value, RGB* Block)
{
    Block[h * width + w].red = value.red;
    Block[h * width + w].green = value.green;
    Block[h * width + w].blue = value.blue;
}

RGB QuadTree::getValue(int h, int w, RGB* Block)
{
    return Block[h * width + w];
}

RGB QuadTree::getMean()
{
    RGB mean;
    for (int i = startHeight; i < startHeight + currentHeight; i++)
        for (int j = startWidth; j < startWidth + currentWidth; j++)
            mean += getValue(i, j);
    int total_pixel = currentHeight * currentWidth;
    mean /= total_pixel;
    return mean;
}

RGB QuadTree::getMean(RGB* Block){
    RGB mean;
    for (int i = startHeight; i < startHeight + currentHeight; i++)
        for (int j = startWidth; j < startWidth + currentWidth; j++)
            mean += getValue(i, j, Block);
    int total_pixel = currentHeight * currentWidth;
    mean /= total_pixel;
    return mean;
}

RGB QuadTree::getMin()
{
    RGB mini(255, 255, 255, 255);
    for (int i = startHeight; i < startHeight + currentHeight; i++)
        for (int j = startWidth; j < startWidth + currentWidth; j++)
            mini = min(mini, getValue(i, j));
    return mini;
}

RGB QuadTree::getMin(RGB *Block)
{
    RGB mini(255, 255, 255, 255);
    for (int i = startHeight; i < startHeight + currentHeight; i++)
        for (int j = startWidth; j < startWidth + currentWidth; j++)
            mini = min(mini, getValue(i, j, Block));
    return mini;
}

RGB QuadTree::getMax()
{
    RGB maks;
    for (int i = startHeight; i < startHeight + currentHeight; i++)
        for (int j = startWidth; j < startWidth + currentWidth; j++)
            maks = max(maks, getValue(i, j));
    return maks;
}

RGB QuadTree::getMax(RGB* Block)
{
    RGB maks;
    for (int i = startHeight; i < startHeight + currentHeight; i++)
        for (int j = startWidth; j < startWidth + currentWidth; j++)
            maks = max(maks, getValue(i, j, Block));
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
    std::vector<int> redFrequency(256, 0);
    std::vector<int> greenFrequency(256, 0);
    std::vector<int> blueFrequency(256, 0);

    double entropy = 0.0;
    int total_pixel = currentHeight * currentWidth;
    // count frequency
    for (int i = startHeight; i < startHeight + currentHeight; i++)
    {
        for (int j = startWidth; j < startWidth + currentWidth; j++)
        {
            redFrequency[getValue(i, j).red]++;
            greenFrequency[getValue(i, j).green]++;
            blueFrequency[getValue(i, j).blue]++;
        }
    }
    // calc entropy
    for (int i = 0; i < 256; i++)
    {
        if (redFrequency[i] > 0)
        {
            double p = static_cast<double>(redFrequency[i]) / total_pixel;
            entropy -= p * log2(p);
        }
        if (greenFrequency[i] > 0)
        {
            double p = static_cast<double>(greenFrequency[i]) / total_pixel;
            entropy -= p * log2(p);
        }
        if (blueFrequency[i] > 0)
        {
            double p = static_cast<double>(blueFrequency[i]) / total_pixel;
            entropy -= p * log2(p);
        }
    }
    entropy /= 3;
    return entropy;
}

//masuk rumus 5
double QuadTree::structuralSimilarityIndex()
{
    double structural_similarity_index = 0;
    RGB mean = getMean();
    int total_pixel = currentHeight * currentWidth;
    double sum_x_2_red = 0, sum_x_y_red = 0;
    double sum_x_2_green = 0, sum_x_y_green = 0;
    double sum_x_2_blue = 0, sum_x_y_blue = 0;

    for (int i = startHeight; i < startHeight + currentHeight; i++)
    {
        for (int j = startWidth; j < startWidth + currentWidth; j++)
        {
            sum_x_2_red += pow(getValue(i, j).red, 2);
            sum_x_y_red += getValue(i, j).red * mean.red;

            sum_x_2_green += pow(getValue(i, j).green, 2);
            sum_x_y_green += getValue(i, j).green * mean.green;

            sum_x_2_blue += pow(getValue(i, j).blue, 2);
            sum_x_y_blue += getValue(i, j).blue * mean.blue;
        }
    }
    
    double var_x_red = sum_x_2_red / total_pixel - mean.red * mean.red; 
    double var_x_green = sum_x_2_green / total_pixel - mean.green * mean.green;
    double var_x_blue = sum_x_2_blue / total_pixel - mean.blue * mean.blue;

    double var_xy_red = sum_x_y_red / total_pixel - mean.red * mean.red;
    double var_xy_green = sum_x_y_green / total_pixel - mean.green * mean.green;
    double var_xy_blue = sum_x_y_blue / total_pixel - mean.blue * mean.blue;


    /*
    PENJELASAN PERUBAHAN PADA RUMUS:
    1. Pada rumusnya, harusnya denominator kedua yang berbentuk (var_x_color + var_y_color + c2) yang dimana y adalah warna pixel pada hasil 
    prediksi kompresi berubah. Hal ini terjadi karena karena prediksi kompresi menggunakan rata-rata dari warna block, maka variansinya
    block kompresi bernilai 0, sehingga tidak perlu dimasukkan ke rumus.
    2. Karena alasan yang sama, maka numerator pertama yang merupakan (2*mean.color*mean.color + c1) akan selalu bernilai sama dengan
    denominator pertama yang bernilai (mean.color*mean.color + mean.color*mean.color + c1), sehingga kita bisa menghilangkannya.
    */
    double c1 = 6.5025, c2 = 58.5225;

    double n_red = (2 * var_xy_red + c2);
    double d_red = (var_x_red + c2);
    if(d_red == 0)
    {
        structural_similarity_index += 1;
    }
    else
    {
        structural_similarity_index += n_red / d_red;
    }

    double n_green = (2 * var_xy_green + c2);
    double d_green = (var_x_green + c2);
    structural_similarity_index += n_green / d_green;
    if(d_green == 0)
    {
        structural_similarity_index += 1;
    }
    else
    {
        structural_similarity_index += n_green / d_green;
    }

    double n_blue = (2 * var_xy_blue + c2);
    double d_blue = (var_x_blue + c2);
    structural_similarity_index += n_blue / d_blue;
    if(d_blue == 0)
    {
        structural_similarity_index += 1;
    }
    else
    {
        structural_similarity_index += n_blue / d_blue;
    }

    structural_similarity_index /= 3;
    return structural_similarity_index;
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
        else if(varianceChoice == 5) Variance = structuralSimilarityIndex();
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


void QuadTree::colorNormalization()
{
    RGB normalized = getMean();
    for (int i = startHeight; i < startHeight + currentHeight; i++)
        for (int j = startWidth; j < startWidth + currentWidth; j++)
            setValue(i, j, normalized);
}

void QuadTree::colorNormalization(RGB* referenceBlock, RGB* Block)
{
    RGB normalized = getMean(referenceBlock);
    for (int i = startHeight; i < startHeight + currentHeight; i++)
        for (int j = startWidth; j < startWidth + currentWidth; j++)
            setValue(i, j, normalized, Block);
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


void QuadTree::buildNodesAtDepth(int depth){
    nodesAtDepth[depth].push_back(this);
    if (isSmallest) return;
    topLeftChild->buildNodesAtDepth(depth + 1);
    topRightChild->buildNodesAtDepth(depth + 1);
    bottomLeftChild->buildNodesAtDepth(depth + 1);
    bottomRightChild->buildNodesAtDepth(depth + 1);
}


void QuadTree::generateGIF(RGB* image, std::string outputPath){
    RGB* gifImage = new RGB[width * height];
    unsigned char* gifImageData = new unsigned char[width * height * 4];

    GifWriter gif;
    GifBegin(&gif, outputPath.c_str(), width, height, 100);
    
    for (int i = 0; i < nodesAtDepth.size(); i++){
        for (QuadTree* qt: nodesAtDepth[i]){
            qt->colorNormalization(image, gifImage);
        }
        for (int j = 0; j < height; j++){
            for (int k = 0; k < width; k++){
                int index = (j * width + k) * 4;
                gifImageData[index + 0] = getValue(j, k, gifImage).red;
                gifImageData[index + 1] = getValue(j, k, gifImage).green;
                gifImageData[index + 2] = getValue(j, k, gifImage).blue;
                gifImageData[index + 3] = getValue(j, k, image).alpha;
            }
        }
        GifWriteFrame(&gif, gifImageData, width, height, 100);
    }

    GifEnd(&gif);

    std::cout << "✅ GIF berhasil digenerate ke: "<< outputPath << std::endl;
    delete[] gifImage;
    delete[] gifImageData;
}

#endif
