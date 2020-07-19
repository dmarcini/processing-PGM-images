#ifndef PARSE_SOURCE_HPP_
#define PARSE_SOURCE_HPP_

#include <string>
#include <vector>
#include <map>


namespace PGM {

class SourceString
{
public:
    SourceString(const std::string &str) : str_(str) {}

    std::string param(const char delim = '=') const
    {
        return str_.substr(str_.find(delim) + 1);
    }

    bool start_with(const std::string &ctx) const
    {
        return str_.rfind(ctx, 0) != std::string::npos;
    }

    const std::string str() const
    {
        return str_;
    }
private:
    std::string str_ {};
};


enum class Option
{
    Help,
    LoadImage,
    SaveImage,
    Rotate,
    Reflect,
    Threshold,
    Graininess,
    Negative,
    Histogram,
    Unknow
};


using OptionsVec = std::vector<PGM::SourceString>;
using OptionsMap = std::map<Option, std::string>;


OptionsMap parse_sources(OptionsVec sources)
{
    OptionsMap options;

    for (auto source : sources) {
        if (source.start_with("-h") || source.start_with("--help")) {
            options[Option::Help] = "";
        } else if (source.start_with("--load-image=")) {
            options[Option::LoadImage] = source.param();
        } else if (source.start_with("--save-image=")) {
            options[Option::SaveImage] = source.param();
        } else if (source.start_with("--rotate=")) {
            options[Option::Rotate] = source.param();
        } else if (source.start_with("--reflect=")) {
            options[Option::Reflect] = source.param();
        } else if (source.start_with("--threshold=")) {
            options[Option::Threshold] = source.param();
        } else if (source.start_with("--graininess")) {
            options[Option::Graininess] = source.param();
        } else if (source.start_with("--negative")) {
            options[Option::Negative] = "";
        } else if (source.start_with("--histogram=")) {
            options[Option::Histogram] = source.param();
        } else {
            options[Option::Unknow] = "";
        }
    }

    return options;
}

}; // namespace PGM

#endif // PARSE_SOURCE_HPP_
