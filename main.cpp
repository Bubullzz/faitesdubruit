#include <cassert>

#include "includes/functions.hh"
#include "includes/image.hh"
#include "includes/perlin.hh"
#include "includes/portfolio.hh"
#include "includes/voronoi.hh"


int main() {
    int SEED = 0;
    std::srand(SEED);
    if (SEED == 0) {
        srand(time(NULL));
        SEED = rand();
    }
    int width = 1024;
    int height = 1024;
    int nb_neighbours = 3;
    double power = 11;
    int voronoi_factor = 5;
    int river_count = 200;
    float frequency_perlin = 0.025; // Higher = unzoom;
    Voronoi v = Voronoi(width, height, SEED, voronoi_factor);
    v.get_blended_labels_smooth(8, 2).save("../smooth_label.png");
    BW_Image per = perlin(width, height, frequency_perlin, 1.0, 3, 0.3, SEED);
    BW_Image riv = worm_perlin(per, river_count, 0.35, std::make_pair(119, 181), SEED);

    std::vector<Color3> colors_biome_basic = {
        Color3::fromHex("#0c4875"), // Water Deep
        Color3::fromHex("#136c9c"), // Water Shallow
        Color3::fromHex("#4d3f27"), // sand
        Color3::fromHex("#6b5734"), // idf
        Color3::fromHex("#947848"), // brown
        Color3::fromHex("#b59762"), // mountain grey
        Color3::fromHex("#c9ad7d"), // mountain white
    };

    std::vector<Color3> colors_biome_lavender = {
        Color3::fromHex("#0c4875"), // Water Deep
        Color3::fromHex("#136c9c"), // Water Shallow
        Color3::fromHex("#ad5fad"), // sand
        Color3::fromHex("#ce78b3"), // grass
        Color3::fromHex("#e597b9"), // brown
        Color3::fromHex("#f2b9c4"), // mountain grey
        Color3::fromHex("#f9ddda"), // mountain white
    };

    std::vector<Color3> colors_biome_nether = {
        Color3::fromHex("#292431"), // Water Deep
        Color3::fromHex("#22577c"), // Water Shallow
        Color3::fromHex("#8a2424"), // sand
        Color3::fromHex("#b32e2e"), // grass
        Color3::fromHex("#b32e2e"), // brown
        Color3::fromHex("#e63c3c"), // mountain grey
        Color3::fromHex("#f74040"), // mountain white
    };

    std::vector<Color3> colors_biome_swamp = {
        Color3::fromHex("#0c4875"), // Water Deep
        Color3::fromHex("#136c9c"), // Water Shallow
        Color3::fromHex("#1e381f"), // sand
        Color3::fromHex("#2e5730"), // grass
        Color3::fromHex("#396b45"), // brown
        Color3::fromHex("#789c80"), // mountain grey
        Color3::fromHex("#a3d4ae"), // mountain white
    };

    std::vector<unsigned char> thresholds = {0, 70, 120, 150, 180, 215, 240};

    std::vector all_color_biomes = {
        colors_biome_basic, colors_biome_lavender, colors_biome_nether, colors_biome_swamp
    };

    std::vector<Color_Image> full_images = {};
    for (size_t i = 0; i < all_color_biomes.size(); ++i) {
        full_images.push_back(std::move(color_cut(per, thresholds, all_color_biomes[i])));
    }

    double epsilon = 0.0001;
    int nb_biome = full_images.size();
    Color_Image dithered = Color_Image(width, height);
    Color_Image smooth = Color_Image(width, height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            NClosest close = v.get_n_closest(x, y, nb_neighbours);
            double curr_r = 0;
            double curr_g = 0;
            double curr_b = 0;
            float tot_weight = 0;
            std::vector<float> label_proba(nb_biome);
            for (int i = 0; i < close.distances.size(); i++) {
                int curr_label = close.labels[i] % nb_biome;
                Color3 curr_color = full_images[curr_label].get_color(x, y);
                float weight = 1.0f / (std::pow(close.distances[i], power) + epsilon);
                tot_weight += weight;
                curr_r += weight * curr_color.r;
                curr_g += weight * curr_color.g;
                curr_b += weight * curr_color.b;
                label_proba[curr_label] += weight;
            }
            for (int i = 0; true; i = (i + 1) % nb_biome) {
                float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
                if (r < label_proba[i] / tot_weight) {
                    dithered.set_color(x, y, full_images[i].get_color(x, y));
                    break;
                }
            }
            Color3 color = Color3(curr_r / tot_weight, curr_g / tot_weight, curr_b / tot_weight);
            if (riv[y * width + x] != 0) {
                float blend_factor = 0.3;
                Color3 river_color = Color3::fromHex("#136c9c");
                color = Color3(blend_factor * river_color.r + color.r * (1 - blend_factor), blend_factor * river_color.g + color.g * (1 - blend_factor), blend_factor * river_color.b + color.b * (1 - blend_factor));
            }
            smooth.set_color(x, y, color);
        }
    }

    full_images[0].save("../lucas.png");
    full_images[1].save("../lucas1.png");
    dithered.save("../ditherd.png");
    smooth.save("../smooth.png");
    return 0;
}

