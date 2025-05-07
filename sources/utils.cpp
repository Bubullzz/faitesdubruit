//
// Created by dluca on 07/05/25.
//

#include "../includes/utils.hh"

int clamp(int value, int min, int max) {
    if (value < min) {
        return min;
    }
    if (value > max) {
        return max;
    }
    return value;
}

float random_float(float a, float b) {
    float random = (float) std::rand() / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

float random_angle(int hash) {
    std::srand(hash);
    return 2 * M_PI * (float)std::rand() / (float) RAND_MAX;
}

float lerp(float a, float b, float w) {
    return a + (b - a) * w;
}

float fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}
