#include "rgb.cpp"
#include <cmath>

class quadTree
{
private:
    quadTree* parent;
    bool isSmallest;
    quadTree* child1; //top left
    quadTree* child2; //top right
    quadTree* child3; //bottom left
    quadTree* child4; //bottom right
    rgb normalized_color;
    int startHeight, startWidth;
    int height, width;
    rgb* block;
    const int variance_choice, minimum_block__height_size, minimum_block_width_size;
    const double threshold; //double bener ga??
public:

    // HARUS inisialisasi parent dengan NULL saat pemanggilan pertama =====================!!!!!!!!!!!!!!=========================
    quadTree(int h, int w, int var, int minBlockHeightSize, int minBlockWidthSize, double threshold, int startH, int startW, quadTree* parent);
    /* panjang jd kujelasin dikit
    1. h dan w adalah ukuran block saat ini
    2. var adalah pilian variansi (input)
    3. minBlockHeightSize dan minBlockWidthSize adalah pilihan ukuran minimum (input)
    4. threshold adalah batas (input)
    5. startH dan startW adalah koordinat di gambar aslinya, harusnya ini cm dipake untuk memudahkan aj. Kalau diaras ga dibutuhin, hapus aja
    6. parent untuk bentuk tree
    */
    ~quadTree();

    void setValue(int h, int w, rgb value);
    rgb getValue(int h, int w);

    rgb getMean();
    rgb getMin();
    rgb getMax();

    double variance_variance();
    double variance_MeanAbsoluteDeviation();
    double variance_MaxPixelDifference();
    double variance_Entropy();
    double variance_StructuralSimilarityIndex(); // BONUS

    void checkDivideBlock();

    void colorNormalization();

    rgb returnBlockColor();
    rgb getCompressedColor(int h, int w);

    int getDepth();
    int getNodeAmount();

    // Belum bikin function untuk ngeset value array block biar otomatis. Untuk sementara harus manual pake set value.
};



quadTree::quadTree(int h, int w, int var, int minBlockHeightSize, int minBlockWidthSize, double threshold, int startH, int startW, quadTree* Par) : height(h), width(w), variance_choice(var), minimum_block__height_size(minBlockHeightSize), minimum_block_width_size(minBlockWidthSize), threshold(threshold), startHeight(startH), startWidth(startW), parent(Par)
{
    block = new rgb[height * width];
    isSmallest = true;
    child1 = NULL;
    child2 = NULL;
    child3 = NULL;
    child4 = NULL;
}


quadTree::~quadTree()
{
    delete[] block;
}

void quadTree::setValue(int h, int w, rgb value)
{
    block[h * width + w].red = value.red;
    block[h * width + w].green = value.green;
    block[h * width + w].blue = value.blue;
}

rgb quadTree::getValue(int h, int w)
{
    return block[h * width + w];
}

//ambil value mean dari red, green, blue.
rgb quadTree::getMean()
{
    rgb mean;
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
rgb quadTree::getMin()
{
    rgb min;
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
rgb quadTree::getMax()
{
    rgb max;
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
double quadTree::variance_variance()
{
    double variance = 0;
    rgb mean = getMean();
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
double quadTree::variance_MeanAbsoluteDeviation()
{
    double mean_absolute_deviation = 0;
    rgb mean = getMean();
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
double quadTree::variance_MaxPixelDifference()
{
    double max_pixel_difference = 0;
    rgb min = getMin();
    rgb max = getMax();
    max_pixel_difference += abs(max.red - min.red); //abs disini ga wajib si
    max_pixel_difference += abs(max.green - min.green);
    max_pixel_difference += abs(max.blue - min.blue);
    max_pixel_difference /= 3;
    return max_pixel_difference;
}

//masuk rumus 4
double quadTree::variance_Entropy()
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
void quadTree::checkDivideBlock()
{
    if(height/2 >= minimum_block__height_size && width/2 >= minimum_block_width_size)
    {
        double Variance = 0.0;
        if(variance_choice == 1)
        {
            Variance = variance_variance();
        }
        else if(variance_choice == 2)
        {
            Variance = variance_MeanAbsoluteDeviation();
        }
        else if(variance_choice == 3)
        {
            Variance = variance_MaxPixelDifference();
        }
        else if(variance_choice == 4)
        {
            Variance = variance_Entropy();
        }
        // =========================BONUS==========================
        // else if(variance_choice == 5)
        // {
        //     Variance = variance_StructuralSimilarityIndex();
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

            quadTree block1(halfHeight, halfWidth, variance_choice, minimum_block__height_size, minimum_block_width_size, threshold, startHeight, startWidth, this);
            child1 = &block1;

            quadTree block2(halfHeight, remainingWidth, variance_choice, minimum_block__height_size, minimum_block_width_size, threshold, startHeight, startWidth + halfWidth, this);
            child2 = &block2;

            quadTree block3(remainingHeight, halfWidth, variance_choice, minimum_block__height_size, minimum_block_width_size, threshold, startHeight + halfHeight, startWidth, this);
            child3 = &block3;

            quadTree block4(remainingHeight, remainingWidth, variance_choice, minimum_block__height_size, minimum_block_width_size, threshold, startHeight + halfHeight, startWidth + halfWidth, this);
            child4 = &block4;

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
void quadTree::colorNormalization()
{
    int total_pixel = height * width;
    rgb min = getMin();
    rgb max = getMax();
    rgb normalized;
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
    normalized_color = normalized;
}


rgb quadTree::returnBlockColor()
{
    return normalized_color;
}


//gatau kepake ga, tapi basically ngambil warna setelah dicompress, di koordinat <h,w>
rgb quadTree::getCompressedColor(int h, int w)
{
    if(isSmallest)
    {
       return returnBlockColor(); 
    }
    else
    {
        if(h < height / 2 && w < width / 2)
        {
            return child1->getCompressedColor(h, w);
        }
        else if(h < height / 2 && w >= width / 2)
        {
            return child2->getCompressedColor(h, w - width / 2);
        }
        else if(h >= height / 2 && w < width / 2)
        {
            return child3->getCompressedColor(h - height / 2, w);
        }
        else
        {
            return child4->getCompressedColor(h - height / 2, w - width / 2);
        }
    }
}


//max depth dfs
int quadTree::getDepth()
{
    if(isSmallest)
    {
        return 0;
    }
    else
    {
        int depth1 = child1->getDepth();
        int depth2 = child2->getDepth();
        int depth3 = child3->getDepth();
        int depth4 = child4->getDepth();
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
int quadTree::getNodeAmount()
{
    if(isSmallest)
    {
        return 1;
    }
    else
    {
        return child1->getNodeAmount() + child2->getNodeAmount() + child3->getNodeAmount() + child4->getNodeAmount() + 1;
    }
}