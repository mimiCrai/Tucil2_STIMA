#include "RGB.hpp"
#include <cmath>

class QuadTree
{
private:
    QuadTree* parent;
    bool isSmallest;
    QuadTree* topLeftChild; //top left
    QuadTree* topRightChild; //top right
    QuadTree* bottomLeftChild; //bottom left
    QuadTree* bottomRightChild; //bottom right
    RGB normalizedColor;
    int startHeight, startWidth;
    int height, width;
    RGB* block;
    const int varianceChoice, minimumBlockHeightSize, minimumBlockWidthSize;
    const double threshold; //double bener ga??
public:

    // HARUS inisialisasi parent dengan NULL saat pemanggilan pertama =====================!!!!!!!!!!!!!!=========================
    QuadTree(int h, int w, int var, int minBlockHeightSize, int minBlockWidthSize, double threshold, int startH, int startW, QuadTree* parent);
    /* panjang jd kujelasin dikit
    1. h dan w adalah ukuran block saat ini
    2. var adalah pilian variansi (input)
    3. minBlockHeightSize dan minBlockWidthSize adalah pilihan ukuran minimum (input)
    4. threshold adalah batas (input)
    5. startH dan startW adalah koordinat di gambar aslinya, harusnya ini cm dipake untuk memudahkan aj. Kalau diaras ga dibutuhin, hapus aja
    6. parent untuk bentuk tree
    */
    ~QuadTree();

    void setValue(int h, int w, RGB value);
    RGB getValue(int h, int w);

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

    RGB returnBlockColor();
    RGB getCompressedColor(int h, int w);

    int getDepth();
    int getNodeAmount();

    // Belum bikin function untuk ngeset value array block biar otomatis. Untuk sementara harus manual pake set value.
};