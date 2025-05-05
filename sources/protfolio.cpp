#include "../includes/portfolio.hh"

#include "../includes/image.hh"
#include "../includes/voronoi.hh"
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
    BW_Image vor1 = voronoi(width, height,80) * 0.7;
    BW_Image vor2 = voronoi(width, height,80) * 0.3;
    BW_Image caustiques_bw = vor1.add(vor2).increase_contrast(0.1, 3.5);
    std::vector<unsigned char> thresholds = {0, 1, 150, 255};
    std::vector<Color3> colors = {Color3::fromHex("#006dd5"), Color3::fromHex("#006dd5"),Color3::fromHex("ffffff"),  Color3::fromHex("ffffff")};
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
    BW_Image vor1 = voronoi_edges(width, height,25, 10);
    std::vector<unsigned char> thresholds = {0, 1, 254, 255};
    std::vector<Color3> colors = {Color3::fromHex("#6F4A38"), Color3::fromHex("#6F4A38"),Color3::fromHex("EDCF8F"),  Color3::fromHex("EDCF8F")};
    Color_Image grad = gradient(vor1, thresholds, colors);
    grad.save("../portfolio/giraffe.png");
}