//
// Created by bulle on 5/2/25.
//

#include "../includes/perlin.hh"

#include <random>

std::pair<float, float> random_gradient(int hash) {
    float angle = random_angle(hash);
    return {std::cos(angle), std::sin(angle)};
}

Vec2 generate_gradient(int X, int Y, int seed) {
    int hash = X * 1836311903 ^ Y * 2971215073; // primes numbers
    std::pair<float, float> grad = random_gradient(hash * seed);
    return Vec2(grad.first, grad.second);
}

// Simplification of the origin 2002 algorithm for 2D perlin noise
double noise_grad(int hash, float x, float y) {
    int dir = hash % 8; // gradient has 8 possible directions
    // The number that represents the direction is not important
    // We just need to ensure that all directions are described in the switch below

    switch (dir) {
        case 0:
            return x;
        case 1:
            return -x;
        case 2:
            return y;
        case 3:
            return -y;
        case 4:
            return x + y;
        case 5:
            return -x + y;
        case 6:
            return x - y;
        case 7:
            return -x - y;
        default:
            return 0; // for compiler
    }


}

// Revisited from the original implementation
// https://mrl.cs.nyu.edu/~perlu/noise
// I took some logic from the naive Perlin implementation
float upgraded_perlin_noise(float x, float y, std::vector<int> ptable) {

    int X = static_cast<int>(std::floor(x));
    int Y = static_cast<int>(std::floor(y));

    float x_unit = x - static_cast<float>(X);
    float y_unit = y - static_cast<float>(Y);

    float distLeft = x_unit;
    float distBottom = y_unit;
    float distRight = x_unit - 1; // x - (X + 1)
    float distTop = y_unit - 1; // y - (Y + 1)

    int hashBottomLeft = ptable[(ptable[X % 256] + Y) % 256];
    int hashBottomRight = ptable[(ptable[(X + 1) % 256] + Y) % 256];
    int hashTopLeft = ptable[(ptable[X % 256] + Y + 1) % 256];
    int hashTopRight = ptable[(ptable[(X + 1) % 256] + Y + 1) % 256];

    // Instead of generating random gradient, we will use
    // some predefined gradient (using hashes) for smoothness and less noise (more coherent)
    // This method is good for coherent look of some textures

    float gradBottomLeft = noise_grad(hashBottomLeft, distLeft, distBottom);
    float gradBottomRight = noise_grad(hashBottomRight, distRight, distBottom);
    float gradTopLeft = noise_grad(hashTopLeft, distLeft, distTop);
    float gradTopRight = noise_grad(hashTopRight, distRight, distTop);

    double i1 = lerp(gradBottomLeft, gradBottomRight, fade(x_unit));
    double i2 = lerp(gradTopLeft, gradTopRight, fade(x_unit));

    // Final smoothing
    return lerp(i1, i2, fade(y_unit));
}

float perlin_noise(float x, float y, int seed) {
    int X = static_cast<int>(std::floor(x));
    int Y = static_cast<int>(std::floor(y));

    float x_unit = x - static_cast<float>(X);
    float y_unit = y - static_cast<float>(Y);

    Vec2 distBottomLeft = Vec2(x_unit, y_unit);
    Vec2 distBottomRight = Vec2(x_unit - 1.0f, y_unit); // x - (X + 1)
    Vec2 distTopLeft = Vec2(x_unit, y_unit - 1.0f);  // y - (Y + 1)
    Vec2 distTopRight = Vec2(x_unit - 1.0f, y_unit - 1.0f); // x - (X + 1) && y - (Y + 1)

    Vec2 gradBottomLeft = generate_gradient(X, Y, seed);
    Vec2 gradBottomRight = generate_gradient(X + 1, Y, seed);
    Vec2 gradTopLeft  = generate_gradient(X, Y + 1, seed);
    Vec2 gradTopRight = generate_gradient(X + 1, Y + 1, seed);

    // Bottom dot product between grad and dist
    float X1Y1 = gradBottomLeft.dot(distBottomLeft);
    float X2Y1 = gradBottomRight.dot(distBottomRight);
    float i1 = lerp(X1Y1, X2Y1, fade(x_unit));

    // Top dot product between grad and dist
    float X1Y2 = gradTopLeft.dot(distTopLeft);
    float X2Y2 = gradTopRight.dot(distTopRight);
    float i2 = lerp(X1Y2, X2Y2, fade(x_unit));

    // Final smoothing
    return lerp(i1, i2, fade(y_unit));
}

float perlin_octave(int x, int y, float frequency, float amplitude, int octaves, float persistence, const std::function<float(float, float)>& perlin_noise) {
    float total = 0.0f;
    float max = 0.0f;

    for (int i = 0; i < octaves; i++) {
        float fx = static_cast<float>(x) * frequency;
        float fy = static_cast<float>(y) * frequency;
        total += perlin_noise(fx, fy) * amplitude;
        max += amplitude;
        amplitude *= persistence;
        frequency *= 2;
    }
    return total / max;
}


BW_Image perlin(int width, int height, int seed) {

    std::vector<int> p;
    p.resize(256);
    for (int i = 0; i < 256; i++) {
        p[i] = i;
    }
    std::default_random_engine generator(seed);
    std::shuffle(p.begin(), p.end(), generator);

    auto upgraded_noise = [&p](float fx, float fy) {
    return upgraded_perlin_noise(fx, fy, p);
    };

    auto naive_noise = [seed](float fx, float fy) {
    return perlin_noise(fx, fy, seed);
    };

    BW_Image image = BW_Image(width, height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
              float perlin_noise = perlin_octave(x, y, 0.01, 1.0f, 1, 0.5, naive_noise);
              perlin_noise = 255.0f * (perlin_noise + 1.0f) / 2.0f;
              image[y * width + x] = static_cast<int>(perlin_noise);
        }
    }
    return image;
}