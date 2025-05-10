#pragma once
#include "image.hh"


#include <vector>
#include <algorithm>
#include <limits>

class NClosest {
public:
    std::vector<unsigned char> labels;
    std::vector<float> distances;
    int n;

    NClosest(int num_closest) : n(num_closest) {
        labels.reserve(n);
        distances.reserve(n);
        // Initialize with maximum possible distances
        for (int i = 0; i < n; i++) {
            distances.push_back(std::numeric_limits<float>::max());
            labels.push_back(0);
        }
    }

    void tryAdd(float new_dist, unsigned char new_label) {
        // Check if this distance is smaller than any of our stored distances
        for (int i = 0; i < n; i++) {
            if (new_dist < distances[i]) {
                // Shift existing values down
                for (int j = n-1; j > i; j--) {
                    distances[j] = distances[j-1];
                    labels[j] = labels[j-1];
                }
                // Insert new value
                distances[i] = new_dist;
                labels[i] = new_label;
                break;
            }
        }
    }
};

// Hash Based Voronoi Implementation
class Voronoi {
private:
    int base_width, base_height;
    int SEED;
    float mult_factor;

public:
    Voronoi(const int width, const int height, const int seed, const float mult_factor) : base_width(width),
        base_height(height), SEED(seed), mult_factor(mult_factor) {
    }

    float get_dist_to_closest(const int x, const int y) const;
    BW_Image get_closest_distance_map() const;

    unsigned char get_closest_label(const int x, const int y) const;
    BW_Image get_closest_label_map(const int modulo_label) const;
    NClosest get_n_closest(const int x, const int y, const int n);
    BW_Image get_blended_labels_smooth(int num_neighbours_checked, int num_labels_modulo);
    BW_Image get_blended_labels_sharp(int num_neighbours_checked, int num_labels_modulo);

};


BW_Image voronoi(int width, int height, int nb_points);

BW_Image voronoi_edges(int width, int height, int nb_points, int edge_size);

BW_Image voronoi_zone(int width, int height, int nb_points);
