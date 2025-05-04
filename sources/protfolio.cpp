#include "../includes/portfolio.hh"

#include "../includes/image.hh"
#include "../includes/voronoi.hh"
#include "../includes/functions.hh"
#include "../includes/color3.hh"

#include <ctime>

void caustics() {
    int SEED = 8;
    std::srand(SEED);
    if (SEED == 0)
        srand(time(NULL));
    int width = 1024;
    int height = 1024;
    BW_Image vor1 = Voronoi(width, height,80) * 1;
    BW_Image caustiques_bw = vor1.apply_fun(add_offset_then_exp);
    std::vector<unsigned char> thresholds = {0, 1, 150, 255};
    std::vector<Color3> colors = {Color3::fromHex("#006dd5"), Color3::fromHex("#006dd5"),Color3::fromHex("ffffff"),  Color3::fromHex("ffffff")};
    Color_Image grad = gradient(caustiques_bw, thresholds, colors);
    vor1.save("../output1.png");
    grad.save("../output2.png");
}