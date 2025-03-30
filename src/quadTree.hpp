#include "rgb.hpp"
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