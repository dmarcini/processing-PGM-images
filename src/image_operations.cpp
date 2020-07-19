#include "image_operations.hpp"

#include <iostream>
#include <cstdlib>
#include <cctype>
#include <random>
#include <ctime>
#include <limits>

#include "image.hpp"


namespace PGM {

void rotate(Image &image, const int mult)
{
    Image rot_image {image};

    if (std::abs(mult % 4) == 1 || std::abs(mult % 4) == 3) {
        rot_image.resize(image.height(), image.width());
    }

    if (mult % 4 == 1 || mult % 4 == -3) {
        for (size_t height {0}; height < image.height(); ++height) {
            for (size_t width {0}; width < image.width(); ++width) {
                rot_image(height, width) = image(width, height);
            }
        }
    } else if (mult % 4 == 2 || mult % 4 == -2) {
        for (size_t height {0}; height < image.height(); ++height) {
            for (size_t width {0}; width < image.width(); ++width) {
                rot_image(height, width) = image(image.height() - height - 1,
                                                 image.width() - width - 1);
            }
        }
    } else if (mult % 4 == 3 || mult % 4 == -1) {
        for (size_t width {0}; width < image.width(); ++width) {
            for (size_t height {0}; height < image.height(); ++height) {
                rot_image(width, height) = image(image.height() - height - 1,
                                                 width);
            }
        }
    } else {
        for (size_t height {0}; height < image.height(); ++height) {
            for (size_t width {0}; width < image.width(); ++width) {
                rot_image(height, width) = image(height, width);
            }
        }
    }

    image = std::move(rot_image);
}


void reflect(Image &image, const char axis)
{
    Image ref_image {image};

    if (std::tolower(axis) == 'x') {
        for (size_t height {0}; height < image.height(); ++height) {
            for (size_t width {0}; width < image.width(); ++width) {
                ref_image(height, width) = image(image.height() - height - 1,
                                                 image.width() - width - 1);
            }
        }
    } else if (std::tolower(axis) == 'y') {
        for (size_t height {0}; height < image.height(); ++height) {
            for (size_t width {0}; width < image.width(); ++width) {
                ref_image(height, width) = image(height,
                                                 image.width() - width - 1);
            }
        }
    } else {
        std::cerr << "Invalid axis. The refelection failed.\n";

        return;
    }

    image = std::move(ref_image);
}


void threshold(Image &image, const int threshold)
{
    if (threshold < 0 || static_cast<size_t>(threshold)
                         > image.deep().value_or(255)) {
        std::cerr << "Invalid threshold. The thresholding failed.\n";

        return;
    }

    Image thr_image {image};

    for (size_t height {0}; height < image.height(); ++height) {
        for (size_t width {0}; width < image.width(); ++width) {
            if (thr_image(height, width) > threshold) {
                thr_image(height, width) = image.deep().value_or(255);
            } else {
                thr_image(height, width) = 0;
            }
        }
    }

    image = std::move(thr_image);
}


void graininess(Image &image, int percent)
{
    if (percent < 0 || percent > 100) {
        std::cerr << "Invalid percent. The graininess failed.\n";

        return;
    }

    static std::mt19937 mt {
        static_cast<std::mt19937::result_type>(std::time(nullptr))
    };

    const size_t pixels_num {image.height() * image.width()};
    const size_t samples_num {pixels_num * (percent / 100.0)};

    std::uniform_int_distribution dist {0, std::numeric_limits<int>::max()};

    Image grain_image {image};

    for (size_t i {0}; i < samples_num; ++i) {
        size_t y {dist(mt) % image.height()};
        size_t x {dist(mt) % image.width()};
        size_t pixel {i % 2 ? image.deep().value_or(max_deep) : 0};

        grain_image(y, x) = pixel;
    }

    image = std::move(grain_image);
}


void negative(Image &image)
{
    Image neg_image {image};

    for (size_t height {0}; height < image.height(); ++height) {
        for (size_t width {0}; width < image.width(); ++width) {
            neg_image(height, width) = image.deep().value_or(max_deep)
                                       - image(height, width);
        }
    }

    image = std::move(neg_image);
}


bool create_histogram(const Image &image, const std::string &path)
{
    std::vector<int> histogram;

    histogram.resize(image.deep().value_or(max_deep) + 1);

    for (auto &val : histogram) {
        val = 0;
    }

    for (size_t height {0}; height < image.height(); ++height) {
        for (size_t width {0}; width < image.width(); ++width) {
            ++histogram[image(height, width)];
        }
    }

    if (std::ofstream ofs(path); ofs.is_open()) {
        for (size_t i {0}; i < histogram.size(); ++i) {
            ofs << i << ';' << histogram[i] << '\n';
        }

        ofs.close();

        return true;
    }

    std::cerr << "File '" << path << "' can't be opened to write.\n";

    return false;
}

}; // namespace PGM
