#include "RGB.hpp"

RGB::RGB(): red(0), green(0), blue(0) {}

RGB::RGB(int r, int g, int b): red(r), green(g), blue(b) {}

RGB::RGB(const RGB& other): red(other.red), green(other.green), blue(other.blue) {}

RGB& RGB::operator=(const RGB& other) {
    if (this != &other) {
        red = other.red;
        green = other.green;
        blue = other.blue;
    }
    return *this;
}

RGB::~RGB() {}