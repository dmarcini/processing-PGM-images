#include "image.hpp"

#include <fstream>
#include <string>
#include <iterator>

#include "matrix.hpp"
#include "string_utility.hpp"


namespace PGM {

Image::Image()
    : image_(0, 0) {}


Image::Image(const std::string &path)
    : image_(0, 0)
{
    load_from_file(path);
}


bool Image::load_from_file(const std::string &path)
{
    if (std::ifstream ifs(path); ifs.is_open()) {
        format_ = utility::getline(ifs);

        // remove comments from file
        while (ifs.peek() == '#') {
            comments_ += utility::getline(ifs) + '\n';
        }

        std::vector<std::string> dimension {};
        std::vector<std::string> pixels {};

        utility::from_string(utility::getline(ifs), ' ', dimension);

        image_.allocate(std::stoi(dimension[0]), std::stoi(dimension[1]));

        if (utility::contains(format_, "P(2|3|5|6)")) {
            deep_ = std::stoi(utility::getline(ifs));
        }

        std::string line {};

        // put all pixels in 1 line
        while (!ifs.eof()) {
            line += utility::getline(ifs) + ' ';
        }

        // split pixels to linear vector
        utility::from_string(line, '\n', pixels);

        size_t start {0};
        size_t end {width()};

        // split pixels to 2d vector where:
        // row number = image height
        // pixel number in row = image width
        for (size_t row {0}; row < height(); ++row) {
            std::ostringstream oss;

            std::copy(pixels.begin() + start, pixels.begin() + end,
                      std::ostream_iterator<std::string>(oss, " "));

            start = end;
            end += width();

            image_.fill_row(row, oss.str());
        }

        ifs.close();

        return true;
    }

    std::cerr << "File '" << path << "' can't be opened to read.\n";

    return false;
}


bool Image::save_to_file(const std::string &path)
{
    if (std::ofstream ofs(path); ofs.is_open()) {
        ofs << format_ << '\n';
        ofs << comments_;
        ofs << image_.elements() << " " << image_.rows() << '\n';

        if (deep_.has_value()) {
            ofs << deep_.value() << '\n';
        }

        for (size_t row {0}; row < height(); ++row) {
            for (size_t element {0}; element < width(); ++element) {
                ofs << image_(row, element) << " ";
            }
            ofs << '\n';
        }

        ofs.close();

        return true;
    }

    std::cerr << "File '" << path << "' can't be opened to write.\n";

    return false;
}


void Image::resize(size_t width, size_t height)
{
    image_.resize(height, width);
}

}; // namespace PGM
