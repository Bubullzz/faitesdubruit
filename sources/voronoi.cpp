#include "../includes/voronoi.hh"
#include "../includes/vec2.hh"
#include "../includes/utils.hh"

#include <vector>

float closest(Vec2 current, const std::vector<Vec2> &points) {
    float min_distance = std::numeric_limits<float>::max();
    for (const Vec2 &v: points) {
        float distance = current.distance(v);
        if (distance < min_distance)
            min_distance = distance;
    }
    return clamp(min_distance, 0, 255);
}

BW_Image Voronoi(int width, int height, int nb_points) {
    std::vector<Vec2> points = std::vector<Vec2>(nb_points);
    for (int i = 0; i < nb_points; i++) {
        points[i].x = random_float(0, width);
        points[i].y = random_float(0, height);
    }

    BW_Image image = BW_Image(width, height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            image[y * width + x] = closest(Vec2(x, y), points);
        }
    }
    return image;
}
