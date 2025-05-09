#pragma once
#include "image.hh"

BW_Image voronoi(int width, int height, int nb_points);

BW_Image voronoi_edges(int width, int height, int nb_points, int edge_size);
BW_Image voronoi_zone(int width, int height, int nb_points);
