#pragma once
#include "color3.hh"
#include "../libz/stb_image_write.h"

#include <functional>
#include <memory>

class Image {
public:
    int width, height;
    std::shared_ptr<unsigned char[]> image;

    Image(const int width, const int height) : width(width), height(height), image(nullptr) {
    }


    virtual ~Image() = default;

    unsigned char& operator[](const int index) const {
        return image.get()[index];
    }

    virtual void save(const char *filename) = 0;
};

class BW_Image final : public Image {
public:
    BW_Image(const int width, const int height) : Image(width, height) {
        image = std::make_shared<unsigned char[]>(width * height);
    }

    void save(const char *filename) override {
        stbi_write_png(filename, width, height, 1, image.get(), width);
    }

    BW_Image negative();
    BW_Image max(const BW_Image &other);
    BW_Image min(const BW_Image &other);
    BW_Image add(const BW_Image &other);
    BW_Image sub(const BW_Image &other);
    BW_Image mean(const BW_Image &other);
    BW_Image clamp(unsigned char min, unsigned char max);
    BW_Image operator*(float f);
    BW_Image operator<(float f);
    BW_Image operator>(float f);

    /** normalizes everything in [0, 1]
     * adds offset so that some of them are > 1
     * put it to pow so that values > 1 get bigger and < 1 get smaller
     * put it back into [0, 255] **/
    BW_Image increase_contrast(double offset, double pow);

    /** Should probably not be used
    * let's talk if it feels like it's needed **/
    BW_Image apply_fun(std::function<unsigned char(unsigned char)> func);

};

class Color_Image final : public Image {
public:
    Color_Image(const int width, const int height) : Image(width, height) {
        image = std::make_shared<unsigned char[]>(width * height * 3);
    }

    void save(const char *filename) override {
        stbi_write_png(filename, width, height, 3, image.get(), width * 3);
    }

    void set_color(const int x, const int y, const Color3 &color) {
        int pos = y * width + x;
        image.get()[3 * pos + 0] = color.r;
        image.get()[3 * pos + 1] = color.g;
        image.get()[3 * pos + 2] = color.b;
        return;
    }

    Color3 get_color(const int x, const int y) {
        int pos = y * width + x;
        return Color3(image.get()[3 * pos], image.get()[3 * pos + 1], image.get()[3 * pos + 2]);
    }
};

BW_Image gradient_tester();

