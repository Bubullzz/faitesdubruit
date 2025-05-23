#pragma once

#include "../includes/vec2.hh"
#include "../includes/utils.hh"
#include "../includes/image.hh"
#include <vector>

BW_Image perlin(int width, int height, float frequency, float amplitude, int octaves, float persistence, int seed);

float perlin_octave(int x, int y, float frequency, float amplitude, int octaves, float persistence, int seed);

float perlin_noise(float x, float y, int seed);

float upgraded_perlin_noise(float x, float y, std::vector<int> ptable);

BW_Image worm_perlin(const BW_Image height_image, int max_points, float wobblyness, std::pair<int, int> height_threshold, int seed);
