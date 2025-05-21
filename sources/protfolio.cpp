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
    vor1.save("../portfolio/caustics/0_voronoi_1.png");
    vor2.save("../portfolio/caustics/1_voronoi_2.png");
    vor1.add(vor2).save("../portfolio/caustics/2_combined_voronoi.png");
    caustiques_bw.save("../portfolio/caustics/3_increased_contrast.png");
    gradient(gradient_tester(), thresholds, colors).save("../portfolio/caustics/4_colors.png");
    grad.save("../portfolio/caustics/5_caustics.png");
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
    vor1.save("../portfolio/giraffe/0_voronoi.png");
    color_cut(vor1, thresholds, colors).save("../portfolio/giraffe/1_giraffe_base.png");
    per.save("../portfolio/giraffe/2_perlin.png");
    grad.save("../portfolio/giraffe/3_giraffe.png");
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
    cut.save("../portfolio/basic_map/2_final_base.png");
    cut_lavender.save("../portfolio/basic_map/4_final_lavender.png");
    per.save("../portfolio/basic_map/0_base_perlin.png");
    color_cut(gradient_tester(), thresholds, colors).save("../portfolio/basic_map/1_base_colors.png");
    color_cut(gradient_tester(), thresholds, colors_biome_lavender).save("../portfolio/basic_map/3_lavender_colors.png");
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

    BW_Image perlin_base = BW_Image(width, height);
    BW_Image perlin_flicker = BW_Image(width, height);

    float fade_strength = 2.0f;
    float warped_strength = 3.0f;
    float flicker_strength = 1.2f;
    float flicker_intensity = 0.5f;

    float oscilation_strength = warped_strength / 10.0f;

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

    std::string path = "../portfolio/fire/palette.png";
    color_cut(gradient_tester(), thresholds, colors).save(path.c_str());

    float min_value = INFINITY;
    float max_value = -INFINITY;

    std::vector<float> n_list;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            // Random fire blob movement wavy
            int warped_x = x + warped_strength * std::cos(x * oscilation_strength);
            int warped_y = y + warped_strength * std::sin(y * oscilation_strength);

            // Big perlin
            float perlin_noise = (perlin_octave(warped_x, warped_y, 0.02, 1.0, 2, 0.5, SEED) + 1) / 2 + 0.1f;
            perlin_base[y * width + x] = static_cast<unsigned char>(perlin_noise * 255.0f);

            float fade_height = static_cast<float>(y) / height;

            // Random fire movement
            int flicker_warped_x = x + std::cos(x * oscilation_strength);
            int flicker_warped_y = y + std::sin(y * oscilation_strength);

            // Small perlin
            float flicker_noise = (perlin_octave(flicker_warped_x, flicker_warped_y, 0.1, 1.0, 5, 0.7, SEED * 3) + 1) / 2;
            perlin_flicker[y * width + x] = static_cast<unsigned char>(flicker_strength * (flicker_noise - flicker_intensity) * 255.0f);

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

    image.save("../portfolio/fire/0_fire.png");
    cut.save("../portfolio/fire/fire.png");
    perlin_base.save("../portfolio/fire/perlin_base.png");
    perlin_flicker.save("../portfolio/fire/perlin_flicker.png");
}

void colored_map() {
        int SEED = 0;
    std::srand(SEED);
    if (SEED == 0) {
        srand(time(NULL));
        SEED = rand();
    }
    std::cout << "seed: " << SEED << std::endl;
    int width = 1024;
    int height = 1024;
    int nb_neighbours = 3;
    double power = 11;
    int voronoi_factor = 5;
    int river_count = 200;
    float frequency_perlin = 0.025; // Higher = unzoom;
    Voronoi v = Voronoi(width, height, SEED, voronoi_factor);
    BW_Image per = perlin(width, height, frequency_perlin, 1.0, 3, 0.3, SEED);
    BW_Image riv = worm_perlin(per, river_count, 0.35, std::make_pair(125, 210), SEED);

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
        Color3::fromHex("#302330"), // Water Deep
        Color3::fromHex("#22577c"), // Water Shallow
        Color3::fromHex("#8a2424"), // sand
        Color3::fromHex("#b32e2e"), // grass
        Color3::fromHex("#b32e2e"), // brown
        Color3::fromHex("#e63c3c"), // mountain grey
        Color3::fromHex("#f74040"), // mountain white
    };

    std::vector<Color3> colors_biome_swamp = {
        Color3::fromHex("#062b47"), // Water Deep
        Color3::fromHex("#0a454e"), // Water Shallow
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

    std::vector all_color_rivers = {
        Color3::fromHex("#136c9c"), Color3::fromHex("#fae8e6"), Color3::fromHex("#e6823c"), Color3::fromHex("#349e2b"),
    };

    std::vector<Color_Image> full_images = {};
    for (size_t i = 0; i < all_color_biomes.size(); ++i) {
        full_images.push_back(std::move(color_cut(per, thresholds, all_color_biomes[i])));
    }
    for (size_t i = 0; i < full_images.size(); ++i) {
        std::string path = "../portfolio/colored_map/base_maps/" + std::to_string(i) + ".png";
        full_images[i].save(path.c_str());
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (riv[y * width + x] != 0) {
                for (int i = 0; i < all_color_rivers.size(); i++) {
                    float blend_factor = 0.8;
                    Color3 river_color = all_color_rivers[i];
                    full_images[i].set_color(x, y, full_images[i].get_color(x,y) * (1 - blend_factor) + river_color * blend_factor);
                }
            }
        }
    }

    for (size_t i = 0; i < full_images.size(); ++i) {
        std::string path = "../portfolio/colored_map/river_maps/" + std::to_string(i) + ".png";
        full_images[i].save(path.c_str());
    }
    double epsilon = 0.0001;
    int nb_biome = full_images.size();
    Color_Image dithered = Color_Image(width, height);
    Color_Image smooth = Color_Image(width, height);
    Color_Image nearest_cut = Color_Image(width, height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            NClosest close = v.get_n_closest(x, y, nb_neighbours);
            double curr_r = 0;
            double curr_g = 0;
            double curr_b = 0;
            float tot_weight = 0;
            std::vector<float> label_proba(nb_biome);
            for (int i = 0; i < close.distances.size(); i++) {
                int curr_label = (std::hash<int>()(close.labels[i] + std::hash<int>()(SEED))) % nb_biome;
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
            smooth.set_color(x, y, color);
            int closest_label = (std::hash<int>()(close.labels[0] + std::hash<int>()(SEED))) % nb_biome;
            nearest_cut.set_color(x,y, full_images[closest_label].get_color(x, y));
        }
    }

    for (size_t i = 0; i < all_color_biomes.size(); ++i) {
        std::string path = "../portfolio/colored_map/colors/" + std::to_string(i) + ".png";
        color_cut(gradient_tester(), thresholds, all_color_biomes[i]).save(path.c_str());
    }

    riv.save("../portfolio/colored_map/1_rivers.png");
    v.get_closest_distance_map().save("../portfolio/colored_map/2_voronoi_noise.png");
    v.get_closest_label_map(full_images.size()).save("../portfolio/colored_map/3_voronoi_labels.png");
    per.save("../portfolio/colored_map/0_perlin_noise.png");

    dithered.save("../portfolio/colored_map/6_ditherd.png");
    smooth.save("../portfolio/colored_map/5_smooth.png");
    nearest_cut.save("../portfolio/colored_map/4_nearest_cut.png");
    return;
}