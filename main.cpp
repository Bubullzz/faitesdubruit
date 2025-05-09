#include <cassert>

#include "includes/functions.hh"
#include "includes/image.hh"
#include "includes/perlin.hh"
#include "includes/portfolio.hh"
#include "includes/voronoi.hh"


int main() {
    game_map();
    int SEED = 0;
    std::srand(SEED);
    if (SEED == 0) {
        srand(time(NULL));
        SEED = rand() ;
    }
    int width = 1024;
    int height = 1024;

    int nb_points = width * height;
    BW_Image per = perlin(width, height, 0.005, 1.0, 3, 0.3, SEED);

    std::vector<Color3> colors_biome_basic = {
        Color3::fromHex("#0f5e9c"), // Water Deep
        Color3::fromHex("#1ca3ec"), // Water Shallow
        Color3::fromHex("#EBE7CD"), // sand
        Color3::fromHex("#62760C"), // grass
        Color3::fromHex("#5c3b04"), // brown
        Color3::fromHex("#CBBCB1"), // mountain grey
        Color3::fromHex("#F2EFEA"), // mountain white
    };

    std::vector<Color3> colors_biome_lavender = {
        Color3::fromHex("#f9ddda"), // Water Deep
        Color3::fromHex("#f2b9c4"), // Water Shallow
        Color3::fromHex("#e597b9"), // sand
        Color3::fromHex("#ce78b3"), // grass
        Color3::fromHex("#ad5fad"), // brown
        Color3::fromHex("#834ba0"), // mountain grey
        Color3::fromHex("#573b88"), // mountain white
    };

    std::vector<unsigned char> thresholds = {0, 70, 120, 150, 180, 215, 240};

    std::vector all_color_biomes = {
        colors_biome_basic, colors_biome_lavender
    };

    int w_offset = width / 10;
    int h_offset = height / 10;
    std::vector<Vec2> points = std::vector<Vec2>(nb_points);
    for (int i = 0; i < nb_points; i++) {
        points[i].x = random_float(0, width + w_offset);
        points[i].y = random_float(0, height + h_offset);
    }

    std::vector<Color_Image> full_images = {};
    for (size_t i = 0; i < all_color_biomes.size(); ++i) {
        full_images.push_back(std::move(color_cut(per, thresholds, all_color_biomes[i])));
    }

    int size = full_images.size();
    Color_Image final = Color_Image(width, height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
        }
    }

    final.save("../lucas.png");
    return 0;
}

