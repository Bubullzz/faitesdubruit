#include "../includes/functions.hh"

#include <cmath>
#include <cassert>

Color3 get_color_interpolated(unsigned char pixel_value, unsigned char low, unsigned char high, Color3 low_color,
                              Color3 high_color) {
  unsigned char diff = high - low;
  unsigned char advancement = pixel_value - low;
  float relative_pos = (float)advancement / (float)diff;
  return (low_color * (1.0f - relative_pos)) + (high_color * relative_pos);
}

Color_Image gradient(const BW_Image &in, std::vector<unsigned char> thresholds, std::vector<Color3> colors) {
  assert(thresholds.size() > 0);
  assert(thresholds.size() == colors.size());
  assert(thresholds[0] == 0 && thresholds[thresholds.size() - 1] == 255);
  Color_Image result = Color_Image(in.width, in.height);

  for (int y = 0; y < in.height; y++) {
    for (int x = 0; x < in.width; x++) {
      int pos = y * in.width + x;
      unsigned char pixel_value = in[pos];
      int curr_color_index = 1; // starting at 1 because case where pixel_value == 0 is magically managed
      while (curr_color_index < thresholds.size() && pixel_value > thresholds[curr_color_index]) {
        curr_color_index++;
      }
      if (curr_color_index == thresholds.size()) {
        result.set_color(x, y, colors[colors.size() - 1]);
        continue;
      }
      Color3 low_color = colors[curr_color_index - 1];
      Color3 high_color = colors[curr_color_index];

      Color3 final_col = get_color_interpolated(pixel_value, thresholds[curr_color_index - 1], thresholds[curr_color_index], low_color, high_color);
      result.set_color(x, y, final_col);
    }
  }
  return result;
}


Color_Image color_cut(const BW_Image &in, const std::vector<unsigned char> &thresholds, const std::vector<Color3> &colors) {
  assert(!thresholds.empty());
  assert(thresholds.size() == colors.size());
  assert(thresholds[0] == 0);
  Color_Image result = Color_Image(in.width, in.height);

  for (int y = 0; y < in.height; y++) {
    for (int x = 0; x < in.width; x++) {
      int pos = y * in.width + x;
      unsigned char pixel_value = in[pos];
      int curr_color_index = 1; // starting at 1 because case where pixel_value == 0 is magically managed
      while (curr_color_index < thresholds.size() && pixel_value > thresholds[curr_color_index]) {
        curr_color_index++;
      }
      Color3 low_color = colors[curr_color_index - 1];

      result.set_color(x, y, low_color);
    }
  }
  return result;
}

