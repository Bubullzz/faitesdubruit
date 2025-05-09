#pragma once

#include "image.hh"
#include "color3.hh"
#include <vector>

/**
 * Function to apply different colors on each pixel of a BW_image
 * interpolates the value between 2 colors eh wolla je sais ce que je fais c bon
 * assuming an entry like
 * thresholds : [0, 128, 255]
 * colors : [blue, red, yellow]
 * pixels between 0 and 128 will go from blue to red, from 128 to 255 from red to yellow
 * it is assumed thresholds starts with 0 and ends with 255
 **/
Color_Image gradient(const BW_Image &in, std::vector<unsigned char> thresholds, std::vector<Color3> colors);
Color_Image color_cut(const BW_Image &in, const std::vector<unsigned char> &thresholds, const std::vector<Color3> &colors);
