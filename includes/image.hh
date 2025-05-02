#pragma once
#include "color3.hh"
#include "../libz/stb_image_write.h"

class Image {
public:
    int width, height;
    unsigned char *image;

    Image(const int width, const int height) : width(width), height(height), image(nullptr) {
    }


    virtual ~Image() {
        delete[] image;
    }

    unsigned char& operator[](const int index) const {
        return image[index];
    }

    virtual void save(const char *filename) = 0;
};

class BW_Image final : public Image {
public:
    BW_Image(const int width, const int height) : Image(width, height) {
        image = new unsigned char[width * height];
    }

    void save(const char *filename) override {
        stbi_write_png(filename, width, height, 1, image, width);
    }
};

class Color_Image final : public Image {
public:
    Color_Image(const int width, const int height) : Image(width, height) {
        image = new unsigned char[width * height * 3];
    }

    void save(const char *filename) override {
        stbi_write_png(filename, width, height, 3, image, width * 3);
    }

    void set_color(const int x, const int y, const Color3 &color) {
        int pos = y * width + x;
        image[3 * pos + 0] = color.r;
        image[3 * pos + 1] = color.g;
        image[3 * pos + 2] = color.b;
        return;
    }
};
