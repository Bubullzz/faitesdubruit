#pragma once

#include "../includes/vec2.hh"
#include "../includes/utils.hh"
#include "../includes/image.hh"
#include <vector>

BW_Image perlin(int width, int height, int seed);

float perlin_octave(int x, int y, float frequency, float amplitude, int octaves, float persistence, const std::function<float(float, float)>& perlin_noise);

float perlin_noise(float x, float y, int seed);

float upgraded_perlin_noise(float x, float y, std::vector<int> ptable);
