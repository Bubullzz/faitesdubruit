#include "../includes/portfolio.hh"

#include "../includes/image.hh"
#include "../includes/voronoi.hh"
#include "../includes/perlin.hh"
#include "../includes/functions.hh"
#include "../includes/color3.hh"

#include <ctime>

#include <functional>
using namespace std::placeholders;

void caustics() {
    int SEED = 14;
    std::srand(SEED);
    if (SEED == 0)
        srand(time(NULL));
    int width = 1024;
    int height = 1024;
    BW_Image vor1 = voronoi(width, height, 80) * 0.7;
    BW_Image vor2 = voronoi(width, height, 80) * 0.3;
    BW_Image caustiques_bw = vor1.add(vor2).increase_contrast(0.1, 3.5);
    std::vector<unsigned char> thresholds = {0, 1, 150, 255};
    std::vector<Color3> colors = {
        Color3::fromHex("#006dd5"), Color3::fromHex("#006dd5"), Color3::fromHex("ffffff"), Color3::fromHex("ffffff")
    };
    Color_Image grad = gradient(caustiques_bw, thresholds, colors);
    grad.save("../portfolio/caustics.png");
}


void giraffe() {
    int SEED = 23;
    std::srand(SEED);
    if (SEED == 0)
        srand(time(NULL));
    int width = 1024;
    int height = 1024;
    float detail = 0.2;
    BW_Image vor1 = voronoi_edges(width, height, 25, 25);
    BW_Image per = perlin(width, height, 0.05, 1.0, 4, 0.6, SEED);
    per.save("../perlin.png");
    std::vector<float> skin_normalized;

    for (int i = 0; i < width * height; i++) {
        float wb = static_cast<float>(per[i]) / 255.0;
        skin_normalized.push_back(1.0 + detail * (wb - 0.5));
    }

    std::vector<unsigned char> thresholds = {0, 1, 254, 255};
    std::vector<Color3> colors = {
        Color3::fromHex("#6F4A38"), Color3::fromHex("#6F4A38"), Color3::fromHex("EDCF8F"), Color3::fromHex("EDCF8F")
    };
    Color_Image grad = gradient(vor1, thresholds, colors);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (vor1[y * width + x] < 128) {
                Color3 base = grad.get_color(x, y);
                float factor = skin_normalized[y * width + x];

                Color3 final(
                    std::clamp(base.r * factor, 0.0f, 255.0f),
                    std::clamp(base.g * factor, 0.0f, 255.0f),
                    std::clamp(base.b * factor, 0.0f, 255.0f)
                );
                grad.set_color(x, y, final);
            }
        }
    }
    grad.save("../giraffe.png");
}


void game_map() {
    int SEED = 7;
    std::srand(SEED);
    if (SEED == 0) {
        srand(time(NULL));
        SEED = rand();
    }
    int width = 1024;
    int height = 1024;
    BW_Image per = perlin(width, height, 0.003, 1.0, 3, 0.3, SEED);
    std::vector<unsigned char> thresholds = {0, 70, 120, 150, 180, 215, 240}; //, 70, 200, 240, 255};
    std::vector<Color3> colors = {
        Color3::fromHex("#0f5e9c"), // Water Deep
        Color3::fromHex("#1ca3ec"), // Water Shallow
        Color3::fromHex("#EBE7CD"), // sand
        Color3::fromHex("#62760C"), // grass
        Color3::fromHex("#5c3b04"), // brown
        Color3::fromHex("#CBBCB1"), // mountain grey
        Color3::fromHex("#F2EFEA"), // mountain white
    };

    std::vector<Color3> colors_biome_lavender = {
        Color3::fromHex("#573b88"), // Water Deep
        Color3::fromHex("#834ba0"), // Water Shallow
        Color3::fromHex("#ad5fad"), // sand
        Color3::fromHex("#ce78b3"), // grass
        Color3::fromHex("#e597b9"), // brown
        Color3::fromHex("#f2b9c4"), // mountain grey
        Color3::fromHex("#f9ddda"), // mountain white
    };
    Color_Image cut = color_cut(per, thresholds, colors);
    Color_Image cut_lavender = color_cut(per, thresholds, colors_biome_lavender);
    cut.save("../portfolio/map.png");
    cut_lavender.save("../portfolio/map_lavender.png");
    per.save("../portfolio/tmp.png");
}

void fire() {
   int SEED = 0;
    std::srand(SEED);
    if (SEED == 0)
        SEED = time(NULL);
        std::srand(SEED);
    int width = 512;
    int height = 512;
    BW_Image image = BW_Image(width, height);

    float fade_strength = 2.0f;
    float warped_strength = 3.0f;
    float flicker_strength = 1.2f;
    float flicker_intensity = 0.5f;

    float _warped_strength = warped_strength / 10.0f;

    std::vector<unsigned char> thresholds = {0, 20, 40, 60, 80, 100, 130, 150, 170, 255}; //, 70, 200, 240, 255};

    // Very cool palette
    std::vector<Color3> colors = {
        Color3::fromHex("000000"),
        Color3::fromHex("7F0000"),
        Color3::fromHex("#FF3300"),
        Color3::fromHex("#FF6600"),
        Color3::fromHex("#FF9900"),
        Color3::fromHex("FFB200"),
        Color3::fromHex("FFCC00"),
        Color3::fromHex("FFFF00"),
        Color3::fromHex("FFFF33"),
        Color3::fromHex("FFFFFF"),
    };

    float min_value = INFINITY;
    float max_value = -INFINITY;

    std::vector<float> n_list;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            // Random fire blob movement wavy
            int warped_x = x + warped_strength * std::cos(x * _warped_strength);
            int warped_y = y + warped_strength * std::sin(y * _warped_strength);

            // Big perlin
            float perlin_noise = (perlin_octave(warped_x, warped_y, 0.02, 1.0, 2, 0.5, SEED) + 1) / 2 + 0.1f;

            float fade_height = static_cast<float>(y) / height;

            // Random fire movement
            int flicker_warped_x = x + std::cos(x * _warped_strength);
            int flicker_warped_y = y + std::sin(y * _warped_strength);

            // Small perlin
            float flicker_noise = (perlin_octave(flicker_warped_x, flicker_warped_y, 0.1, 1.0, 5, 0.7, SEED * 3) + 1) / 2;

            // Small fire texture granularity
            perlin_noise += flicker_strength * (flicker_noise - flicker_intensity);

            // Top fading to black
            perlin_noise *= std::pow(fade_height, fade_strength);

            n_list.push_back(perlin_noise);

            if (perlin_noise < min_value) {
                min_value = perlin_noise;
            }
            if (perlin_noise > max_value) {
                max_value = perlin_noise;
            }
        }
    }
    for (int i = 0; i < n_list.size(); i++) {
        float norm = (n_list[i] - min_value) / (max_value - min_value);
        image[i] = static_cast<int>(norm * 255.0f);
    }
    Color_Image cut = gradient(image, thresholds, colors);

    cut.save("../fire.png");
}