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

float perlin_noise(float x, float y, int seed) {
    int X = static_cast<int>(std::floor(x));
    int Y = static_cast<int>(std::floor(y));

    float x_unit = x - static_cast<float>(X);
    float y_unit = y - static_cast<float>(Y);

    Vec2 distBottomLeft = Vec2(x_unit, y_unit);
    Vec2 distBottomRight = Vec2(x_unit - 1.0f, y_unit); // x - (X + 1)
    Vec2 distTopLeft = Vec2(x_unit, y_unit - 1.0f); // y - (Y + 1)
    Vec2 distTopRight = Vec2(x_unit - 1.0f, y_unit - 1.0f); // x - (X + 1) && y - (Y + 1)

    Vec2 gradBottomLeft = generate_gradient(X, Y, seed);
    Vec2 gradBottomRight = generate_gradient(X + 1, Y, seed);
    Vec2 gradTopLeft = generate_gradient(X, Y + 1, seed);
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

float perlin_octave(int x, int y, float frequency, float amplitude, int octaves, float persistence, int seed) {
    float total = 0.0f;
    float max = 0.0f;

    for (int i = 0; i < octaves; i++) {
        float fx = static_cast<float>(x) * frequency;
        float fy = static_cast<float>(y) * frequency;
        total += perlin_noise(fx, fy, seed) * amplitude;
        max += amplitude;
        amplitude *= persistence;
        frequency *= 2;
    }
    return total / max;
}


BW_Image perlin(int width, int height, float frequency, float amplitude, int octaves, float persistence, int seed) {
    BW_Image image = BW_Image(width, height);
    std::vector<float> noise_got;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float perlin_noise = perlin_octave(x, y, frequency, amplitude, octaves, persistence, seed);
            noise_got.push_back(perlin_noise);
        }
    }

    float max_val = *std::max_element(noise_got.begin(), noise_got.end());
    float min_val = *std::min_element(noise_got.begin(), noise_got.end());

    for (int i = 0; i < noise_got.size(); i++) {
        float norm = (noise_got[i] - min_val) / (max_val - min_val);
        image[i] = static_cast<int>(norm * 255.0f);
    }

    return image;
}

std::vector<std::pair<int, int> > find_peaks(BW_Image height_map, int max_peaks, int height_threshold, int seed) {
    std::vector<std::pair<int, int> > peaks;
    int width = height_map.width;
    int height = height_map.height;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (height_map[y * width + x] > height_threshold) {
                peaks.emplace_back(x, y);
            }
        }
    }

    std::default_random_engine generator(seed);
    std::shuffle(peaks.begin(), peaks.end(), generator);
    if (peaks.size() > max_peaks) {
        peaks.resize(max_peaks);
    }

    return peaks;
}

void make_rivers(const BW_Image height_map, BW_Image river_image, const BW_Image variation, float wobbly,
                 int height_threshold, int river_id, std::pair<int, int> last_river_point, int depth = 0) {

    // Panic exit after too many recursion
    // Can happen if river is stuck in a circle loop
    if (depth == 100) {
      return;
    }
    std::pair<int, int> neighbor_down = last_river_point;
    int width = height_map.width;
    int height = height_map.height;

    // Avoid river on border
    if (last_river_point.first < 0 || last_river_point.second < 0 || last_river_point.first >= width - 1 || last_river_point.second >= height - 1) {
        return;
    }

    int current_index = last_river_point.second * width + last_river_point.first;

    // Merge two rivers from different ids
    if (river_image[current_index] != 0 && river_image[current_index] != river_id) {
        return;
    }

    int min_height = height_map[current_index];
    river_image[current_index] = river_id;

    // River at sea level
    if (min_height <= height_threshold) {
        return;
    }

    for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
            if (x == 0 && y == 0) {
                continue;
            }

            int dx = last_river_point.first + x;
            int dy = last_river_point.second + y;

            if (dx < 0 || dy < 0 || dx >= width || dy >= height) {
                continue;
            }

            int neighbor_height = height_map[dy * width + dx];
            if (neighbor_height < min_height) {
                min_height = neighbor_height;
                neighbor_down = {dx, dy};
            }
        }
    }

    // River did not move (first check to avoid atan2 impossible value)
    if (neighbor_down == last_river_point) {
        return;
    }

    int neighbor_down_x = neighbor_down.first;
    int neighbor_down_y = neighbor_down.second;
    int last_river_point_x = last_river_point.first;
    int last_river_point_y = last_river_point.second;

    float noise = static_cast<float>(variation[neighbor_down_y * width + neighbor_down_x]) / 255.0f;
    float noised_angle = noise * 2.0 * M_PI;

    int down_direction_x = neighbor_down_x - last_river_point_x;
    int down_direction_y = neighbor_down_y - last_river_point_y;

    // Angle from direction
    float direction_angle = std::atan2(down_direction_y, down_direction_x);

    // Give directions (vector x and y perturbated)
    float mix_angle_x = std::cos(direction_angle) * (1.0f - wobbly) + std::cos(noised_angle) * wobbly;
    float mix_angle_y = std::sin(direction_angle) * (1.0f - wobbly) + std::sin(noised_angle) * wobbly;

    int angle_x = static_cast<int>(std::round(mix_angle_x));
    int angle_y = static_cast<int>(std::round(mix_angle_y));

    neighbor_down = {neighbor_down_x + angle_x, neighbor_down_y + angle_y};

    // River did not move at all
    if (neighbor_down == last_river_point) {
        return;
    }

    // Triple draw
    for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
            int nx = neighbor_down.first + x;
            int ny = neighbor_down.second + y;

            if (nx >= 0 && ny >= 0 && nx < width && ny < height) {
                river_image[ny * width + nx] = river_id;
            }
        }
    }

    return make_rivers(height_map, river_image, variation, wobbly, height_threshold, river_id, neighbor_down, depth + 1);
}

BW_Image worm_perlin(const BW_Image height_image, int max_points, float wobblyness,
                     std::pair<int, int> height_threshold, int seed) {
    // Returns the BW Image of rivers only

    BW_Image river_image = BW_Image(height_image.width, height_image.height);
    BW_Image perlin_river_variation = perlin(height_image.width, height_image.height, 0.1, 1.0, 4, 0.6, seed * 3);
    std::vector<std::pair<int, int> > peaks = find_peaks(height_image, max_points, height_threshold.second, seed);

    for (int i = peaks.size(); i > 0; i--) {
        make_rivers(height_image, river_image, perlin_river_variation, wobblyness, height_threshold.first, i, peaks[i]);
    }

    for (int y = 0; y < height_image.height; y++) {
        for (int x = 0; x < height_image.width; x++) {
            int current_index = y * height_image.width + x;
            if (river_image[current_index] != 0) {
                river_image[current_index] = 255;
            }
        }
    }

    return river_image;
}
