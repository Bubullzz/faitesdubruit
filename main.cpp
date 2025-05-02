#include "includes/image.hh"
#include "includes/voronoi.hh"
#include <ctime>
int main() {
    int SEED = 0;
    if (SEED == 0) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
    }
    BW_Image im = Voronoi(512,512,3);
    im.save("../output.png");

}

