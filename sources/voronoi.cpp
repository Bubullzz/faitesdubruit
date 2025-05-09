#include "../includes/voronoi.hh"
#include "../includes/vec2.hh"
#include "../includes/utils.hh"

#include <vector>
#include <utility>
float closest(Vec2 current, const std::vector<Vec2> &points) {
    float min_distance = std::numeric_limits<float>::max();
    for (const Vec2 &v: points) {
        float distance = current.distance(v);
        if (distance < min_distance)
            min_distance = distance;
    }
    return clamp(min_distance, 0, 255);
}

unsigned char closest_label(Vec2 current, const std::vector<Vec2> &points) {
    float min_distance = std::numeric_limits<float>::max();
    unsigned char label = 0;
    for (const Vec2 &v: points) {
        float distance = current.distance(v);
        if (distance < min_distance) {
            min_distance = distance;
            label = std::hash<unsigned char>()(std::hash<unsigned char>()(v.x) * std::hash<unsigned char>()(v.y));
        }
    }
    return label;
}

float closestDifference(Vec2 current, const std::vector<Vec2>& points) {
    // Ensure there are at least two points to compare
    if (points.size() < 2) {
        return 0.0f; // Or throw an error if appropriate
    }

    float min1 = std::numeric_limits<float>::max();
    float min2 = std::numeric_limits<float>::max();

    for (const Vec2& v : points) {
        float distance = current.distance(v);
        if (distance < min1) {
            min2 = min1;
            min1 = distance;
        } else if (distance < min2) {
            min2 = distance;
        }
    }
    return std::abs(min2 - min1);
}

BW_Image voronoi(int width, int height, int nb_points) {
    // offset so that it's like there are points "outside" the screen
    int w_offset = width / 10;
    int h_offset = height / 10;
    std::vector<Vec2> points = std::vector<Vec2>(nb_points);
    for (int i = 0; i < nb_points; i++) {
        points[i].x = random_float(0, width + w_offset);
        points[i].y = random_float(0, height + h_offset);
    }

    BW_Image image = BW_Image(width, height);
    float max_distance = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float f = closest(Vec2(x, y), points);
            if (f > max_distance) {
                max_distance = f;
            }
        }
    }
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            image[y * width + x] = closest(Vec2(x, y), points) / max_distance * 255;
        }
    }
    return image;
}

BW_Image voronoi_edges(int width, int height, int nb_points, int edge_size) {

    // offset so that it's like there are points "outside" the screen
    // makes the output more uniform
    float offset_ratio = 1.1;
    int offseted_width = width * offset_ratio;
    int offseted_height = height * offset_ratio;
    int offset_width_diff = offseted_width - width;
    int offset_height_diff = offseted_height - height;
    std::vector<Vec2> points = std::vector<Vec2>(nb_points);

    // * 1.1 so that we can create points outside the window
    int cell_side_size = std::sqrt(offseted_width * offseted_height / nb_points);

    int width_subdiv = offseted_width / cell_side_size;
    int height_subdiv = offseted_height / cell_side_size;

    for (int i = 0; i < width_subdiv; i++) {
        for (int j = 0; j < height_subdiv; j++) {
            points[i * height_subdiv + j].x = random_float(i * cell_side_size, (i+1) * cell_side_size) - offset_width_diff / 2;
            points[i * height_subdiv + j].y = random_float(j * cell_side_size, (j+1) * cell_side_size) - offset_height_diff / 2;
        }
    }

    BW_Image image = BW_Image(width, height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float closests_diff = closestDifference(Vec2(x, y), points);
            image[y * width + x] = closests_diff < edge_size ? 255 : 0;
        }
    }
    return image;
}

BW_Image voronoi_zone(const int width, const int height, const int nb_points) {
    int w_offset = width / 10;
    int h_offset = height / 10;
    std::vector<Vec2> points = std::vector<Vec2>(nb_points);
    for (int i = 0; i < nb_points; i++) {
        points[i].x = random_float(0, width + w_offset);
        points[i].y = random_float(0, height + h_offset);
    }
    BW_Image image = BW_Image(width, height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            image[y * width + x] = closest_label(Vec2(x, y), points);
        }
    }
    return image;
}
