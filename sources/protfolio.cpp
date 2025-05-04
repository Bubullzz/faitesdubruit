#include "../includes/portfolio.hh"

#include "../includes/image.hh"
#include "../includes/voronoi.hh"
#include "../includes/functions.hh"
#include "../includes/color3.hh"

#include <ctime>

void caustics() {
    int SEED = 12;
    std::srand(SEED);
    BW_Image im = Voronoi(512,512,40);
    std::vector<unsigned char> thresholds = {0, 1, 150, 255};
    std::vector<Color3> colors = {Color3::fromHex("#006dd5"), Color3::fromHex("#006dd5"),Color3::fromHex("ffffff"),  Color3::fromHex("ffffff")};
    Color_Image grad = gradient(im * 0.7, thresholds, colors);
    grad.save("../portfolio/caustics.png");
}