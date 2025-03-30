#ifndef RGB_HPP
#define RGB_HPP

class RGB
{
private:
public:
    int red;
    int green;
    int blue;
    RGB();
    RGB(int r, int g, int b);
    RGB(const RGB& other);
    RGB& operator=(const RGB& other);
    ~RGB();
};

#endif