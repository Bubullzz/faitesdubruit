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
    BW_Image vor1 = Voronoi(width, height,80) * 0.7;
    BW_Image vor2 = Voronoi(width, height,80) * 0.3;
    BW_Image caustiques_bw = vor1.add(vor2).apply_fun(std::bind(add_offset_then_pow, _1, 0.1, 3.5));
    std::vector<unsigned char> thresholds = {0, 1, 150, 255};
    std::vector<Color3> colors = {Color3::fromHex("#006dd5"), Color3::fromHex("#006dd5"),Color3::fromHex("ffffff"),  Color3::fromHex("ffffff")};
    Color_Image grad = gradient(caustiques_bw, thresholds, colors);
    grad.save("../portfolio/caustics.png");
}