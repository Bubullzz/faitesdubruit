#include "includes/image.hh"
#include "includes/voronoi.hh"
#include "includes/functions.hh"
#include "includes/color3.hh"
#include "includes/portfolio.hh"

#include <ctime>
int main() {
    caustics();
    int SEED = 0;
    std::srand(SEED);
    if (SEED == 0)
        srand(time(NULL));
    int width = 512;
    int height = 512;
    BW_Image voronoi_big = Voronoi(width,height,40);
    BW_Image voronoi_small = Voronoi(width,height,40);
    BW_Image pre = (voronoi_small * 0.5).add(voronoi_big * 0.5) * 1;
    BW_Image im = pre.clamp(0,190);
    std::vector<unsigned char> thresholds = {0, 0, 180, 255};
    std::vector<Color3> colors = {Color3::fromHex("#006dd5"), Color3::fromHex("#006dd5"),Color3::fromHex("c3dcf4"),  Color3::fromHex("c3dcf4")};
    Color_Image grad = gradient(im, thresholds, colors);
    voronoi_big.save("../output1.png");
    voronoi_small.save("../output2.png");
    im.save("../output3.png");
    grad.save("../output4.png");
}

