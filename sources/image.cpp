#include "../includes/image.hh"
#include <cassert>
#include <algorithm>
#include <cmath>

BW_Image BW_Image::negative()
{
    BW_Image result(this->width, this->height);
    for (int i = 0; i < this->width * this->height; i++)
        result.image[i] = 255 - this->image[i];
    return result;
}

BW_Image BW_Image::max(const BW_Image &other)
{
    assert(this->width == other.width);
    assert(this->height == other.height);
    BW_Image result(this->width, this->height);
    for (int i = 0; i < this->width * this->height; i++)
        result.image[i] = std::max(this->image[i], other.image[i]);
    return result;
}

BW_Image BW_Image::min(const BW_Image &other)
{
    assert(this->width == other.width);
    assert(this->height == other.height);
    BW_Image result(this->width, this->height);
    for (int i = 0; i < this->width * this->height; i++)
        result.image[i] = std::min(this->image[i], other.image[i]);
    return result;
}

BW_Image BW_Image::add(const BW_Image &other)
{
    assert(this->width == other.width);
    assert(this->height == other.height);
    BW_Image result(this->width, this->height);
    for (int i = 0; i < this->width * this->height; i++)
        result.image[i] = std::clamp(this->image[i] + other.image[i], 0, 255);
    return result;
}

BW_Image BW_Image::sub(const BW_Image &other)
{
    assert(this->width == other.width);
    assert(this->height == other.height);
    BW_Image result(this->width, this->height);
    for (int i = 0; i < this->width * this->height; i++)
        result.image[i] = std::abs(this->image[i] - other.image[i]);
    return result;
}

BW_Image BW_Image::mean(const BW_Image &other)
{
    assert(this->width == other.width);
    assert(this->height == other.height);
    BW_Image result(this->width, this->height);
    for (int i = 0; i < this->width * this->height; i++)
        result.image[i] = this->image[i] / 2 + other.image[i] / 2;
    return result;
}

BW_Image BW_Image::clamp(unsigned char min, unsigned char max)
{
    BW_Image result(this->width, this->height);
    for (int i = 0; i < this->width * this->height; i++)
        result.image[i] = std::clamp(this->image[i], min, max);
    return result;
}

BW_Image BW_Image::operator*(float f)
{
    BW_Image result(this->width, this->height);
    for (int i = 0; i < this->width * this->height; i++)
        result.image[i] = this->image[i] * f;
    return result;
}

BW_Image BW_Image::operator<(float f)
{
    BW_Image result(this->width, this->height);
    for (int i = 0; i < this->width * this->height; i++)
        result.image[i] = this->image[i] < f ? this->image[i] : 0;
    return result;
}

BW_Image BW_Image::operator>(float f)
{
    BW_Image result(this->width, this->height);
    for (int i = 0; i < this->width * this->height; i++)
        result.image[i] = this->image[i] > f ? this->image[i] : 0;
    return result;
}

BW_Image BW_Image::increase_contrast(double offset, double pow) {
    BW_Image result(this->width, this->height);
    for (int i = 0; i < this->width * this->height; i++) {
        double vf = (double)this->image[i] / 255 + offset;
        result.image[i] = std::clamp((int)(std::pow(vf, pow) * 255), 0, 255);
    }
    return result;
}

BW_Image BW_Image::apply_fun(std::function<unsigned char(unsigned char)> func) {
    BW_Image result(this->width, this->height);
    for (int i = 0; i < this->width * this->height; i++)
        result.image[i] = func(this->image[i]);
    return result;
}

BW_Image gradient_tester()
{
    BW_Image im = BW_Image(256, 128);

    for (int x = 0; x < 128; x++) {
        for (int y = 0; y < 256; y++) {
            im[x * 256 + y] = y;
        }
    }
    return im;
}