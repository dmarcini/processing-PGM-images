#ifndef IMAGE_OPERATIONS_HPP_
#define IMAGE_OPERATIONS_HPP_

#include "image.hpp"


namespace PGM {

constexpr int max_deep {255};

void rotate(Image &image, int mult);
void reflect(Image &image, char axis);
void threshold(Image &image, int treshold);
void graininess(Image &image, int percent);
void negative(Image &image);

bool create_histogram(const Image &image, const std::string &path);

}; // namespace PGM

#endif // IMAGE_OPERATIONS_HPP_
