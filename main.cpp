#include <cassert>

#include "includes/functions.hh"
#include "includes/image.hh"
#include "includes/perlin.hh"
#include "includes/portfolio.hh"
#include "includes/voronoi.hh"


int main() {
    std::cout << "Generating caustics..." << std::endl;
    caustics();
    std::cout << "Generating giraffe..." << std::endl;
    giraffe();
    std::cout << "Generating game map..." << std::endl;
    game_map();
    std::cout << "Generating fire..." << std::endl;
    fire();
    std::cout << "Generating biome map..." << std::endl;
    colored_map();
}

