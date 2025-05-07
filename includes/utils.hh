#pragma once
#include <cstdlib>
#include <cmath>

int clamp(int value, int min, int max);

float random_float(float a, float b);

float random_angle(int hash);

float fade(float t);

float lerp(float a, float b, float w);