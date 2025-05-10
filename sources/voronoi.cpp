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

Vec2 hash22(int x, int y) {
    // Magic number (large prime) for better distribution
    Vec2 p = Vec2(Vec2(x,y).dot(Vec2(127.1,311.7)),
                 Vec2(x,y).dot(Vec2(269.5,168.3)));
    p = Vec2(std::sin(p.x)*18.5453, std::sin(p.y)*18.5453);
    return Vec2(p.x - (long)p.x,p.y - (long)p.y);
}

float Voronoi::get_dist_to_closest(const int x, const int y) const {
    float pos_x = (float)x / (float)base_width * mult_factor;
    float pos_y = (float)y / (float)base_height * mult_factor / ((float)base_width / (float)base_height);
    Vec2 curr_point = Vec2(pos_x, pos_y);
    int uv_x = std::floor(pos_x);
    int uv_y = std::floor(pos_y);
    float min_distance = std::numeric_limits<float>::max();

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            Vec2 neigh_uv = Vec2(uv_x + i, uv_y + j);
            Vec2 offset = hash22(neigh_uv.x, neigh_uv.y) * 0.5 + Vec2(0.5, 0.5); // sets offset in [0., 1.]
            Vec2 neigh_actual = offset + neigh_uv;
            float dist = curr_point.distance(neigh_actual);
            if (dist < min_distance) {
                min_distance = dist;
            }
        }
    }
    return min_distance;
}

unsigned char Voronoi::get_closest_label(const int x, const int y) const {
    float pos_x = (float)x / (float)base_width * mult_factor;
    float pos_y = (float)y / (float)base_height * mult_factor / ((float)base_width / (float)base_height);
    Vec2 curr_point = Vec2(pos_x, pos_y);
    int uv_x = std::floor(pos_x);
    int uv_y = std::floor(pos_y);
    float min_distance = std::numeric_limits<float>::max();
    unsigned char label;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            Vec2 neigh_uv = Vec2(uv_x + i, uv_y + j);
            Vec2 offset = hash22(neigh_uv.x, neigh_uv.y) * 0.5 + Vec2(0.5, 0.5); // sets offset in [0., 1.]
            Vec2 neigh_actual = offset + neigh_uv;
            float dist = curr_point.distance(neigh_actual);
            if (dist < min_distance) {
                min_distance = dist;
                label = (unsigned char)((std::sin(offset.dot(Vec2(127.1, 311.7))) * 0.5 + 0.5) * 255.);
            }
        }
    }
    return label;
}

BW_Image Voronoi::get_closest_distance_map() const {
    BW_Image im = BW_Image(base_width, base_height);
    for (int y = 0; y < base_height; y++) {
        for (int x = 0; x < base_width; x++) {
            im[y * base_width + x] = get_dist_to_closest(x, y)* 100;
        }
    }
    return im;
}


NClosest Voronoi::get_n_closest(const int x, const int y, const int n){
    float pos_x = (float)x / (float)base_width * mult_factor;
    float pos_y = (float)y / (float)base_height * mult_factor / ((float)base_width / (float)base_height);
    Vec2 curr_point = Vec2(pos_x, pos_y);
    int uv_x = std::floor(pos_x);
    int uv_y = std::floor(pos_y);

    NClosest result(n);

    // dans le doute si le n ieme plus proche est pas dans les 9 autour gi iterer dans les 25 autour
    for (int i = -2; i <= 2; i++) {
        for (int j = -2; j <= 2; j++) {
            Vec2 neigh_uv = Vec2(uv_x + i, uv_y + j);
            Vec2 offset = hash22(neigh_uv.x, neigh_uv.y) * 0.5 + Vec2(0.5, 0.5);
            Vec2 neigh_actual = offset + neigh_uv;
            float dist = curr_point.distance(neigh_actual);

            unsigned char label = (unsigned char)((std::sin(offset.dot(Vec2(127.1, 311.7))) * 0.5 + 0.5) * 255.);

            result.tryAdd(dist, label);
        }
    }

    return result;
}

BW_Image Voronoi::get_blended_labels_sharp(int num_neighbours_checked, int num_labels_modulo) {
    BW_Image im = BW_Image(base_width, base_height);
    for (int y = 0; y < base_height; y++) {
        for (int x = 0; x < base_width; x++) {
            NClosest result = get_n_closest(x, y, num_neighbours_checked);
            float inverted_distances_sum = 0.;
            for (float dist: result.distances)
                inverted_distances_sum += 1/(dist + 0.001);
            for (int i = 0; i < result.distances.size(); i++) {
                int curr_label = result.labels[i] % num_labels_modulo;
                im[y * base_width + x] += (float)curr_label * 1/(result.distances[i] + 0.001) / inverted_distances_sum * 255 / num_labels_modulo;
            }
        }
    }
    return im;
}

BW_Image Voronoi::get_blended_labels_smooth(int num_neighbours_checked, int num_labels_modulo) {
    BW_Image im = BW_Image(base_width, base_height);
    for (int y = 0; y < base_height; y++) {
        for (int x = 0; x < base_width; x++) {
            NClosest result = get_n_closest(x, y, num_neighbours_checked);
            for (int i = 0; i < result.distances.size(); i++) {
                float k = 2.;
                int curr_label = result.labels[i] % num_labels_modulo;
                float weight = std::exp(-k * result.distances[i]);
                im[y * base_width + x] = std::clamp(0, 255, (int)im[y * base_width + x] + (int)(curr_label * weight * 255 / num_labels_modulo));
            }
        }
    }
    return im;
}





BW_Image Voronoi::get_closest_label_map(const int modulo_label) const {
    BW_Image im = BW_Image(base_width, base_height);
    for (int y = 0; y < base_height; y++) {
        for (int x = 0; x < base_width; x++) {
            im[y * base_width + x] = (get_closest_label(x, y) % modulo_label) * 255 / modulo_label;
        }
    }
    return im;
}


