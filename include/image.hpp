#ifndef IMAGE_HPP_
#define IMAGE_HPP_

#include <string>
#include <optional>

#include "matrix.hpp"


namespace PGM {

class Image
{
public:
    Image();
    Image(const std::string &path);

    int& operator()(size_t x, size_t y) { return image_(x, y); }
    const int& operator()(size_t x, size_t y) const { return image_(x, y); };

    bool load_from_file(const std::string &path);
    bool save_to_file(const std::string &path);

    void resize(size_t width, size_t height);

    size_t width() const { return image_.elements(); }
    size_t height() const { return image_.rows(); }
    std::optional<size_t> deep() const { return deep_; }

    const math::Matrix<int>& image() const { return image_; }
private:
    std::string format_ {""};
    std::string comments_ {""};

    std::optional<size_t> deep_ {std::nullopt};

    math::Matrix<int> image_;
};

}; // namespace PGM

#endif // IMAGE_HPP_
